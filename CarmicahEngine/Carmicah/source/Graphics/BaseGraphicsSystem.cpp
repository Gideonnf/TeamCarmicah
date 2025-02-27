/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Similar function used across all rendering systems to check if uniform exists in shader

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/BaseGraphicsSystem.h"
#include "Graphics/RenderHelper.h"
#include "Systems/AssetManager.h"
#include "ECS/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "log.h"

namespace Carmicah
{
	void BaseGraphicsSystem::Init(const std::string& shdr)
	{
		auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(shdr);
		mCurrShader = shdrRef.s;
		mNearestDepth = AssetManager::GetInstance()->enConfig.minImgDepth;
		mFurtherstDepth = AssetManager::GetInstance()->enConfig.maxImgDepth;
	}

	float BaseGraphicsSystem::CalcDepth(const float& depth, const RENDER_LAYERS& layer)
	{
		float localRange = (mNearestDepth - mFurtherstDepth);// 2
		float trueRange = localRange * static_cast<float>(MAX_LAYERS);// 2 * 4 = 8
		return -(depth + localRange * static_cast<float>(layer) + mNearestDepth) / trueRange;// ([-1~1] + [1,3,5,7]) / 800
	}

	void BaseGraphicsSystem::SetNewEntity(const unsigned int& entity, const std::string& primitiveName, const unsigned int& id, bool worldSpace, bool isDebug)
	{
		RenderHelper::BufferID bufferID(AssetManager::GetInstance()->GetAsset<BasePrimitive>(primitiveName).uid, mCurrShader, worldSpace, id);

		if (RenderHelper::GetInstance()->mBufferMap.find(bufferID) == RenderHelper::GetInstance()->mBufferMap.end())
			GenBatch(primitiveName, id, worldSpace, isDebug);

		EntityData newDat;
		ToggleActiveEntity(true);
		newDat.mBufferData = &RenderHelper::GetInstance()->mBufferMap.find(bufferID)->second;

		if (newDat.mBufferData->freeData.size() != 0)
		{
			newDat.posInMemory = newDat.mBufferData->freeData.front();
			newDat.mBufferData->freeData.pop();
		}
		else
		{
			newDat.posInMemory = newDat.mBufferData->objCount++;
			if (newDat.mBufferData->objCount >= newDat.mBufferData->buffer.size() * AssetManager::GetInstance()->enConfig.batchRenderSize)
				GenBatch(primitiveName, id, worldSpace, isDebug);
		}
		mEntityBufferLoc.emplace(entity, newDat);
		return;
	}

	void BaseGraphicsSystem::GenBatch(const std::string& primitiveName, unsigned int id, bool worldSpace, bool isDebug, bool isForced)
	{
		bool newBatch{ false };
		RenderHelper::BufferID bufferID(AssetManager::GetInstance()->GetAsset<BasePrimitive>(primitiveName).uid, mCurrShader, worldSpace, id);

		if (RenderHelper::GetInstance()->mBufferMap.find(bufferID) == RenderHelper::GetInstance()->mBufferMap.end())
		{
			RenderHelper::GetInstance()->mBufferMap.emplace(bufferID, BatchBuffer{});
			newBatch = true;
		}

		BatchBuffer& ret = RenderHelper::GetInstance()->mBufferMap.find(bufferID)->second;
		ret.isDebug = isDebug;
		int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
		if (!isForced && (ret.freeData.size() != 0 || ret.buffer.size() * mBatchSize > ret.objCount))
		{
			std::stringstream ss;
			ss << "Shader: " << mCurrShader << " + Primitive: " << primitiveName << " is not yet filled\n";
			CM_CORE_INFO(ss.str());
			return;
		}

		BatchBuffer::BBuffer bufferData;

		glCreateVertexArrays(1, &bufferData.vao);
		glCreateBuffers(1, &bufferData.vbo);

		if (!isDebug)
		{
			Primitive& p = AssetManager::GetInstance()->GetAsset<Primitive>(primitiveName);
			ret.pRef = &p;

			// Create The GPU things
			glNamedBufferStorage(bufferData.vbo, mBatchSize * sizeof(vtxTexd2D) * p.vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);

			// layout (location=0) in vec2 aVertexPosition;
			glEnableVertexArrayAttrib(bufferData.vao, 0);
			glVertexArrayVertexBuffer(bufferData.vao, 0, bufferData.vbo, 0, sizeof(vtxTexd2D));
			glVertexArrayAttribFormat(bufferData.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 0, 0);
			// layout (location=1) in vec2 aTextureCoord;
			glEnableVertexArrayAttrib(bufferData.vao, 1);
			glVertexArrayVertexBuffer(bufferData.vao, 1, bufferData.vbo, sizeof(Vec2f), sizeof(vtxTexd2D));
			glVertexArrayAttribFormat(bufferData.vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 1, 1);
			// layout (location=2) in vec4 aColor;
			glEnableVertexArrayAttrib(bufferData.vao, 2);
			glVertexArrayVertexBuffer(bufferData.vao, 2, bufferData.vbo, sizeof(Vec2f) * 2, sizeof(vtxTexd2D));
			glVertexArrayAttribFormat(bufferData.vao, 2, 4, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 2, 2);
			// layout (location=3) in float aDepth;
			glEnableVertexArrayAttrib(bufferData.vao, 3);
			glVertexArrayVertexBuffer(bufferData.vao, 3, bufferData.vbo, sizeof(float) * 4 + sizeof(Vec2f) * 2, sizeof(vtxTexd2D));
			glVertexArrayAttribFormat(bufferData.vao, 3, 1, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 3, 3);
			// layout (location=4) in uvec2 aID;
			glEnableVertexArrayAttrib(bufferData.vao, 4);
			glVertexArrayVertexBuffer(bufferData.vao, 4, bufferData.vbo, sizeof(float) * 5 + sizeof(Vec2f) * 2, sizeof(vtxTexd2D));
			glVertexArrayAttribIFormat(bufferData.vao, 4, 2, GL_UNSIGNED_INT, 0);
			glVertexArrayAttribBinding(bufferData.vao, 4, 4);

			if (newBatch)
			{
				// Indexing
				glCreateBuffers(1, &ret.ebo);
				glNamedBufferStorage(ret.ebo, sizeof(GLushort) * p.drawCnt, p.idx.data(), GL_MAP_WRITE_BIT);
			}
		}
		else
		{
			BasePrimitive& p = AssetManager::GetInstance()->GetAsset<BasePrimitive>(primitiveName);
			ret.pRef = &p;

			glNamedBufferStorage(bufferData.vbo, mBatchSize * sizeof(vtx2D) * p.vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);

			// layout(location=0) in vec2 aVertexPosition;
			glEnableVertexArrayAttrib(bufferData.vao, 0);
			glVertexArrayVertexBuffer(bufferData.vao, 0, bufferData.vbo, 0, sizeof(vtx2D));
			glVertexArrayAttribFormat(bufferData.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 0, 0);
			// layout(location=1) in float aDepth;
			glEnableVertexArrayAttrib(bufferData.vao, 1);
			glVertexArrayVertexBuffer(bufferData.vao, 1, bufferData.vbo, sizeof(Vec2f), sizeof(vtx2D));
			glVertexArrayAttribFormat(bufferData.vao, 1, 1, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(bufferData.vao, 1, 1);
			// layout (location=2) in unsigned int aID;
			glEnableVertexArrayAttrib(bufferData.vao, 2);
			glVertexArrayVertexBuffer(bufferData.vao, 2, bufferData.vbo, sizeof(Vec2f) + sizeof(float), sizeof(vtx2D));
			glVertexArrayAttribIFormat(bufferData.vao, 2, 1, GL_UNSIGNED_INT, 0);
			glVertexArrayAttribBinding(bufferData.vao, 2, 2);

			if (newBatch)
			{
				// Indexing
				std::vector<GLushort> idxOrder{};
				idxOrder.reserve(p.drawCnt);
				for (GLushort i{}; i < static_cast<GLushort>(p.drawCnt); ++i)
					idxOrder.emplace_back(i);
				glCreateBuffers(1, &ret.ebo);
				glNamedBufferStorage(ret.ebo, sizeof(GLushort) * p.drawCnt, idxOrder.data(), GL_MAP_WRITE_BIT);
			}
		}

		glVertexArrayElementBuffer(bufferData.vao, ret.ebo);

		if (newBatch)
		{
			// Instancing Pt.2
			std::vector<DrawElemCmd> vDrawCmd;
			vDrawCmd.reserve(mBatchSize);
			for (int i{}; i < mBatchSize; ++i)
			{
				DrawElemCmd dec;
				dec.vtxCnt = ret.pRef->drawCnt;
				dec.instanceCnt = 1;
				dec.firstIdx = 0;
				dec.baseVtx = i * static_cast<GLuint>(ret.pRef->vtx.size());
				dec.baseInstance = i;
				vDrawCmd.emplace_back(dec);
			}
			glCreateBuffers(1, &ret.ibo);
			glNamedBufferStorage(ret.ibo, mBatchSize * sizeof(DrawElemCmd), vDrawCmd.data(), GL_MAP_WRITE_BIT);
		}

		ret.buffer.push_back(bufferData);
		glBindVertexArray(0);
	}

	void BaseGraphicsSystem::EditBatchData(const unsigned int& entity, bool worldSpace, RENDER_LAYERS layer)
	{
		EntityData& dat = mEntityBufferLoc.find(entity)->second;
		int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;

		// If invalid
		if (!ComponentManager::GetInstance()->HasComponent<Renderer>(entity))
		{
			return;
		}
		auto& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
		auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(renderer.model) };
		auto& tryTex = AssetManager::GetInstance()->GetAsset<Texture>(renderer.Texture());

		float depth{};
		Mtx3x3f wSpace{};
		if (worldSpace)
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			depth = t.Depth();
			wSpace = t.worldSpace;
		}
		else
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<UITransform>(entity);
			depth = t.Depth();
			wSpace = t.worldSpace;
		}
		depth = CalcDepth(depth, layer);

		std::vector<vtxTexd2D> temp;
		temp.reserve(p.vtx.size());
		for (unsigned int i{}; i < p.vtx.size(); ++i)
		{
			vtxTexd2D tt;
			tt.pos = wSpace * p.vtx[i];
			tt.ids[0] = entity;
			tt.ids[1] = tryTex.t;
			renderer.Color(tt.color);
			tt.depth = depth;
			tt.uv = tryTex.mtx * p.texCoord[i];
			temp.emplace_back(tt);
		}

		glNamedBufferSubData(dat.mBufferData->buffer[dat.posInMemory / mBatchSize].vbo, sizeof(vtxTexd2D) * p.vtx.size() * (dat.posInMemory % mBatchSize), sizeof(vtxTexd2D) * p.vtx.size(), temp.data());

	}

	void BaseGraphicsSystem::EditDebugBatchData(const unsigned int& entity, Mtx3x3f& mtx, bool worldSpace, RENDER_LAYERS layer)
	{
		EntityData& dat = mEntityBufferLoc.find(entity)->second;
		int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;

		float depth{};
		Mtx3x3f wSpace{};
		if (worldSpace)
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			depth = t.Depth();
		}
		else
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<UITransform>(entity);
			depth = t.Depth();
		}
		depth = CalcDepth(depth, layer);

		std::vector<vtx2D> temp;
		BasePrimitive const* p = dat.mBufferData->pRef;
		temp.reserve(p->drawCnt);
		for (unsigned int i{}; i < p->drawCnt; ++i)
		{
			vtx2D tt;
			tt.pos = mtx * p->vtx[i];
			tt.depth = depth;
			tt.id = entity;
			temp.emplace_back(tt);
		}
		glNamedBufferSubData(dat.mBufferData->buffer[dat.posInMemory / mBatchSize].vbo, sizeof(vtx2D) * p->drawCnt * (dat.posInMemory % mBatchSize), sizeof(vtx2D) * p->drawCnt, temp.data());

	}

	void BaseGraphicsSystem::ToggleActiveEntity(bool setActive)
	{
		if (setActive)
			++mActiveEntityCount;
		else
			--mActiveEntityCount;
	}

	void BaseGraphicsSystem::DeleteBatchData(const unsigned int& entity, bool eraseFromMap)
	{
		EntityData& dat = mEntityBufferLoc.find(entity)->second;
		int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
		size_t vtxSize = dat.mBufferData->pRef->vtx.size();
		ToggleActiveEntity(false);

		// Flush data with 0
		if (!dat.mBufferData->isDebug)
		{
			std::vector<vtxTexd2D> temp;
			temp.resize(vtxSize);

			glNamedBufferSubData(dat.mBufferData->buffer[dat.posInMemory / mBatchSize].vbo, sizeof(vtxTexd2D) * vtxSize * (dat.posInMemory % mBatchSize), sizeof(vtxTexd2D) * vtxSize, temp.data());
		}
		else
		{
			std::vector<vtx2D> temp;
			temp.resize(vtxSize);

			glNamedBufferSubData(dat.mBufferData->buffer[dat.posInMemory / mBatchSize].vbo, sizeof(vtx2D) * vtxSize * (dat.posInMemory % mBatchSize), sizeof(vtx2D) * vtxSize, temp.data());
		}

		dat.mBufferData->freeData.push(dat.posInMemory);
		if(eraseFromMap)
			mEntityBufferLoc.erase(entity);
	}
}
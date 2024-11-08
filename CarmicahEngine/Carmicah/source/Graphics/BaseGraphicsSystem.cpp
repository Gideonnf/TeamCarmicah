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
#include "Systems/AssetManager.h"
#include "ECS/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "log.h"

namespace Carmicah
{
	void BaseGraphicsSystem::Init(std::string shdr)
	{
		if (shdr.empty())
		{
			auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.defaultShader);
			mCurrShader = shdrRef.s;
		}
		else
		{
			auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(shdr);
			mCurrShader = shdrRef.s;
		}
		mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
		mNearestDepth = AssetManager::GetInstance()->enConfig.minImgDepth;
		mFurtherstDepth = AssetManager::GetInstance()->enConfig.maxImgDepth;
	}

	bool BaseGraphicsSystem::UniformExists(const GLuint& shdr, const char* str, GLint& ref)
	{
		ref = glGetUniformLocation(shdr, str);
		if (ref >= 0)
			return true;

		std::stringstream ss;
		ss << "Uniform variable: " << str << " dosen't exist!!!\n";
		CM_CORE_ERROR(ss.str());

		return false;
	}

	float BaseGraphicsSystem::CalcDepth(const float& depth, const RENDER_LAYERS& layer)
	{
		float localRange = (mNearestDepth - mFurtherstDepth);// 2
		float trueRange = localRange * static_cast<float>(MAX_LAYERS);// 2 * 4 = 8
		return -(depth + localRange * static_cast<float>(layer) + mNearestDepth) / trueRange;// ([-1~1] + [1,3,5,7]) / 800
	}

	void BaseGraphicsSystem::GenBatch(const Primitive& p)
	{
		BatchBuffer ret{};
		ret.pRef = &p;

		// Create The GPU things
		glCreateVertexArrays(1, &ret.vao);
		glCreateBuffers(1, &ret.vbo);
		glNamedBufferStorage(ret.vbo, mBatchSize * sizeof(vtxTexd2D) * p.vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);

		// layout (location=0) in vec2 aVertexPosition;
		glEnableVertexArrayAttrib(ret.vao, 0);
		glVertexArrayVertexBuffer(ret.vao, 0, ret.vbo, 0, sizeof(vtxTexd2D));
		glVertexArrayAttribFormat(ret.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(ret.vao, 0, 0);
		// layout (location=1) in vec2 aTextureCoord;
		glEnableVertexArrayAttrib(ret.vao, 1);
		glVertexArrayVertexBuffer(ret.vao, 1, ret.vbo, sizeof(Vec2f), sizeof(vtxTexd2D));
		glVertexArrayAttribFormat(ret.vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(ret.vao, 1, 1);
		// layout (location=2) in float aDepth;
		glEnableVertexArrayAttrib(ret.vao, 2);
		glVertexArrayVertexBuffer(ret.vao, 2, ret.vbo, sizeof(Vec2f) * 2, sizeof(vtxTexd2D));
		glVertexArrayAttribFormat(ret.vao, 2, 1, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(ret.vao, 2, 2);
		// layout (location=3) in unsigned int aID;
		glEnableVertexArrayAttrib(ret.vao, 3);
		glVertexArrayVertexBuffer(ret.vao, 3, ret.vbo, sizeof(Vec2f) * 2 + sizeof(float), sizeof(vtxTexd2D));
		glVertexArrayAttribIFormat(ret.vao, 3, 2, GL_UNSIGNED_INT, 0);
		glVertexArrayAttribBinding(ret.vao, 3, 3);
		

		// Indexing
		glCreateBuffers(1, &ret.ebo);
		glNamedBufferStorage(ret.ebo, sizeof(GLushort) * p.drawCnt, p.idx.data(), GL_MAP_WRITE_BIT);
		glVertexArrayElementBuffer(ret.vao, ret.ebo);

		// Instancing Pt.2
		std::vector<DrawElemCmd> vDrawCmd;
		vDrawCmd.reserve(mBatchSize);
		for (unsigned int i{}; i < mBatchSize; ++i)
		{
			DrawElemCmd dec;
			dec.vtxCnt = p.drawCnt;
			dec.instanceCnt = 1;
			dec.firstIdx = 0;
			dec.baseVtx = i * static_cast<GLuint>(p.vtx.size());
			dec.baseInstance = i;
			vDrawCmd.emplace_back(dec);
		}
		glCreateBuffers(1, &ret.ibo);
		glNamedBufferStorage(ret.ibo, mBatchSize * sizeof(DrawElemCmd), vDrawCmd.data(), GL_MAP_WRITE_BIT);

		AssetManager::GetInstance()->AddAsset(std::to_string(ret.vao), ret);
		mBufferData.push_back(ret);

		glBindVertexArray(0);
	}

	void BaseGraphicsSystem::GenDebugBatch(const BasePrimitive& p)
	{
		BatchBuffer ret{};
		ret.pRef = &p;

		glCreateVertexArrays(1, &ret.vao);
		glCreateBuffers(1, &ret.vbo);
		glNamedBufferStorage(ret.vbo, mBatchSize * sizeof(vtx2D) * p.vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);

		// layout(location=0) in vec2 aVertexPosition;
		glEnableVertexArrayAttrib(ret.vao, 0);
		glVertexArrayVertexBuffer(ret.vao, 0, ret.vbo, 0, sizeof(vtx2D));
		glVertexArrayAttribFormat(ret.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(ret.vao, 0, 0);
		// layout(location=1) in float aDepth;
		glEnableVertexArrayAttrib(ret.vao, 1);
		glVertexArrayVertexBuffer(ret.vao, 1, ret.vbo, sizeof(Vec2f), sizeof(vtx2D));
		glVertexArrayAttribFormat(ret.vao, 1, 1, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(ret.vao, 1, 1);


		// Indexing
		std::vector<GLushort> idxOrder{};
		idxOrder.reserve(p.drawCnt);
		for (GLushort i{}; i < static_cast<GLushort>(p.drawCnt); ++i)
			idxOrder.emplace_back(i);
		glCreateBuffers(1, &ret.ebo);
		glNamedBufferStorage(ret.ebo, sizeof(GLushort) * p.drawCnt, idxOrder.data(), GL_MAP_WRITE_BIT);
		glVertexArrayElementBuffer(ret.vao, ret.ebo);

		// Instancing Pt.2
		std::vector<DrawElemCmd> vDrawCmd;
		vDrawCmd.reserve(mBatchSize);
		for (unsigned int i{}; i < mBatchSize; ++i)
		{
			DrawElemCmd dec;
			dec.vtxCnt = p.drawCnt;
			dec.instanceCnt = 1;
			dec.firstIdx = 0;
			dec.baseVtx = i * static_cast<GLuint>(p.vtx.size());
			dec.baseInstance = i;
			vDrawCmd.emplace_back(dec);
		}
		glCreateBuffers(1, &ret.ibo);
		glNamedBufferStorage(ret.ibo, mBatchSize * sizeof(DrawElemCmd), vDrawCmd.data(), GL_MAP_WRITE_BIT);

		AssetManager::GetInstance()->AddAsset(std::to_string(ret.vao), ret);
		mBufferData.push_back(ret);

		glBindVertexArray(0);
	}

	void BaseGraphicsSystem::EditBatchData(const unsigned int& entity, const unsigned int& pos, bool worldSpace, RENDER_LAYERS layer)
	{
		auto& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
		auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(renderer.model) };
		auto& tryTex = AssetManager::GetInstance()->GetAsset<Texture>(renderer.texture);

		float depth{};
		Mtx3x3f wSpace{};
		if (worldSpace)
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			depth = t.depth;
			wSpace = t.worldSpace;
		}
		else
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<UITransform>(entity);
			depth = t.depth;
			wSpace.translateThis(t.pos.x,t.pos.y).scaleThis(t.scale.x,t.scale.y);
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
			tt.depth = depth;
			tt.uv = tryTex.mtx * p.texCoord[i];
			temp.emplace_back(tt);
		}

		glNamedBufferSubData(mBufferData[0].vbo, sizeof(vtxTexd2D) * p.vtx.size() * pos, sizeof(vtxTexd2D) * p.vtx.size(), temp.data());

	}

	void BaseGraphicsSystem::EditDebugBatchData(const unsigned int& entity, const unsigned int& pos, const BasePrimitive& primitive, Mtx3x3f& mtx, bool worldSpace, RENDER_LAYERS layer)
	{
		float depth{};
		Mtx3x3f wSpace{};
		if (worldSpace)
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			depth = t.depth;
		}
		else
		{
			auto& t = ComponentManager::GetInstance()->GetComponent<UITransform>(entity);
			depth = t.depth;
		}
		depth = CalcDepth(depth, layer);

		std::vector<vtx2D> temp;
		temp.reserve(primitive.drawCnt);
		for (unsigned int i{}; i < primitive.drawCnt; ++i)
		{
			vtx2D tt;
			tt.pos = mtx * primitive.vtx[i];
			tt.depth = depth;
			temp.emplace_back(tt);
		}

		glNamedBufferSubData(mBufferData[0].vbo, sizeof(vtx2D) * primitive.drawCnt * pos, sizeof(vtx2D) * primitive.drawCnt, temp.data());

	}

	void BaseGraphicsSystem::BatchRender()
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, AssetManager::GetInstance()->mArrayTex);

		for (auto& i : mBufferData)
		{
			glBindVertexArray(i.vao);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER,i.ibo);

			glMultiDrawElementsIndirect(GL_TRIANGLES,
				GL_UNSIGNED_SHORT,// Indices
				(GLvoid*)0,
				mBatchSize,
				0);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void BaseGraphicsSystem::BatchDebugRender()
	{
		for (auto& i : mBufferData)
		{
			glBindVertexArray(i.vao);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, i.ibo);

			glMultiDrawElementsIndirect(GL_LINE_LOOP,
				GL_UNSIGNED_SHORT,// Indices
				(GLvoid*)0,
				mBatchSize,
				0);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//void BaseGraphicsSystem::RenderPrimitive(const Primitive& p)
	//{
	//	glBindVertexArray(p.vaoid);
	//
	//	switch (p.drawMode)
	//	{
	//	case GL_LINE_LOOP:
	//		glLineWidth(2.f);
	//		glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
	//		break;
	//	case GL_TRIANGLES:
	//		glDrawElements(GL_TRIANGLES, p.drawCnt, GL_UNSIGNED_SHORT, NULL);
	//		break;
	//	case GL_TRIANGLE_FAN:
	//		glDrawArrays(GL_TRIANGLE_FAN, 0, p.drawCnt);
	//		break;
	//	}
	//}
}
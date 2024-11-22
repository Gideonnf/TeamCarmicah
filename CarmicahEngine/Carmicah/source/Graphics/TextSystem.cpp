/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			TextSystem handles the rendering of text to the screen

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/TextSystem.h"
#include "Systems/GOFactory.h"
#include "Components/UITransform.h"
#include "Components/TextRenderer.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"
#include "Math/Matrix3x3.h"
#include "RenderHelper.h"

namespace Carmicah
{
	void TextSystem::ClearText(const Entity& e, unsigned int startPos, unsigned int endPos)
	{
		// Gets related data
		EntityData& dat = mEntityBufferLoc.find(e)->second;
		int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
		size_t vtxSize = dat.mBufferData->pRef->vtx.size();

		bool killAll{ (startPos | endPos) == 0 };

		if (endPos == 0)
			endPos = dat.mBufferData->buffer.size() * mBatchSize;

		// Flush data with 0
		endPos -= startPos;// eg. erase 112 to 124 -> 12
		for (unsigned int i{ startPos / mBatchSize }; i < dat.mBufferData->buffer.size(); ++i)
		{
			size_t numVtxToChange = vtxSize * std::min(endPos, static_cast<unsigned int>(mBatchSize)) - startPos;
			if (numVtxToChange == 0)
				break;

			if (!dat.mBufferData->isDebug)
			{
				std::vector<vtxTexd2D> temp;
				temp.resize(numVtxToChange);

				glNamedBufferSubData(dat.mBufferData->buffer[i].vbo, startPos, sizeof(vtxTexd2D) * numVtxToChange, temp.data());
			}
			else
			{
				std::vector<vtx2D> temp;
				temp.resize(numVtxToChange);

				glNamedBufferSubData(dat.mBufferData->buffer[i].vbo, startPos, sizeof(vtx2D) * numVtxToChange, temp.data());
			}
			startPos = 0;
			endPos -= mBatchSize;
		}


		if (killAll)// KILL ALL
		{
			ToggleActiveEntity(dat, false);
			//dat.mBufferData->freeData = std::queue<unsigned int>(); // font dosen't use this
			dat.mBufferData->objCount = 0;
			mEntityBufferLoc.erase(e);
			RenderHelper::GetInstance()->UnassignFont(e); // unassign this Buffer ID back to the queue for other fonts to use
		}

	}

	void TextSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<TextRenderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<TextSystem>(mSignature);

		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.fontShader);
	}
	void TextSystem::EntityDestroyed(Entity id)
	{
		/*
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
			ClearText(id);
		*/
	}

	void TextSystem::Render()
	{
		/*
		for (auto& entity : mEntitiesSet)
		{
			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& foundFontTex{ AssetManager::GetInstance()->GetAsset<Font>(txtRenderer.font) };

			// Find first char that is stored differently in the buffer
			if (txtRenderer.oldTxt.compare(txtRenderer.txt) == 0)
				continue;
			size_t firstDiff{ txtRenderer.oldTxt.size() };
			for (size_t i{}; i < txtRenderer.oldTxt.size(); ++i)
			{
				if (txtRenderer.oldTxt.size() != txtRenderer.txt.size())
				{
					firstDiff = i;
					break;
				}
			}

			if (firstDiff != txtRenderer.oldTxt.size() && txtRenderer.txt.size() < txtRenderer.oldTxt.size())
				ClearText(entity, firstDiff, txtRenderer.oldTxt.size() - txtRenderer.txt.size());

			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(txtRenderer.model) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity)};

			size_t charsToWrite = txtRenderer.txt.size() - firstDiff;

			// Pass in uniforms (no scale currently sadge)
			if (RenderHelper::GetInstance()->mFontUniforms.find(entity) == RenderHelper::GetInstance()->mFontUniforms.end())
				RenderHelper::GetInstance()->mFontUniforms.emplace(entity, RenderHelper::FontUniform(txtRenderer.colorR, txtRenderer.colorG, txtRenderer.colorB, UITrans.GetPos()));
			else
				RenderHelper::GetInstance()->mFontUniforms[entity].offset = UITrans.GetPos();

			if (charsToWrite > 0)
			{
				float xTrack{};
				for (size_t txtCount{}; txtCount < firstDiff; ++txtCount)
				{
					Font::FontChar ch = foundFontTex.mFontMaps[txtRenderer.txt[txtCount] - foundFontTex.charOffset];
					xTrack += (ch.advance >> 7) * UITrans.Scale().x; // bitshift by 6 to get value in pixels (2^6 = 64)
				}

				std::vector<vtxTexd2D> charData;
				charData.reserve(p.vtx.size() * txtRenderer.txt.size());
				// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
				for (size_t txtCount{ firstDiff }; txtCount < txtRenderer.txt.size(); ++txtCount)
				{
					Font::FontChar ch = foundFontTex.mFontMaps[txtRenderer.txt[txtCount] - foundFontTex.charOffset];

					xTrack += (ch.advance >> 7) * UITrans.Scale().x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
					Mtx3x3f charTransform{};

					// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
					charTransform
						.translateThis(
							xTrack + static_cast<float>(ch.xBearing) * UITrans.Scale().x * 0.5f,
							-(static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * UITrans.Scale().y * 0.5f)
						.scaleThis(static_cast<float>(ch.width) * UITrans.Scale().x * 0.5f,
							static_cast<float>(ch.height) * UITrans.Scale().y * 0.5f);

					const FontTexture& t{ AssetManager::GetInstance()->GetAsset<FontTexture>(ch.texRef) };

					float depth{ CalcDepth(UITrans.Depth(), UI_LAYER) };

					for (unsigned int i{}; i < p.vtx.size(); ++i)
					{
						vtxTexd2D tt;
						tt.pos = charTransform * p.vtx[i];
						tt.ids[0] = entity;
						tt.ids[1] = t.t;
						tt.depth = depth;
						tt.uv = t.mtx * p.texCoord[i];
						charData.emplace_back(tt);
					}

					// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					xTrack += (ch.advance >> 7) * UITrans.Scale().x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				}
				// render
			}
		}
		*/
	}

}
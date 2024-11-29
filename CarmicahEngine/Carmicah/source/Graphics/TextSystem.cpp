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
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
		{
			if (test->second.mBufferData->isDebug)
				ReplaceTextBuffer(id, std::vector<vtx2D>());
			else
				ReplaceTextBuffer(id, std::vector<vtxTexd2D>());
			mEntityBufferLoc.erase(id);
		}
	}

	void TextSystem::Update()
	{
		for (std::unordered_map<unsigned int, EntityData>::iterator it{ mEntityBufferLoc.begin() };
			it != mEntityBufferLoc.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<TextRenderer>(it->first) || !ComponentManager::GetInstance()->HasComponent<UITransform>(it->first))
			{
				auto test = mEntityBufferLoc.find(it->first);
				if (test != mEntityBufferLoc.end())
				{
					if (test->second.mBufferData->isDebug)
						ReplaceTextBuffer(it->first, std::vector<vtx2D>());
					else
						ReplaceTextBuffer(it->first, std::vector<vtxTexd2D>());
					it = mEntityBufferLoc.erase(it);
					continue;
				}
			}
			++it;
		}
		// Add new Data
		if (mActiveEntityCount != mEntitiesSet.size())
		{
			for (auto& entity : mEntitiesSet)
			{
				// if entity is active -> skip
				if (mEntityBufferLoc.find(entity) != mEntityBufferLoc.end())
					continue;

				std::string& primName = ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity).model;

				unsigned int specificId = RenderHelper::GetInstance()->AssignFont(entity);
				RenderHelper::BufferID bufferID(AssetManager::GetInstance()->GetAsset<BasePrimitive>(primName).uid, mCurrShader, false, specificId);

				if (RenderHelper::GetInstance()->mBufferMap.find(bufferID) == RenderHelper::GetInstance()->mBufferMap.end())
					GenBatch(primName, specificId, false, false);

				EntityData newDat;
				ToggleActiveEntity(true);
				newDat.mBufferData = &RenderHelper::GetInstance()->mBufferMap.find(bufferID)->second;
				mEntityBufferLoc.emplace(entity, newDat);
			}
		}

		for (auto& ebl : mEntityBufferLoc)
		{
			auto& entity = ebl.first;

			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& foundFontTex{ AssetManager::GetInstance()->GetAsset<Font>(txtRenderer.font) };
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(txtRenderer.model) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity) };

			// Pass in uniforms (no scale currently sadge) TODO
			if (RenderHelper::GetInstance()->mFontUniforms.find(entity) == RenderHelper::GetInstance()->mFontUniforms.end())
			{
				CM_CORE_WARN("No Font Uniform linked to this font");
				continue;
			}
			else
			{
				RenderHelper::GetInstance()->mFontUniforms[entity].offset = UITrans.Pos();
				RenderHelper::GetInstance()->mFontUniforms[entity].scale = UITrans.Scale();
				RenderHelper::GetInstance()->mFontUniforms[entity].SetColor(txtRenderer.colorR, txtRenderer.colorG, txtRenderer.colorB);
			}

			// Find first char that is stored differently in the buffer
			if (txtRenderer.oldTxt.compare(txtRenderer.txt) == 0)
				continue;
			size_t smallerLength{ std::min(txtRenderer.oldTxt.size(), txtRenderer.txt.size()) },
				firstDiff{ smallerLength };
			for (size_t i{}; i < smallerLength; ++i)
			{
				if (txtRenderer.oldTxt.at(i) != txtRenderer.txt.at(i))
				{
					firstDiff = i;
					break;
				}
			}

			size_t charsToWrite{ txtRenderer.txt.size() - firstDiff };
			// if (old text changed) && (New text is smaller than old text[need to explicitly delete])
			if (firstDiff != txtRenderer.oldTxt.size() && txtRenderer.txt.size() < txtRenderer.oldTxt.size())
			{
				if (ebl.second.mBufferData->isDebug)
					ReplaceTextBuffer(entity, std::vector<vtx2D>(), firstDiff, txtRenderer.oldTxt.size());
				else
					ReplaceTextBuffer(entity, std::vector<vtxTexd2D>(), firstDiff, txtRenderer.oldTxt.size());
			}

			if (charsToWrite > 0)
			{
				const int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;

				unsigned int fontBufferID = RenderHelper::GetInstance()->mFontUniforms.find(entity)->second.bufferID;

				for (int i = 0;
					i < static_cast<int>(txtRenderer.txt.size()) - (static_cast<int>(ebl.second.mBufferData->buffer.size()) * mBatchSize);
					i += mBatchSize)
					GenBatch(txtRenderer.model, fontBufferID, false, false, true);
				

				// Find the xTrack
				float xTrack{};
				for (size_t txtCount{}; txtCount < firstDiff; ++txtCount)
				{
					Font::FontChar ch = foundFontTex.mFontMaps[txtRenderer.txt[txtCount] - charOffset];
					xTrack += (ch.advance >> 7); // bitshift by 6 to get value in pixels (2^6 = 64)
				}

				// Continue with actual mem alloc
				std::vector<vtxTexd2D> charData;
				charData.reserve(p.vtx.size() * charsToWrite);
				// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
				for (size_t txtCount{ firstDiff }; txtCount < txtRenderer.txt.size(); ++txtCount)
				{
					Font::FontChar ch = foundFontTex.mFontMaps[txtRenderer.txt[txtCount] - charOffset];

					xTrack += (ch.advance >> 7) * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
					Mtx3x3f charTransform{};

					// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
					charTransform
						.translateThis(
							xTrack + static_cast<float>(ch.xBearing) * 0.5f,
							-(static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * 0.5f)
						.scaleThis(static_cast<float>(ch.width) * 0.5f,
							static_cast<float>(ch.height) * 0.5f);

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
					xTrack += (ch.advance >> 7) * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				}
				// Push to Buffer
				ReplaceTextBuffer(entity, charData, firstDiff);
			}
		
			txtRenderer.oldTxt = txtRenderer.txt;
		}
	}

}
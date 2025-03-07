/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextGraphicsSystem.cpp

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
#include "Graphics/TextGraphicsSystem.h"
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
	void TextGraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<TextRenderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<TextGraphicsSystem>(mSignature);

		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);

		BaseGraphicsSystem::GenBatch("Square", 1, false, false);
	}

	void TextGraphicsSystem::EntityDestroyed(Entity id)
	{
		RenderHelper::GetInstance()->UnassignFont(id);
		mCpyEntitySet.erase(id);
	}
	
	void TextGraphicsSystem::Update()
	{
		// Remove data
		for (std::set<unsigned int>::iterator entity = mCpyEntitySet.begin(); entity != mCpyEntitySet.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<TextRenderer>(*entity) || !ComponentManager::GetInstance()->HasComponent<UITransform>(*entity))
			{
				RenderHelper::GetInstance()->UnassignFont(*entity);
				entity = mCpyEntitySet.erase(entity);
				continue;
			}
			++entity;
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& font{ AssetManager::GetInstance()->GetAsset<Font>(txtRenderer.font) };
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(model) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity) };

			// Find first char that is stored differently in the buffer
			if (txtRenderer.oldTxt.compare(txtRenderer.txt) == 0 && txtRenderer.colorR && !UITrans.Updated())
				continue;
			// Get Buffer
			auto buff = RenderHelper::GetInstance()->mFontData.find(entity);
			// Pass in uniforms
			if (buff == RenderHelper::GetInstance()->mFontData.end())
			{
				RenderHelper::GetInstance()->AssignFont(entity);
				mCpyEntitySet.insert(entity);
				buff = RenderHelper::GetInstance()->mFontData.find(entity);
			}
			
			// Store Font Uniforms
			buff->second.offset = UITrans.ExtractWorldPos();
			buff->second.scale = UITrans.Scale();
			buff->second.SetColor(txtRenderer.colorR, txtRenderer.colorG, txtRenderer.colorB);
			float depth{ CalcDepth(UITrans.Depth(), UI_LAYER) };

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
				buff->second.vtxSize = static_cast<int>(txtRenderer.txt.size());
			}

			if (charsToWrite > 0)
			{
				// Find the xTrack
				float xTrack{};
				for (size_t txtCount{}; txtCount < firstDiff; ++txtCount)
				{
					Font::FontChar ch = font.mFontMaps[txtRenderer.txt[txtCount] - charOffset];
					xTrack += (ch.advance >> 7); // bitshift by 6 to get value in pixels (2^6 = 64)
				}

				// Continue with actual mem alloc
				RenderHelper::GetInstance()->ReserveFontBuffer(buff->second, charsToWrite);
				// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
				for (size_t txtCount{ firstDiff }; txtCount < txtRenderer.txt.size(); ++txtCount)
				{
					Font::FontChar ch = font.mFontMaps[txtRenderer.txt[txtCount] - charOffset];

					xTrack += (ch.advance >> 7) * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
					Mtx3x3f charTransform{};

					// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
					charTransform
						.translateThis(
							xTrack + static_cast<float>(ch.xBearing) * 0.5f,
							-(static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * 0.5f)
						.scaleThis(static_cast<float>(ch.width) * 0.5f,
							static_cast<float>(ch.height) * 0.5f);

					txtRenderer.totalHeight = std::max(txtRenderer.totalHeight, charTransform.m[7]);

					const FontTexture& t{ AssetManager::GetInstance()->GetAsset<FontTexture>(ch.texRef) };

					for (unsigned int i{}; i < p.vtx.size(); ++i)
					{
						vtxTexd2D& tt = buff->second.vtx[txtCount * p.vtx.size() + i];
						tt.pos = charTransform * p.vtx[i];
						tt.ids[0] = entity;
						tt.ids[1] = t.t;
						tt.depth = depth;
						tt.uv = t.mtx * p.texCoord[i];
					}

					// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
					xTrack += (ch.advance >> 7) * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				}
				txtRenderer.totalWidth = xTrack;
				buff->second.vtxSize = static_cast<int>(txtRenderer.txt.size());
			}

			buff->second.offset.y -= (txtRenderer.totalHeight * 0.5f * buff->second.scale.y) * (txtRenderer.txtAlign & TextRenderer::TXTALIGN::TXT_CHECKY);
			buff->second.offset.x -= (txtRenderer.totalWidth * 0.5f * buff->second.scale.x) * ((txtRenderer.txtAlign & TextRenderer::TXTALIGN::TXT_CHECKX) >> 4);

			for (unsigned int i{}; i < std::min(txtRenderer.oldTxt.size(), txtRenderer.txt.size()) * p.vtx.size(); ++i)
			{
				buff->second.vtx[i].depth = depth;
			}

			txtRenderer.oldTxt = txtRenderer.txt;
		}
	}

}
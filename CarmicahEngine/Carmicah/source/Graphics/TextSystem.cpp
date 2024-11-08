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

namespace Carmicah
{
	void TextSystem::Init(const float& canvasWidth, const float& canvasHeight)
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<TextRenderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<TextSystem>(mSignature);

		BaseGraphicsSystem::Init("font");
		screenMtx.translateThis(-1.f, -1.f).scaleThis(2 / canvasWidth, 2 / canvasHeight);

		GenBatch(AssetManager::GetInstance()->GetAsset<Primitive>("Square"));
	}

	void TextSystem::Render()
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLint uniformLoc{};
		if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, screenMtx.m);

		for (auto& entity : mEntitiesSet)
		{
			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity)};
			auto& foundFontTex{ AssetManager::GetInstance()->GetAsset<Font>(txtRenderer.font) };
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(txtRenderer.model) };

			float xTrack = UITrans.pos.x, yTrack = UITrans.pos.y;

			if (UniformExists(mCurrShader, "uTextColor", uniformLoc))
				glUniform3f(uniformLoc, txtRenderer.colorR, txtRenderer.colorG, txtRenderer.colorB);

			// Clear Buffer
			glClearNamedBufferSubData(mBufferData[0].vbo, GL_RGBA8, 0, sizeof(vtxTexd2D) * 4 * mLastTextNumCount, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			mLastTextNumCount = 0;

			std::vector<vtxTexd2D> charData;
			charData.reserve(p.vtx.size() * txtRenderer.txt.size());
			// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
			for (auto& c : txtRenderer.txt)
			{
				Font::FontChar ch = foundFontTex.mFontMaps[c - foundFontTex.charOffset];

				xTrack += (ch.advance >> 7) * UITrans.scale.x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				Mtx3x3f charTransform{};

				// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
				charTransform
					.translateThis(
					xTrack + static_cast<float>(ch.xBearing) * UITrans.scale.x * 0.5f,
					yTrack - (static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * UITrans.scale.y * 0.5f)
					.scaleThis(static_cast<float>(ch.width) * UITrans.scale.x * 0.5f,
						static_cast<float>(ch.height) * UITrans.scale.y * 0.5f);

				const FontTexture& t{ AssetManager::GetInstance()->GetAsset<FontTexture>(ch.texRef) };

				float depth{ CalcDepth(UITrans.depth, UI_LAYER) };

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

				++mLastTextNumCount;

				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				xTrack += (ch.advance >> 7) * UITrans.scale.x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glNamedBufferSubData(mBufferData[0].vbo, 0, sizeof(vtxTexd2D) * p.vtx.size() * mLastTextNumCount, charData.data());
			BatchRender();
		}

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
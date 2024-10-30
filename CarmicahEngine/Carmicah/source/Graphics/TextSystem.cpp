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
	void TextSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<TextRenderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<TextSystem>(mSignature);

		auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.fontShader);
		mCurrShader = shdrRef.s;
	}

	void TextSystem::Render(GLuint canvasWidth, GLuint canvasHeight)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Mtx3x3f projection{};
		projection.translateThis(-1.f, -1.f).scaleThis(2.f / static_cast<float>(canvasWidth), 2.f / static_cast<float>(canvasHeight));

		for (auto& entity : mEntitiesSet)
		{
			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity)};
			auto& foundFontTex{ AssetManager::GetInstance()->GetAsset<Font>(txtRenderer.font) };
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(txtRenderer.model) };

			float xTrack = UITrans.pos.x, yTrack = UITrans.pos.y;

			GLint uniformLoc;
			if (uniformExists(mCurrShader, "uTextColor", uniformLoc))
				glUniform3f(uniformLoc, txtRenderer.colorR, txtRenderer.colorG, txtRenderer.colorB);

			// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
			for (auto& c : txtRenderer.txt)
			{
				Font::FontChar ch = foundFontTex.mFontMaps[c];
				//FontChar ch = foundFontTex-second[c];

				xTrack += (ch.advance >> 7) * UITrans.scale.x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				Mtx3x3f charTransform{};

				// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
				charTransform
					.translateThis(
					xTrack + static_cast<float>(ch.xBearing) * UITrans.scale.x * 0.5f,
					yTrack - (static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * UITrans.scale.y * 0.5f)
					.scaleThis(static_cast<float>(ch.width) * UITrans.scale.x * 0.5f,
						static_cast<float>(ch.height) * UITrans.scale.y * 0.5f);

				charTransform = projection * charTransform;

				if (uniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
					glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, charTransform.m);

				glBindVertexArray(p.vaoid);
				glBindTextureUnit(0, ch.texID);
				switch (p.drawMode)
				{
				case GL_LINE_LOOP:
					glLineWidth(2.f);
					glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
					break;
				case GL_TRIANGLES:
					glDrawElements(GL_TRIANGLES, p.drawCnt, GL_UNSIGNED_SHORT, NULL);
					break;
				case GL_TRIANGLE_FAN:
					glDrawArrays(GL_TRIANGLE_FAN, 0, p.drawCnt);
					break;
				}

				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				xTrack += (ch.advance >> 7) * UITrans.scale.x * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
			}
		}
		
		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Graphics/TextSystem.h"
#include "Systems/GOFactory.h"
#include "Components/UITransform.h"
#include "Components/TextRenderer.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"

namespace Carmicah
{
	void TextSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<TextRenderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<TextSystem>(mSignature);

		auto shdrRef = AssetManager::GetInstance()->mShaderPgms.find(shaderName);
		if (shdrRef != AssetManager::GetInstance()->mShaderPgms.end())
			mCurrShader = shdrRef->second;
		else
			CM_CORE_ERROR("TextSystem failed to load Shader");
	}

	void TextSystem::Render(GLuint canvasWidth, GLuint canvasHeight)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat3 projection{ 1 };
		projection = glm::translate(projection, glm::vec2(-1.f, -1.f));
		projection = glm::scale(projection, glm::vec2(2.f / static_cast<float>(canvasWidth), 2.f / static_cast<float>(canvasHeight)));

		for (auto& entity : mEntitiesSet)
		{
			auto& txtRenderer{ ComponentManager::GetInstance()->GetComponent<TextRenderer>(entity) };
			auto& UITrans{ ComponentManager::GetInstance()->GetComponent<UITransform>(entity)};
			auto foundFontTex{ AssetManager::GetInstance()->mFontMaps.find(txtRenderer.font) };
			if (foundFontTex == AssetManager::GetInstance()->mFontMaps.end())
			{
				std::stringstream ss;
				ss << "Font not found: " << txtRenderer.font << std::endl;
				CM_CORE_ERROR(ss.str());
				continue;
			}
			auto tryPrimitive{ AssetManager::GetInstance()->mPrimitiveMaps.find(txtRenderer.model) };
			Primitive* p;
			if (tryPrimitive == AssetManager::GetInstance()->mPrimitiveMaps.end())
			{
				std::stringstream ss;
				ss << "Renderer Model not found: " << txtRenderer.model << std::endl;
				CM_CORE_ERROR(ss.str());
				continue;
			}
			else
				p = &tryPrimitive->second;

			float xTrack = UITrans.xPos, yTrack = UITrans.yPos;


			GLint uniformLoc;
			if (uniformExists(mCurrShader, "uTextColor", uniformLoc))
				glUniform3f(uniformLoc, txtRenderer.color.x, txtRenderer.color.y, txtRenderer.color.z);

			// iterate through all characters (alot of it divides by 2 since quad is based on [-1,1])
			for (auto& c : txtRenderer.txt)
			{
				FontChar ch = foundFontTex->second[c];

				xTrack += (ch.advance >> 7) * UITrans.xScale * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
				glm::mat3 charTransform{ 1 };

				// scale is multiplied by 0.5f since the mesh is from -0.5 to 0.5
				charTransform = glm::translate(charTransform, glm::vec2(
					xTrack + static_cast<float>(ch.xBearing) * UITrans.xScale * 0.5f,
					yTrack - (static_cast<float>(ch.height >> 1) - static_cast<float>(ch.yBearing)) * UITrans.yScale * 0.5f));

				charTransform = glm::scale(charTransform, glm::vec2(
					static_cast<float>(ch.width) * UITrans.xScale * 0.5f,
					static_cast<float>(ch.height) * UITrans.yScale * 0.5f));

				charTransform = projection * charTransform;

				if (uniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
					glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(charTransform));

				glBindVertexArray(p->vaoid);
				glBindTextureUnit(0, ch.texID);
				switch (p->drawMode)
				{
				case GL_LINE_LOOP:
					glLineWidth(2.f);
					glDrawArrays(GL_LINE_LOOP, 0, p->drawCnt);
					break;
				case GL_TRIANGLES:
					glDrawElements(GL_TRIANGLES, p->drawCnt, GL_UNSIGNED_SHORT, NULL);
					break;
				case GL_TRIANGLE_FAN:
					glDrawArrays(GL_TRIANGLE_FAN, 0, p->drawCnt);
					break;
				}

				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				xTrack += (ch.advance >> 7) * UITrans.xScale * 0.5f; // bitshift by 6 to get value in pixels (2^6 = 64)
			}
		}
		
		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
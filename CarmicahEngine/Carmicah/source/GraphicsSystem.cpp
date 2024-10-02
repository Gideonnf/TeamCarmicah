/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Graphics System handles the rendering of textured meshes onto the screen (using transform, camera reference, and texture matrix)

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
#include "Graphics/GraphicsSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace Carmicah
{
	bool GraphicsSystem::uniformExists(const char* str, GLint& ref)
	{
		ref = glGetUniformLocation(mCurrShader, str);
		if (ref >= 0)
			return true;

		std::cerr << "Uniform variable: " << str << " dosen't exist!!!\n";
		std::exit(EXIT_FAILURE);
		return false;

	}
	
	void GraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);

		auto& shdrRef = AssetManager::GetInstance()->mShaderPgms.find(AssetManager::GetInstance()->enConfig.defaultShader);
		if (shdrRef != AssetManager::GetInstance()->mShaderPgms.end())
			mCurrShader = shdrRef->second;
	}

	void GraphicsSystem::SetScreenSize(GLuint camWidth, GLuint camHeight, Entity& cam)
	{
		auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		currCam.notUpdated = false;
		currCam.xScale = 1.f / static_cast<float>(camWidth);
		currCam.yScale = 1.f / static_cast<float>(camHeight);
	}

	void GraphicsSystem::Render(Entity& cam)
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(mCurrShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Handle Camera Transform
		auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		if (!currCam.notUpdated)
		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			currCam.camSpace = glm::mat3(1);
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			currCam.camSpace = glm::scale(currCam.camSpace, glm::vec2{ camTrans.xScale, camTrans.yScale });
			currCam.camSpace = glm::rotate(currCam.camSpace, glm::radians(-camTrans.rot));
			currCam.camSpace = glm::translate(currCam.camSpace, glm::vec2{ camTrans.xPos, camTrans.yPos });
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			Renderer& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
			auto& tryPrimitive{ AssetManager::GetInstance()->mPrimitiveMaps.find(renderer.model) };
			Primitive* p;
			if (tryPrimitive == AssetManager::GetInstance()->mPrimitiveMaps.end())
			{
				std::cerr << "Renderer Model not found: " << renderer.model << std::endl;
				p = &AssetManager::GetInstance()->mPrimitiveMaps.begin()->second;
			}
			else
				p = &tryPrimitive->second;

			// Handle Entities transform
			if (!transform.notUpdated)
			{
				transform.worldSpace = glm::mat3(1.f);
				transform.worldSpace = glm::translate(transform.worldSpace, glm::vec2{ transform.xPos, transform.yPos});
				transform.worldSpace = glm::rotate(transform.worldSpace, glm::radians(transform.rot));
				transform.worldSpace = glm::scale(transform.worldSpace, glm::vec2{ transform.xScale, transform.yScale});
				transform.camSpace = currCam.camSpace * transform.worldSpace;
			}
			else if (!currCam.notUpdated)
				transform.camSpace = currCam.camSpace * transform.worldSpace;

			GLint uniformLoc{};
			if (uniformExists("uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(transform.camSpace));

			if (uniformExists("uTex2d", uniformLoc))
				glUniform1i(uniformLoc, 0);

			if (uniformExists("uAnimationMult", uniformLoc))
			{
				if (renderer.texureMat == glm::mat3(0))
				{
					std::cerr << "Renderer Texture Matrix empty" << std::endl;
					glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat3(1)));
				}
				else
					glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(renderer.texureMat));
			}

			glBindVertexArray(p->vaoid);

			// Error Checking if texture no exists
			auto& tryTex = AssetManager::GetInstance()->mTextureMaps.find(renderer.texture);
			if (tryTex == AssetManager::GetInstance()->mTextureMaps.end())
			{
				std::cerr << "Texture not found" << renderer.texture << std::endl;
				glBindTextureUnit(0, AssetManager::GetInstance()->mTextureMaps.begin()->second.t);
			}
			else
				glBindTextureUnit(0, tryTex->second.t);

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
		}

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
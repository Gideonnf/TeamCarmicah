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
#include "Graphics/GraphicsSystem.h"

#include <ECS/ECSTypes.h>
#include "Systems/GOFactory.h"

#include "Components/Transform.h"
#include "Components/Renderer.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

#include "Systems/AssetManager.h"
#include "log.h"

namespace Carmicah
{
	void GraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);
		auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.defaultShader);
		mCurrShader = shdrRef.s;
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
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx33Identity(currCam.camSpace);
			currCam.camSpace.scaleThis(camTrans.xScale, camTrans.yScale).rotDegThis(-camTrans.rot).translateThis(-camTrans.xPos, -camTrans.yPos);
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			// Handle Entities transform
			if (!transform.notUpdated)
			{
				Mtx33Identity(transform.worldSpace);
				transform.worldSpace.translateThis(transform.xPos, transform.yPos).rotDegThis(transform.rot).scaleThis(transform.xScale, transform.yScale);
				transform.camSpace = currCam.camSpace * transform.worldSpace;
			}
			else if (!currCam.notUpdated)
				transform.camSpace = currCam.camSpace * transform.worldSpace;

			if (mCurrShader == 0)
				continue;

			// Get Components
			Renderer& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(renderer.model) };

			// Set Uniforms
			GLint uniformLoc{};
			if (uniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, transform.camSpace.m);

			//if (uniformExists(mCurrShader, "uTex2d", uniformLoc)) // Only if multiple textures
			//	glUniform1i(uniformLoc, 0);

			if (uniformExists(mCurrShader, "uAnimationMult", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, renderer.textureMat.m);

			glBindVertexArray(p.vaoid);

			// Error Checking if texture no exists
			auto& tryTex = AssetManager::GetInstance()->GetAsset<Texture>(renderer.texture);
			glBindTextureUnit(0, tryTex.t);

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
		}

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
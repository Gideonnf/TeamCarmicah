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
		currCam.scale.x = 1.f / static_cast<float>(camWidth);
		currCam.scale.y = 1.f / static_cast<float>(camHeight);
	}

	void GraphicsSystem::Render(Entity& cam)
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mCurrShader);
		// Just did discard instead, cuz this stopped working
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Needs RBO to depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Handle Camera Transform
		auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		if (!currCam.notUpdated)
		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx33Identity(currCam.camSpace);
			currCam.camSpace.scaleThis(camTrans.scale.x, camTrans.scale.y).rotDegThis(-camTrans.rot).translateThis(-camTrans.pos.x, -camTrans.pos.y);
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			// Handle Entities transform
			if (!transform.notUpdated)
			{
				//Mtx33Identity(transform.worldSpace);
				//transform.worldSpace.translateThis(transform.pos.x, transform.pos.y).rotDegThis(transform.rot).scaleThis(transform.scale.x, transform.scale.y);
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
			if (UniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, transform.camSpace.m);

			//if (UniformExists(mCurrShader, "uTex2d", uniformLoc)) // Only if multiple textures
			//	glUniform1i(uniformLoc, 0);

			if (UniformExists(mCurrShader, "uDepth", uniformLoc))
				glUniform1f(uniformLoc, CalcDepth(transform.depth, RENDER_LAYERS::BASE_LAYER));

			if (UniformExists(mCurrShader, "uID", uniformLoc))
				glUniform1ui(uniformLoc, entity);

			// Error Checking if texture no exists
			auto& tryTex = AssetManager::GetInstance()->GetAsset<Texture>(renderer.texture);

			if (UniformExists(mCurrShader, "uAnimationMult", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, tryTex.mtx.m);
			glBindTextureUnit(0, tryTex.t);

			RenderPrimitive(p);
		}

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
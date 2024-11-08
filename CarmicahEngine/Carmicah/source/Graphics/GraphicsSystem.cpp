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
		BaseGraphicsSystem::Init("");

		GenBatch(AssetManager::GetInstance()->GetAsset<Primitive>("Square"));
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

		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		// Just did discard instead, cuz this stopped working
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Needs RBO to depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Handle Camera Transform
		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx3x3f camSpace{};
			camSpace.scaleThis(camTrans.scale.x, camTrans.scale.y).rotDegThis(-camTrans.rot).translateThis(-camTrans.pos.x, -camTrans.pos.y);
			GLint uniformLoc{};
			if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
		}

		if (mEntitiesSet.size() < mEntityBufferLoc.size())
		{
			for (auto& entity : mEntityBufferLoc)
			{
				auto e{ mEntitiesSet.find(entity.first) };
				if (e != mEntitiesSet.end())
					continue;
				entity.second.isActive = false;
				std::vector<vtx2D> temp;
				temp.resize(6);

				glNamedBufferSubData(mBufferData[0].vbo, sizeof(vtx2D) *6 * entity.second.posInMemory, sizeof(vtx2D) * 6, temp.data());
				mEntityBufferLoc.erase(entity.first);
				break;
			}
		}

		for (auto& entity : mEntityBufferLoc)
		{
			if (!entity.second.isActive)
				continue;

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity.first);

			if (transform.notUpdated)
				continue;

			EditBatchData(entity.first, entity.second.posInMemory, true, BASE_LAYER);
		}
		
		// Add new Data
		if (mEntityBufferLoc.size() != mEntitiesSet.size())
		{
			for (auto& entity : mEntitiesSet)
			{
				auto e{ mEntityBufferLoc.find(entity) };
				if (e != mEntityBufferLoc.end())
					continue;
				EntityData ed{};
				ed.isActive = true;
				ed.posInMemory = mEntityBufferIDTrack++;

				EditBatchData(entity, ed.posInMemory, true, BASE_LAYER);
				mEntityBufferLoc.emplace(entity, ed);
			}
		}


		BatchRender();

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
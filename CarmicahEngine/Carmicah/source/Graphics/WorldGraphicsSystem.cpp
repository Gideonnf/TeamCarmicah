/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			WorldGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Graphics System handles the rendering of textured meshes onto the screen (using transform, camera reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/WorldGraphicsSystem.h"

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
	void WorldGraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<WorldGraphicsSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);
	}

	void WorldGraphicsSystem::SetScreenSize(GLuint camWidth, GLuint camHeight, Entity& cam)
	{
		auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		currCam.Scale(1.f / static_cast<float>(camWidth), 1.f / static_cast<float>(camHeight));
	}

	void WorldGraphicsSystem::EntityDestroyed(Entity id)
	{
		auto test =  mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
		{
			DeleteBatchData(id);
			mEntityBufferLoc.erase(id);
		}
	}

	void WorldGraphicsSystem::Update()
	{
		for (std::unordered_map<unsigned int, EntityData>::iterator entity = mEntityBufferLoc.begin(); entity != mEntityBufferLoc.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<Renderer>(entity->first) || !ComponentManager::GetInstance()->HasComponent<Transform>(entity->first))
			{
				DeleteBatchData(entity->first);
				entity = mEntityBufferLoc.erase(entity);
				continue;
			}

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity->first);

			if (!transform.Updated() && !ComponentManager::GetInstance()->GetComponent<Renderer>(entity->first).Updated())
			{
				++entity;
				continue;
			}

			EditBatchData(entity->first, true, BASE_LAYER);
			++entity;
		}
		
		// Add new Data
		if (mActiveEntityCount != mEntitiesSet.size())
		{
			for (auto& entity : mEntitiesSet)
			{
				// if entity is active -> skip
				if (mEntityBufferLoc.find(entity) != mEntityBufferLoc.end())
					continue;

				SetNewEntity(entity, ComponentManager::GetInstance()->GetComponent<Renderer>(entity).model, 0, true, false);
				EditBatchData(entity, true, BASE_LAYER);
			}
		}
	}

}
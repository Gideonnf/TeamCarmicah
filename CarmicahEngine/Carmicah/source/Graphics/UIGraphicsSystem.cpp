/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			UIGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			UI Graphics System handles the rendering of textured meshes onto the screen (using transform, screen reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/UIGraphicsSystem.h"

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
	void UIGraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<UIGraphicsSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);
	}

	void UIGraphicsSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
			DeleteBatchData(id);
	}



	void UIGraphicsSystem::Update()
	{

		for (std::unordered_map<unsigned int, EntityData>::iterator entity = mEntityBufferLoc.begin(); entity != mEntityBufferLoc.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<Renderer>(entity->first))
			{
				DeleteBatchData(entity->first);
				entity = mEntityBufferLoc.erase(entity);
				continue;
			}

			auto& transform = ComponentManager::GetInstance()->GetComponent<UITransform>(entity->first);

			if (!transform.Updated() && !ComponentManager::GetInstance()->GetComponent<Renderer>(entity->first).Updated())
			{
				++entity;
				continue;
			}

			EditBatchData(entity->first, false, UI_LAYER);
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

				SetNewEntity(entity, ComponentManager::GetInstance()->GetComponent<Renderer>(entity).model, 0, false, false);
				EditBatchData(entity, false, UI_LAYER);
			}
		}
	}

}
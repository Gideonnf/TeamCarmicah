/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ColliderRenderSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles rendering the collision boxes of gameobjects with the collider2D component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/ColliderRenderSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"

namespace Carmicah
{
	void ColliderRenderSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ColliderRenderSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.debugShader);

		primitive = "DebugSquare";
	}

	void ColliderRenderSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
		{
			DeleteBatchData(id);
			mEntityBufferLoc.erase(id);
		}
	}



	void ColliderRenderSystem::Update()
	{
		for (std::unordered_map<unsigned int, EntityData>::iterator entity = mEntityBufferLoc.begin(); entity != mEntityBufferLoc.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<Collider2D>(entity->first) || !ComponentManager::GetInstance()->HasComponent<Transform>(entity->first))
			{
				DeleteBatchData(entity->first);
				entity = mEntityBufferLoc.erase(entity);
				continue;
			}

			if (ComponentManager::GetInstance()->HasComponent<Transform>(entity->first))
			{
				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity->first);

				// --TODO-- needs collider to also do a update check
				//if (!transform.Updated())
				//{
				//	++entity;
				//	continue;
				//}

				auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity->first);
				Mtx3x3f trans{};
				trans.translateThis((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f)
					.rotDegThis(transform.Rot())
					.scaleThis(collider.max.x - collider.min.x, collider.max.y - collider.min.y);

				EditDebugBatchData(entity->first, trans, true, DEBUG_LAYER);
			}
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

				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
				auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
				Mtx3x3f trans{};
				trans.translateThis((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f)
					.rotDegThis(transform.GetRot())
					.scaleThis(collider.max.x - collider.min.x, collider.max.y - collider.min.y);

				SetNewEntity(entity, primitive, 0, true, true);
				EditDebugBatchData(entity, trans, true, DEBUG_LAYER);
			}
		}
	}
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RigidbodyRendererSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Rigidbody Render System handles rendering the rigidbody parts such as velocity of gameobjects with the rigidbody component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <limits>
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/RigidbodyRendererSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"


namespace Carmicah
{
	void RigidbodyRendererSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<RigidbodyRendererSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.debugShader);

		primitive = "DebugLine";
	}

	void RigidbodyRendererSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
		{
			DeleteBatchData(id);
			mEntityBufferLoc.erase(id);
		}
	}



	void RigidbodyRendererSystem::Update()
	{
		for (std::unordered_map<unsigned int, EntityData>::iterator entity = mEntityBufferLoc.begin(); entity != mEntityBufferLoc.end();)
		{
			if (!ComponentManager::GetInstance()->HasComponent<RigidBody>(entity->first) || !ComponentManager::GetInstance()->HasComponent<Transform>(entity->first))
			{
				DeleteBatchData(entity->first);
				entity = mEntityBufferLoc.erase(entity);
				continue;
			}

			auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity->first);
			if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
				fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
			{
				DeleteBatchData(entity->first);
				entity = mEntityBufferLoc.erase(entity);
				continue;
			}

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity->first);

			if (!transform.Updated())
			{
				++entity;
				continue;
			}

			// Get rotation
			float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
			// Get scale multi
			//float biggerVel = fmaxf(fabs(rigidbody.velocity.x), fabs(rigidbody.velocity.y));
			//trans.translateThis(transform.pos.x, transform.pos.y).scaleThis(biggerVel, biggerVel).rotRadThis(rot);

			Mtx3x3f trans{};
			trans.translateThis(transform.Pos()).rotRadThis(rot);

			EditDebugBatchData(entity->first, trans, true, DEBUG_LAYER);
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

				auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity);
				if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
					fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
					continue;

				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
				// Get rotation
				float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
				Mtx3x3f trans{};
				trans.translateThis(transform.Pos()).rotRadThis(rot);

				SetNewEntity(entity, primitive, 0, true, true);
				EditDebugBatchData(entity, trans, true, DEBUG_LAYER);
			}
		}
	}
}
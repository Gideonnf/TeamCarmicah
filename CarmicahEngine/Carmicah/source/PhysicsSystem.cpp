/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        PhysicsSystem.cpp

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Functions definitions for the Collision system. Handles the collision response and the collision checking between objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include "Systems/PhysicsSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Collider2D.h"
#include "Systems/GOFactory.h"
#include "Systems/CollisionSystem.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"
#include "log.h"

namespace Carmicah
{

	void PhysicsSystem::UpdatePosition(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		rigidbody.posPrev.x = transform.xPos;
		rigidbody.posPrev.y = transform.yPos;

		rigidbody.zposPrev = transform.zPos;
	}

	void PhysicsSystem::ApplyForce(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		float deltaTime = CarmicahTimer::GetDt();

		if (rigidbody.objectType == "Dynamic")
		{

			rigidbody.velocity.y += rigidbody.gravity * deltaTime;

			transform.xPos += rigidbody.velocity.x * deltaTime;
			transform.yPos += rigidbody.velocity.y * deltaTime;

			if (rigidbody.velocity.x > 0) 
			{
				transform.rot += 50.0f * deltaTime;

				if (transform.rot > 360.0f)
				{
					transform.rot -= 360.0f;
				}
			}
			else {
				
				transform.rot -= 50.0f * deltaTime;
				if (transform.rot < -360.0f) 
				{
					transform.rot += 360.0f;
				}

			}
		}
		else if (rigidbody.objectType == "Kinematic")
		{
			if (rigidbody.velocity.x != 0)
			{
				transform.xPos += rigidbody.velocity.x * deltaTime;
			}

			if (rigidbody.velocity.y != 0)
			{
				transform.yPos += rigidbody.velocity.y * deltaTime;
			}

		}
	}

	void PhysicsSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<PhysicsSystem>(mSignature);



	}

	void PhysicsSystem::Update()
	{


		for (auto entity : mEntitiesSet)
		{
			ApplyForce(entity);
			UpdatePosition(entity);

		}
	}

	void PhysicsSystem::Exit()
	{

	}
}

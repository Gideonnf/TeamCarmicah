/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        PhysicsSystem.cpp

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Functions definitions for the Physics system. Handles the physics of all the game objects

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
	/**
	 * @brief Updates the previous position (both x and y) of an object based on its current transform.
	 *
	 * The function fetches the RigidBody and Transform components of the object.
	 * It saves the current position into the previous position variables in the RigidBody component,
	 * so that it can be used for physics calculations or other logic requiring the previous position.
	 *
	 * @param obj The entity for which the previous position is updated.
	 */
	void PhysicsSystem::UpdatePosition(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		rigidbody.posPrev.x = transform.xPos;
		rigidbody.posPrev.y = transform.yPos;

		rigidbody.zposPrev = transform.zPos;
	}

	/**
	 * @brief Applies force to a dynamic object, updating its velocity and position based on gravity and its velocity.
	 *
	 * This function applies the force of gravity to the object's velocity and updates the position of the object
	 * based on its velocity and the elapsed time (deltaTime). For dynamic objects, it also handles the rotation update
	 * based on the velocity. If the object is kinematic, it simply moves based on the current velocity.
	 *
	 * @param obj The entity on which the force is applied.
	 */
	void PhysicsSystem::ApplyForce(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		float deltaTime = (float)CarmicahTimer::GetDt();

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
	/**
	 * @brief Initializes the PhysicsSystem by setting its signature and registering it with the SystemManager.
	 *
	 * The signature of the PhysicsSystem is set to include the RigidBody and Transform components.
	 * The system is then registered with the SystemManager, which manages its lifecycle.
	 */
	void PhysicsSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<PhysicsSystem>(mSignature);



	}

	/**
	 * @brief Updates the physics of all entities tracked by the system.
	 *
	 * For each entity in the system, it applies forces (like gravity) and updates their positions.
	 * This function should be called once per frame to ensure all entities' physics are updated.
	 */
	void PhysicsSystem::Update()
	{


		for (auto entity : mEntitiesSet)
		{
			ApplyForce(entity);
			UpdatePosition(entity);

		}
	}

	/**
	 * @brief Cleans up the PhysicsSystem when it is no longer needed.
	 *
	 * This function is called when the system is shut down. Any necessary cleanup should be handled here.
	 * Currently, it does not perform any cleanup but serves as a placeholder for future needs.
	 */
	void PhysicsSystem::Exit()
	{

	}
}
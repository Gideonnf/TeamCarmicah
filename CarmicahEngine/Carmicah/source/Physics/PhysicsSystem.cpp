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
#include "PhysicsSystem.h"
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

		rigidbody.posPrev.x = transform.Pos().x;
		rigidbody.posPrev.y = transform.Pos().y;

		rigidbody.zposPrev = transform.Depth();

	}

	void PhysicsSystem::StopObject(Entity& obj)
	{
		
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);

		rigidbody.velocity = { 0.0f, 0.0f };

		rigidbody.forcesManager.SetCurrentVelocity(rigidbody.velocity);
		rigidbody.forcesManager.SetSumForces({ 0.0f, 0.0f });

		float deltaTime = (float)CarmicahTime::GetInstance()->GetDeltaTime();

		rigidbody.forcesManager.UpdateForces(deltaTime);
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
	void PhysicsSystem::Integrate(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		//float deltaTime = (float)CarmicahTimer::GetDt();

		float deltaTime = (float)CarmicahTime::GetInstance()->GetDeltaTime();

		rigidbody.forcesManager.SetCurrentVelocity(rigidbody.velocity);


		rigidbody.forcesManager.UpdateForces(deltaTime);


		Vector2D<float> sumForces = rigidbody.forcesManager.GetSumForces();
		//if (rigidbody.objectType == "")
		//{
		//	CM_CORE_ERROR("Rigid body has no type");
		//}

		if (rigidbody.objectType == rbTypes::DYNAMIC|| rigidbody.objectType == rbTypes::KINEMATIC)
		{
			transform.PosXAdd(rigidbody.velocity.x * deltaTime);
			transform.PosYAdd(rigidbody.velocity.y * deltaTime);


			rigidbody.acceleration.x = sumForces.x * (1/rigidbody.mass) + rigidbody.gravity;
			rigidbody.acceleration.y = sumForces.y * (1 / rigidbody.mass) + rigidbody.gravity;

			rigidbody.velocity.x = rigidbody.velocity.x + rigidbody.acceleration.x * deltaTime;

			rigidbody.velocity.y = rigidbody.velocity.y + rigidbody.acceleration.y * deltaTime;



		}

		//Clear Summed Forces
		rigidbody.forcesManager.SetSumForces({ 0.0f , 0.0f});
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

			UpdatePosition(entity);
			Integrate(entity);

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

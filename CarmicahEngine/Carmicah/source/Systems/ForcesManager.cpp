/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ForcesManager.cpp

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
	void ForcesManager::UpdateForces(Entity& obj, float deltaTime)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& force = componentManager->GetComponent<Forces>(obj);

		force.dragForce.Update(deltaTime, force.linearForce.unitDirection);
		force.linearForce.Update(deltaTime);
	}

	void ForcesManager::ApplyForces(Entity& obj, Vector2D<float>& velocity) 
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& force = componentManager->GetComponent<Forces>(obj);
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);

		if (force.dragForce.isActive) 
		{
			force.dragForce.Update(1.0f, rigidbody.velocity);

		}

		if (force.linearForce.isActive) 
		{
			rigidbody.velocity += force.linearForce.GetForceVector();
		}
	}

}

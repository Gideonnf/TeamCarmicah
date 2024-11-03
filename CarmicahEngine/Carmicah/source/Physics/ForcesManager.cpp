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
#include "Physics/PhysicsSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Physics/ForcesManager.h"
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


	Vector2D<float> LinearDirectionalForce::GetForceVector() const
	{
		return unitDirection * magnitude;
	}

	float LinearDirectionalForce::SetLifeTime(float lifeTimeValue) 
	{
		return this->lifetime = lifeTimeValue;
	}

	bool LinearDirectionalForce::ActivateForce(bool state)
	{
		return isActive = state;
	}

	void LinearDirectionalForce::Update(float deltaTime)
	{
		age += deltaTime;

		if (age >= lifetime)
		{
			age = 0.0f;
			std::cout << "Force with " << unitDirection.x << ", " << unitDirection.y << std::endl;
			isActive = false;
			
		}
	}

	void ForcesManager::AddLinearForce(LinearDirectionalForce& force) 
	{
		/*force.SetLifeTime(3.0f);
		force.ActivateForce();*/

		linearForces.push_back(force);

	}

	void ForcesManager::UpdateForces(float deltaTime)
	{
		//auto* componentManager = ComponentManager::GetInstance();

		// Clear accumulated force before recalculating
		accumulatedForce = { 0.0f, 0.0f };

		for (auto& force : linearForces) 
		{
			if (force.isActive == true) 
			{

				force.Update(deltaTime);
				
				accumulatedForce += force.GetForceVector();

				//std::cout << accumulatedForce << std::endl;

			}
			else 
			{
				//SetSumForces({ 0.0f,0.0f });
			}

		}

		
		
	}

	Vector2D<float> ForcesManager::GetSumForces() 
	{
		return accumulatedForce;
	}

	Vector2D<float> ForcesManager::SetSumForces(Vector2D<float> value)
	{
		return accumulatedForce = value;
	}

	/*void ForcesManager::ApplyForces(Entity& obj, Vector2D<float>& velocity) 
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);

		if (force.dragForce.isActive) 
		{
			force.dragForce.Update(1.0f, rigidbody.velocity);

		}

		if (force.linearForce.isActive) 
		{
			rigidbody.velocity += force.linearForce.GetForceVector();
		}
	}*/

}

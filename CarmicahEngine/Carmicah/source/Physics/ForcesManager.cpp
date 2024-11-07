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

	Vector2D<float> DragForce::CalculateDragForce(const Vector2D<float>& currentVelocity) const
	{
		Vector2D<float> result;

		// Assuming directionalDrag is a constant that includes fluid density and area
		result.x = -currentVelocity.x * directionalDrag;
		result.y = -currentVelocity.y * directionalDrag;

		return result;
	}


	void ForcesManager::AddLinearForce(LinearDirectionalForce& force) 
	{
		/*force.SetLifeTime(3.0f);
		force.ActivateForce();*/

		linearForces.push_back(force);

	}

	void ForcesManager::SetCurrentVelocity(const Vector2D<float>& velocity) 
	{
		objectVelocity = velocity;
	}



	void ForcesManager::RemoveForce()
	{
		// Remove all inactive linear forces
		linearForces.erase(
			std::remove_if(linearForces.begin(), linearForces.end(),
				[](const LinearDirectionalForce& force) { return !force.isActive; }),
			linearForces.end()
		);

	}

	void ForcesManager::UpdateForces(float deltaTime)
	{

		// Clear accumulated force before recalculating
		accumulatedForce = { 0.0f, 0.0f };

		for (auto& force : linearForces) 
		{
			if (force.isActive == true) 
			{

				force.Update(deltaTime);
				
				accumulatedForce += force.GetForceVector();

			}


		}

		accumulatedForce += dragForce.CalculateDragForce(objectVelocity);
				

		RemoveForce();
		
	}

	Vector2D<float> ForcesManager::GetSumForces() const 
	{
		return accumulatedForce;
	}

	Vector2D<float> ForcesManager::SetSumForces(Vector2D<float> value)
	{
		return accumulatedForce = value;
	}

}

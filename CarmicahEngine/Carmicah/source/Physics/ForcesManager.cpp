/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ForcesManager.cpp

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Functions definitions for the forces manager. Handles the individual forces of each game object

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

	/**
	 * @brief Calculates the force vector for a LinearDirectionalForce.
	 *
	 * This function multiplies the unit direction of the force with its magnitude
	 * to produce a resulting force vector.
	 *
	 * @return Vector2D<float> The calculated force vector.
	 */
	Vector2D<float> LinearDirectionalForce::GetForceVector() const
	{
		Vector2D<float> forceVector;

		forceVector = unitDirection * magnitude;

		return forceVector;
	}

	/**
	 * @brief Sets the lifetime of the linear force.
	 *
	 * Takes a float parameter and sets the lifetime of the LinearDirectionalForce
	 * instance to the given value.
	 *
	 * @param lifeTimeValue The desired lifetime of the force.
	 * @return float The updated lifetime value of the force.
	 */
	float LinearDirectionalForce::SetLifeTime(float lifeTimeValue) 
	{
		return this->lifetime = lifeTimeValue;
	}

	/**
	 * @brief Updates the age and activation status of the linear directional force.
	 *
	 * Increments the force's age by the given deltaTime. If the age exceeds the force's lifetime,
	 * it resets the age and deactivates the force.
	 *
	 * @param deltaTime The time elapsed since the last update, in seconds.
	 */
	void LinearDirectionalForce::Update(float deltaTime)
	{
		age += deltaTime;

		if (age >= lifetime)
		{
			age = 0.0f;
			isActive = false;
		}
	}

	/**
	 * @brief Calculates the drag force based on the current velocity.
	 *
	 * Computes a drag force that opposes the current velocity, based on the
	 * directional drag coefficient.
	 *
	 * @param currentVelocity The current velocity of the object.
	 * @return Vector2D<float> The calculated drag force vector.
	 */
	Vector2D<float> DragForce::CalculateDragForce(const Vector2D<float>& currentVelocity) const
	{
		Vector2D<float> result;

		// Assuming directionalDrag is a constant that includes fluid density and area
		result.x = -currentVelocity.x * directionalDrag;
		result.y = -currentVelocity.y * directionalDrag;

		return result;
	}

	/**
	 * @brief Adds a linear directional force to the forces manager.
	 *
	 * Pushes a given LinearDirectionalForce instance to the manager's list of active
	 * linear forces.
	 *
	 * @param force The linear directional force to add.
	 */
	void ForcesManager::AddLinearForce(LinearDirectionalForce& force) 
	{

		linearForces.push_back(force);

	}

	/**
	 * @brief Sets the current velocity of the object affected by forces.
	 *
	 * Stores the provided velocity vector as the object's current velocity,
	 * used in drag force calculations.
	 *
	 * @param velocity The current velocity vector of the object.
	 */
	void ForcesManager::SetCurrentVelocity(const Vector2D<float>& velocity) 
	{
		objectVelocity = velocity;
	}


	/**
	 * @brief Removes inactive forces from the forces manager.
	 *
	 * Checks each LinearDirectionalForce in the list and removes any that are inactive.
	 */
	void ForcesManager::RemoveForce()
	{
		// Remove all inactive linear forces
		linearForces.erase(
			std::remove_if(linearForces.begin(), linearForces.end(),
				[](const LinearDirectionalForce& force) { return !force.isActive; }),
			linearForces.end()
		);

	}

	/**
	 * @brief Updates all active forces and calculates the accumulated force.
	 *
	 * Resets the accumulated force, then iterates through active linear forces to
	 * update their status and add their force vectors. Also calculates and adds
	 * the drag force based on the object's velocity.
	 *
	 * @param deltaTime The time elapsed since the last update, in seconds.
	 */
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

	/**
	 * @brief Retrieves the total accumulated force on the object.
	 *
	 * Returns the sum of all active forces currently acting on the object.
	 *
	 * @return Vector2D<float> The accumulated force vector.
	 */
	Vector2D<float> ForcesManager::GetSumForces() const 
	{
		return accumulatedForce;
	}

	/**
	 * @brief Sets the accumulated force to a specified vector value.
	 *
	 * Directly sets the accumulated force to the provided vector, which may be
	 * used to manually control the net force applied to the object.
	 *
	 * @param value The vector value to set as the accumulated force.
	 * @return Vector2D<float> The updated accumulated force vector.
	 */
	Vector2D<float> ForcesManager::SetSumForces(Vector2D<float> value)
	{
		return accumulatedForce = value;
	}

}

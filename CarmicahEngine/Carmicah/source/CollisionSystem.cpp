/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        CollisionSystem.cpp

 author:	   Lee Yong Yee(90%)
 co-author(s): Gideon Francis(10%)

 email:        l.yongyee@digipen.edu

 brief:        Functions definitions for the Collision system. Handles the collision response and the collision checking between objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "Systems/CollisionSystem.h"
#include "Systems/GOFactory.h"
#include <ECS/ECSTypes.h>
#include <ECS/GameObject.h>
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/RigidBody.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"
#include <algorithm>

namespace Carmicah
{
	/**
	 * @brief Prints the number of entities currently tracked by the CollisionSystem.
	 */
	void CollisionSystem::PrintEntities()
	{
		std::cout << "Entities in collision system: " << mEntitiesSet.size() << std::endl;
	}

	/**
	 * @brief Updates the Axis-Aligned Bounding Box (AABB) for a given entity based on its transform and previous position.
	 *
	 * @param obj The entity whose AABB is being updated.
	 */
	void CollisionSystem::UpdateAABB(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& AABB = componentManager->GetComponent<Collider2D>(obj);
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);

		AABB.min.x = -(transform.xScale * 0.5f) + rigidbody.posPrev.x;
		AABB.min.y = -(transform.yScale * 0.5f) + rigidbody.posPrev.y;

		AABB.max.x = (transform.xScale * 0.5f) + rigidbody.posPrev.x;
		AABB.max.y = (transform.yScale * 0.5f) + rigidbody.posPrev.y;
	}

	/**
	 * @brief Checks for collision between two entities using their AABB and velocities.
	 *
	 * This function uses the separating axis theorem to determine if the AABBs intersect and returns true if a collision occurs.
	 * It also calculates the first time of collision (tFirst) within the current frame.
	 *
	 * @param obj1 The first entity to check for collision.
	 * @param obj2 The second entity to check for collision.
	 * @param tFirst A float that stores the first time of collision.
	 * @return True if the objects collide, false otherwise.
	 */
	bool CollisionSystem::CollisionIntersect(Entity& obj1, Entity& obj2, float tFirst)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);

		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(obj1);
		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(obj2);

		auto& AABB1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& AABB2 = componentManager->GetComponent<Collider2D>(obj2);

		
		
		
		// Continue with the standard AABB intersection check (as in your original logic)
		if (AABB1.max.x == 0.0f || AABB2.max.x == 0.0f)
		{
			return false;
		}

		if (!(AABB1.max.x < AABB2.min.x || AABB1.min.x > AABB2.max.x ||
			AABB1.max.y < AABB2.min.y || AABB1.min.y > AABB2.max.y))
		{
			return true;  // No collision if there's no overlap on either axis
		}

		

		float firstTimeOfCollision = 0.0f, tLast = CarmicahTimer::GetDt();

		if ((rigidbody1.velocity.x - rigidbody2.velocity.x) < 0) 
		{
			if (AABB1.min.x > AABB2.max.x) 
			{
				return false;
			}

			if (AABB1.max.x < AABB2.min.x) 
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB1.max.x - AABB2.min.x) / rigidbody1.velocity.x);
			}

			if (AABB1.min.x < AABB2.max.x) 
			{
				tLast = std::min((AABB1.min.x - AABB2.max.x) / rigidbody1.velocity.x, tLast);
			}

		}
		else if((rigidbody1.velocity.x - rigidbody2.velocity.x) > 0)
		{
			if (AABB1.min.x > AABB2.max.x)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB1.max.x - AABB2.min.x) / rigidbody1.velocity.x);
			}

			if (AABB1.max.x > AABB2.min.x)
			{
				tLast = std::min((AABB1.min.x - AABB2.max.x) / rigidbody1.velocity.x, tLast);
			}

			if (AABB1.max.x < AABB2.min.x)
			{
				return false;
			}
		}
		else 
		{
			if (AABB1.max.x < AABB2.min.x) 
			{
				return false;
			}

			if (AABB1.min.x > AABB2.max.x)
			{
				return false;
			}
		}

		if ((rigidbody1.velocity.y - rigidbody2.velocity.y) < 0)
		{
			if (AABB1.max.y < AABB2.min.y)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB1.max.y - AABB2.min.y) / rigidbody1.velocity.y);
			}

			if (AABB1.min.y < AABB2.max.y)
			{
				tLast = std::min((AABB1.min.y - AABB2.max.y) / rigidbody1.velocity.y, tLast);
			}

			if (AABB1.min.y > AABB2.max.y)
			{
				return false;
			}
		}
		else if ((rigidbody1.velocity.y - rigidbody2.velocity.y) > 0)
		{
			if (AABB1.min.y > AABB2.max.y)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB1.max.y - AABB2.min.y) / rigidbody1.velocity.y);
			}

			if (AABB1.max.y > AABB2.min.y)
			{
				tLast = std::min((AABB1.min.y - AABB2.max.y) / rigidbody1.velocity.y, tLast);
			}

			if (AABB1.max.y < AABB2.min.y)
			{
				return false;
			}
		}
		else
		{
			if (AABB1.max.y < AABB2.min.y)
			{
				return false;
			}

			if (AABB1.min.y > AABB2.max.y)
			{
				return false;
			}
		}

		if (firstTimeOfCollision > tLast || firstTimeOfCollision == 0.0f) 
		{
			return false;
		}

		tFirst = firstTimeOfCollision;

		return true;
	}

	/**
	 * @brief Handles the response to a collision between two entities. Updates their positions and velocities accordingly.
	 *
	 * Depending on the object types (dynamic, static, kinematic), the response will either stop the entities or destroy them.
	 *
	 * @param obj1 The first entity in the collision.
	 * @param obj2 The second entity in the collision.
	 * @param tFirst The first time of collision, used to update entity positions.
	 */
	void CollisionSystem::CollisionResponse(Entity& obj1, Entity& obj2, float tFirst)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(obj1);
		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(obj2);

		auto& collider1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& collider2 = componentManager->GetComponent<Collider2D>(obj2);

		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);


		// Handle dynamic vs static collision
		if (rigidbody1.objectType == "Dynamic" && rigidbody2.objectType == "Static" )
		{
			
			// Update position based on the first time of collision (tFirst)
			transform1.xPos = rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x;
			transform1.yPos = rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y;

			// Zero out both velocity components (or apply bounce/rest)
			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			gGOFactory->Destroy(obj1);

		}
		else if (rigidbody1.objectType == "Dynamic" && rigidbody2.objectType == "Dynamic")
		{
			transform1.xPos = rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x;
			transform1.yPos = rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y;

			transform2.xPos = rigidbody2.velocity.x * tFirst + rigidbody2.posPrev.x;
			transform2.yPos = rigidbody2.velocity.y * tFirst + rigidbody2.posPrev.y;

			
			// Zero out both velocity components (or apply bounce/rest)
			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			rigidbody2.velocity.x = 0;
			rigidbody2.velocity.y = 0;

			//std::cout << "Obj 1 Collided at " << collider1.max << " " << collider1.min << std::endl;
			//std::cout << "Obj 2 Collided at " << collider2.max << " " << collider2.min << std::endl;
		}
		else if (rigidbody1.objectType == "Kinematic" && rigidbody2.objectType == "Static")
		{
			// Update position based on the first time of collision (tFirst)
			transform1.xPos = rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x;
			transform1.yPos = rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y;

			// Zero out both velocity components (or apply bounce/rest)
			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			
		}


	}


	/**
	 * @brief Performs a static vs dynamic collision check for two entities.
	 *
	 * This function checks for collision between a dynamic and static object and calls the collision response if a collision occurs.
	 *
	 * @param obj1 The dynamic entity.
	 * @param obj2 The static entity.
	 */
	void CollisionSystem::StaticDynamicCollisionCheck(Entity& obj1, Entity& obj2) 
	{
		auto& rigidbody1 = ComponentManager::GetInstance()->GetComponent<RigidBody>(obj1);
		auto& AABB2 = ComponentManager::GetInstance()->GetComponent<Collider2D>(obj2);
		
		Vector2D<float> vec1;
		vec1.x = rigidbody1.posPrev.x - AABB2.min.x;
		vec1.y = rigidbody1.posPrev.y - AABB2.min.y;
		Vector2D<float> vec2;
		vec2.x = 0.0f;
		vec2.y = -1.0f;
		Vector2D<float> vec3;
		vec3.x = rigidbody1.posPrev.x - AABB2.max.x;
		vec3.y = rigidbody1.posPrev.y - AABB2.max.y;
		Vector2D<float> vec4;
		vec4.x = 1.0f;
		vec4.y = 0.0f;
		Vector2D<float> vec5;
		vec5.x = rigidbody1.posPrev.x - AABB2.max.x;
		vec5.y = rigidbody1.posPrev.y - AABB2.max.y;
		Vector2D<float> vec6;
		vec6.x = 0.0f;
		vec6.y = 1.0f;
		Vector2D<float> vec7;
		vec7.x = rigidbody1.posPrev.x - AABB2.min.x;
		vec7.y = rigidbody1.posPrev.y - AABB2.min.y;
		Vector2D<float> vec8;
		vec8.x = -1.0f;
		vec8.y = 0.0f;

		if (
			(Vector2DDotProduct<float>(vec1, vec2) >= 0.0f) && (Vector2DDotProduct<float>(rigidbody1.velocity, vec2) <= 0.0f) ||
			(Vector2DDotProduct<float>(vec3, vec4) >= 0.0f) && (Vector2DDotProduct<float>(rigidbody1.velocity, vec4) <= 0.0f) ||
			(Vector2DDotProduct<float>(vec5, vec6) >= 0.0f) && (Vector2DDotProduct<float>(rigidbody1.velocity, vec6) <= 0.0f) ||
			(Vector2DDotProduct<float>(vec7, vec8) >= 0.0f) && (Vector2DDotProduct<float>(rigidbody1.velocity, vec8) <= 0.0f)
			)
		{
			float firstTimeOfCollision = 0.0f;
			if (CollisionIntersect(obj1, obj2, firstTimeOfCollision) == true)
			{
				CollisionResponse(obj1, obj2, firstTimeOfCollision);
			}
		}

	}

	/**
	 * @brief Checks for collisions between all entities within the system.
	 *
	 * This function iterates over all dynamic entities and checks for collisions with other entities (static, dynamic, or kinematic).
	 */
	void CollisionSystem::CollisionCheck()
	{
		auto* componentManager = ComponentManager::GetInstance();

		for (auto it1 = mEntitiesSet.begin(); it1 != mEntitiesSet.end(); ++it1)
		{
			Entity entity1 = *it1;
			auto& rigidbody1 = componentManager->GetComponent<RigidBody>(entity1);

			if (rigidbody1.objectType == "Dynamic")
			{
				for (auto it2 = std::next(it1); it2 != mEntitiesSet.end(); ++it2) 
				{
					Entity entity2 = *it2;

					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);

					if (rigidbody2.objectType == "Static") 
					{
						StaticDynamicCollisionCheck(entity1, entity2);
					}
					else
					{
						float firstTimeOfCollision = 0.0f;
						if (CollisionIntersect(entity1, entity2, firstTimeOfCollision)) 
						{
							CollisionResponse(entity1, entity2, firstTimeOfCollision);
						}
					}
				}
			}
			else if (rigidbody1.objectType == "Kinematic") 
			{
				for (auto it2 = std::next(it1); it2 != mEntitiesSet.end(); ++it2)
				{
					Entity entity2 = *it2;

					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);

					if (rigidbody2.objectType == "Static")
					{
						StaticDynamicCollisionCheck(entity1, entity2);
					}
					else
					{
						float firstTimeOfCollision = 0.0f;
						if (CollisionIntersect(entity1, entity2, firstTimeOfCollision))
						{
							CollisionResponse(entity1, entity2, firstTimeOfCollision);
						}
					}
				}
			}
		}
	}

	/**
	 * @brief Initializes the CollisionSystem by setting its signature and registering the system with the SystemManager.
	 */
	void CollisionSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());

		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<CollisionSystem>(mSignature);
	}

	/**
	 * @brief Updates the CollisionSystem by updating the AABB of each entity and performing collision checks.
	 */
	void CollisionSystem::Update()
	{

		for (auto entity : mEntitiesSet)
		{

			UpdateAABB(entity);
			CollisionCheck();
		}


	}

	/**
	 * @brief Cleans up any resources or state when exiting the CollisionSystem.
	 */
	void CollisionSystem::Exit()
	{

	}

}

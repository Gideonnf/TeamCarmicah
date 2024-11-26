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
#include "../Physics/PhysicsSystem.h"
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
#include <cmath>

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
	void CollisionSystem::UpdateOBB(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& collider = componentManager->GetComponent<Collider2D>(obj);

		// Calculate the half-width and half-height of the object
		float halfWidth = (collider.customWidth * collider.localScale) * 0.5f;
		float halfHeight = (collider.customHeight * collider.localScale) * 0.5f;

		// Rotation angle in radians
		float angleInRadians = transform.Rot() * (PI / 180.0f);
		//float angleInRadians = collider.customRotation * (PI / 180.0f);
		float cosTheta = cos(angleInRadians);
		float sinTheta = sin(angleInRadians);

		GetOBBVertices(obj);

		//// Determine the min and max of the rotated vertices to define the AABB
		//Vec2f min(collider.objVert[0]);
		//Vec2f max(collider.objVert[0]);

		//for (const auto& vertex : collider.objVert)
		//{
		//	if (vertex.x < min.x) min.x = vertex.x;
		//	if (vertex.y < min.y) min.y = vertex.y;
		//	if (vertex.x > max.x) max.x = vertex.x;
		//	if (vertex.y > max.y) max.y = vertex.y;
		//}

		//// Update the collider's AABB
		//collider.min = min;
		//collider.max = max;

		collider.min.x = transform.Pos().x - halfWidth;
		collider.min.y = transform.Pos().y - halfHeight;
		collider.max.x = transform.Pos().x + halfWidth;
		collider.max.y = transform.Pos().y + halfHeight;
	}


	void CollisionSystem::GetOBBVertices(Entity& obj)
	{

		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& collider = componentManager->GetComponent<Collider2D>(obj);

		float halfWidth = (collider.customWidth * collider.localScale) * 0.5f;
		float halfHeight = (collider.customHeight * collider.localScale) * 0.5f;

		float angleInRadians = transform.Rot() * (PI / 180.0f); // Convert to radians
		float cosTheta = cos(angleInRadians);
		float sinTheta = sin(angleInRadians);

		collider.objVert.clear(); // Clear previous vertices

		// Calculate vertices based on rotation and position
		collider.objVert.push_back(transform.Pos() + Vec2f(-halfWidth * cosTheta - -halfHeight * sinTheta,
			-halfWidth * sinTheta + -halfHeight * cosTheta));

		collider.objVert.push_back(transform.Pos() + Vec2f(halfWidth * cosTheta - -halfHeight * sinTheta,
			halfWidth * sinTheta + -halfHeight * cosTheta));

		collider.objVert.push_back(transform.Pos() + Vec2f(halfWidth * cosTheta - halfHeight * sinTheta,
			halfWidth * sinTheta + halfHeight * cosTheta));

		collider.objVert.push_back(transform.Pos() + Vec2f(-halfWidth * cosTheta - halfHeight * sinTheta,
			-halfWidth * sinTheta + halfHeight * cosTheta));


	}

	void CollisionSystem::CalculateEdges(Entity& obj)
	{

		auto* componentManager = ComponentManager::GetInstance();
		auto& collider = componentManager->GetComponent<Collider2D>(obj);

		collider.objEdges.clear(); // Clear previous edges
		collider.objNormals.clear(); // Clear previous normals

		for (size_t i = 0; i < collider.objVert.size(); ++i)
		{
			Vec2f edge = collider.objVert[(i + 1) % collider.objVert.size()] - collider.objVert[i];
			collider.objEdges.push_back(edge);

			// Calculate and store edge normal
			Vec2f normal(edge.y, -edge.x); // Perpendicular normal
			normal.normalize(); // Ensure normal is unit length
			collider.objNormals.push_back(normal);
		}

	}

	int CollisionSystem::WhichSide(std::vector<Vec2f>& otherVertices, Vec2f& point, Vec2f& outwardNorm)
	{
		bool positive;
		bool negative;
		bool zero;

		positive = negative = zero = 0;

		for (size_t i = 0; i < otherVertices.size(); i++)
		{
			float projection = outwardNorm.dot(otherVertices[i] - point);

			if (projection > 0)
			{
				positive = true;
			}
			else if (projection < 0)
			{
				negative = true;
			}
			else
			{
				zero = true;
			}

			if (positive && negative || zero)
			{
				return 0;
			}

		}

		return positive ? 1 : -1;
	}


	/**
	 * @brief Checks for intersection/overlap between OBB objects
	 *
	 * Depending on the object types (dynamic, static, kinematic), the response will either stop the entities or destroy them.
	 *
	 * @param obj1 The first entity in the collision.
	 * @param obj2 The second entity in the collision.
	 */
	bool CollisionSystem::TestIntersection(Entity& obj1, Entity& obj2)
	{

		auto* componentManager = ComponentManager::GetInstance();
		auto& collider1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& collider2 = componentManager->GetComponent<Collider2D>(obj2);

		for (size_t i = 0, i1 = collider1.objVert.size() - 1; i < collider1.objVert.size(); i1 = i, i++)
		{

			Vec2f outwardNormal = collider1.objNormals[i];

			if (WhichSide(collider2.objVert, collider1.objVert[i], outwardNormal) > 0)
			{
				return false;
			}
		}

		for (size_t i = 0, i1 = collider2.objVert.size() - 1; i < collider2.objVert.size(); i1 = i, i++)
		{

			Vec2f outwardNormal = collider2.objNormals[i];

			if (WhichSide(collider1.objVert, collider2.objVert[i], outwardNormal) > 0)
			{
				return false;
			}
		}

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
	void CollisionSystem::CollisionResponse(Entity& obj1, Entity& obj2)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(obj1);
		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(obj2);

		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);

		//ResolvePenetration(obj1, obj2);

		// Handle dynamic vs static collision
		if (rigidbody1.objectType == rbTypes::DYNAMIC && rigidbody2.objectType == rbTypes::STATIC)
		{



			// Zero out the velocity of dynamic object (or apply bounce/rest)
			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			//gGOFactory->Destroy(obj1);

		}
		else if (rigidbody1.objectType == rbTypes::DYNAMIC && rigidbody2.objectType == rbTypes::KINEMATIC)
		{
			/*transform1.PosX(rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.velocity.x * tFirst + rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.velocity.y * tFirst + rigidbody2.posPrev.y);*/

			transform1.PosX(rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.posPrev.y);


			/*rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			rigidbody2.velocity.x = 0;
			rigidbody2.velocity.y = 0;*/


		}
		else if (rigidbody1.objectType == rbTypes::KINEMATIC && rigidbody2.objectType == rbTypes::STATIC)
		{
			// Update position based on the first time of collision (tFirst)
			transform1.PosX(rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.posPrev.y);


			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;


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

			if (rigidbody1.objectType == rbTypes::DYNAMIC)
			{
				for (auto it2 = mEntitiesSet.begin(); it2 != mEntitiesSet.end(); ++it2)
				{
					Entity entity2 = *it2;

					if (entity2 == entity1)
					{
						continue;
					}

					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);


					if (TestIntersection(entity1, entity2))
					{
						CollisionResponse(entity1, entity2);
					}
				}
			}
			else if (rigidbody1.objectType == rbTypes::KINEMATIC)
			{
				for (auto it2 = mEntitiesSet.begin(); it2 != mEntitiesSet.end(); ++it2)
				{
					Entity entity2 = *it2;

					if (entity2 == entity1)
					{
						continue;
					}

					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);


					if (TestIntersection(entity1, entity2))
					{

						CollisionResponse(entity1, entity2);
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

			UpdateOBB(entity);
			//GetOBBVertices(entity);
			CalculateEdges(entity);
		}

		CollisionCheck();



	}

	/**
	 * @brief Cleans up any resources or state when exiting the CollisionSystem.
	 */
	void CollisionSystem::Exit()
	{

	}

}
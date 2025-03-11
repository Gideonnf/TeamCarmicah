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
#include "TransformSystem.h"
#include "Systems/GOFactory.h"
#include "Systems/AssetManager.h"
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
	 * @brief Retrieves or assigns a unique index for the given entity.
	 *
	 * This function checks if the given entity already has an assigned index in the `entityIndexMap`.
	 * If the entity is not found, it assigns a new unique index to the entity, increments the
	 * internal counter, and stores the mapping. The function then returns the index of the entity.
	 *
	 * @param entity The entity for which to retrieve or assign an index.
	 * @return int The unique index associated with the given entity.
	 */
	int CollisionSystem::GetEntityIndex(Entity& entity)
	{
		if (entityIndexMap.find(entity) == entityIndexMap.end())
		{
			entityIndexMap[entity] = entityCounter++;
		}
		return entityIndexMap[entity];
	}

	/**
	 * @brief Inserts an entity into the spatial grid based on its position.
	 *
	 * This function calculates the grid cell for the given entity based on its position
	 * and updates the corresponding row and column bitsets in the spatial grid.
	 *
	 * @param entity The entity to be inserted into the grid.
	 * @param position The position of the entity in 2D space.
	 */
	void CollisionSystem::InsertEntityToGrid(Entity& entity, const Vec2f& position)
	{
		int row = static_cast<int>(position.y / cellSize);
		int col = static_cast<int>(position.x / cellSize);
		int entityIndex = GetEntityIndex(entity);

		if (row >= 0 && row < GRID_HEIGHT)
		{
			rowsBitArray[row].set(entityIndex);
		}
		if (col >= 0 && col < GRID_WIDTH)
		{
			colsBitArray[col].set(entityIndex);
		}
	}

	/**
	 * @brief Retrieves potential collision candidates for a given entity.
	 *
	 * This function determines the grid cell for the given entity and combines
	 * the bitsets for the corresponding row and column. It then returns a list
	 * of entities that potentially collide with the given entity.
	 *
	 * @param entity The entity for which to find potential collisions.
	 * @param position The position of the entity in 2D space.
	 * @return std::vector<Entity> A vector of entities that potentially collide with the given entity.
	 */
	std::vector<Entity> CollisionSystem::GetPotentialCollisions(Entity& entity, const Vec2f& position)
	{
		int row = static_cast<int>(position.y / cellSize);
		int col = static_cast<int>(position.x / cellSize);
		int entityIndex = GetEntityIndex(entity);

		std::bitset<MAX_ENTITIES> combined = 0;

		if (row >= 0 && row < GRID_HEIGHT)
		{
			combined |= rowsBitArray[row];  // Merge row entities
		}
		if (col >= 0 && col < GRID_WIDTH)
		{
			combined |= colsBitArray[col];  // Merge column entities
		}

		// Remove self
		combined.reset(entityIndex);

		std::vector<Entity> potentialCollisions;
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (combined.test(i)) // Check if bit is set
			{
				for (const auto& pair : entityIndexMap)
				{
					if (pair.second == i)
					{
						potentialCollisions.push_back(pair.first);
						break;
					}
				}
			}
		}

		return potentialCollisions;
	}

	/**
	 * @brief Clears all entities from the spatial grid.
	 *
	 * This function resets all bitsets in both the row and column arrays,
	 * effectively removing all entities from the spatial grid.
	 */
	void CollisionSystem::ClearGrid() 
	{
		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			rowsBitArray[i].reset();
		}
		for (int i = 0; i < GRID_WIDTH; i++)
		{
			colsBitArray[i].reset();
		}
	}

	/**
	 * @brief Updates the Oriented Bounding Box (OBB) for a given entity based on its transform and associated primitive model.
	 *
	 * This function calculates the OBB by determining the minimum and maximum bounds of the entity's vertices,
	 * applying scaling and rotation according to the entity's transform, and updating the collider component
	 * with the new OBB vertices. It also handles texture changes to ensure that the OBB is recalculated when necessary.
	 *
	 * @param obj The entity whose OBB is being updated.
	 */
	void CollisionSystem::UpdateOBB(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& collider = componentManager->GetComponent<Collider2D>(obj);

		std::vector<Vec2f> worldVertices;
		if (componentManager->HasComponent<Renderer>(obj))
		{
			auto& rend = componentManager->GetComponent<Renderer>(obj);
			
			// Retrieve the Primitive associated with the Renderer
			const auto& assetManager = AssetManager::GetInstance();
			if (!assetManager->AssetExist<Primitive>(rend.model))
			{
				CM_CORE_WARN("Model not found for entity's Renderer: " + rend.model);
				return;
			}
			
			const auto& primitive = assetManager->GetAsset<Primitive>(rend.model);

			// Initialize customWidth and customHeight if they are uninitialized
			
				// Find the bounds of the primitive's vertices
				float minX = primitive.vtx[0].x;
				float maxX = primitive.vtx[0].x;
				float minY = primitive.vtx[0].y;
				float maxY = primitive.vtx[0].y;

				for (const auto& vertex : primitive.vtx)
				{
					if (vertex.x < minX)
					{
						minX = vertex.x;
					}
					if (vertex.x > maxX)
					{
						maxX = vertex.x;
					}
					if (vertex.y < minY)
					{
						minY = vertex.y;
					}
					if (vertex.y > maxY)
					{
						maxY = vertex.y;
					}
				}

				std::string retrievedTexture = rend.GetTexture();

				if (collider.currentTexture == "" || (collider.currentTexture != retrievedTexture && !componentManager->HasComponent<Animation>(obj)))
				{
					collider.currentTexture = retrievedTexture;
					collider.OBBinit = false;
				}


				if (collider.OBBinit == false)
				{
					if (collider.customWidth == 0)
						collider.CustomWidth( maxX - minX);
					if (collider.customHeight == 0)
						collider.CustomHeight(maxY - minY);

					collider.OBBinit = true;
				}

				if (transform.Scale().x == 0 || transform.Scale().y == 0)
				{
					transform.ScaleX(1.0f);
					transform.ScaleY(1.0f);
				}
			
				if (transform.ParentID() != 0)
				{
					auto& parent = componentManager->GetComponent<Transform>(transform.ParentID());

					// Compute the world position of the child object
					Vec2f worldPos = {
						transform.worldSpace.m20, // Translation x-component
						transform.worldSpace.m21  // Translation y-component
					};

					// Compute combined scale
					Vec2f combinedScale = {
						transform.Scale().x * transform.CalcedRenderingScale().x * parent.accumulatedScale.x,
						transform.Scale().y * transform.CalcedRenderingScale().y * parent.accumulatedScale.y
					};

					// Compute combined rotation (assuming rotations are additive)
					float combinedRot = transform.Rot() + parent.Rot();
					float angle = combinedRot * (PI / 180.0f);
					float cosTheta = cos(angle);
					float sinTheta = sin(angle);

					float halfWidth = collider.GetCustomWidth() * 0.5f * std::abs(combinedScale.x);
					float halfHeight = collider.GetCustomHeight() * 0.5f * std::abs(combinedScale.y);

					// Clear existing OBB vertices
					collider.objVert.clear();

					// Recalculate child OBB vertices with combined transformations
					std::vector<Vec2f> obbVertices;
					obbVertices.emplace_back(worldPos.x + halfWidth * cosTheta - halfHeight * sinTheta,
						worldPos.y + halfWidth * sinTheta + halfHeight * cosTheta); // Top-right
					obbVertices.emplace_back(worldPos.x - halfWidth * cosTheta - halfHeight * sinTheta,
						worldPos.y - halfWidth * sinTheta + halfHeight * cosTheta); // Top-left
					obbVertices.emplace_back(worldPos.x - halfWidth * cosTheta + halfHeight * sinTheta,
						worldPos.y - halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-left
					obbVertices.emplace_back(worldPos.x + halfWidth * cosTheta + halfHeight * sinTheta,
						worldPos.y + halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-right

					collider.objVert = obbVertices;

					// Update min and max bounds for the collider
					collider.min.x = worldPos.x - halfWidth;
					collider.min.y = worldPos.y - halfHeight;
					collider.max.x = worldPos.x + halfWidth;
					collider.max.y = worldPos.y + halfHeight;
				}
				else
				{
					// Calculate half-dimensions of the OBB
					float halfWidth = collider.GetCustomWidth() * 0.5f * transform.CalcedRenderingScale().x * std::abs(transform.Scale().x);
					float halfHeight = collider.GetCustomHeight() * 0.5f * transform.CalcedRenderingScale().y * std::abs(transform.Scale().y);

					// Rotation in radians
					float angle = transform.Rot() * (PI / 180.0f);
					float cosTheta = cos(angle);
					float sinTheta = sin(angle);

					collider.objVert.clear();

					// Calculate OBB corners relative to center
					Vec2f center = transform.Pos();
					std::vector<Vec2f> obbVertices;
					obbVertices.emplace_back(center.x + halfWidth * cosTheta - halfHeight * sinTheta,
						center.y + halfWidth * sinTheta + halfHeight * cosTheta); // Top-right

					obbVertices.emplace_back(center.x - halfWidth * cosTheta - halfHeight * sinTheta,
						center.y - halfWidth * sinTheta + halfHeight * cosTheta); // Top-left
					obbVertices.emplace_back(center.x - halfWidth * cosTheta + halfHeight * sinTheta,
						center.y - halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-left
					obbVertices.emplace_back(center.x + halfWidth * cosTheta + halfHeight * sinTheta,
						center.y + halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-right

					// Update the collider with OBB vertices
					collider.objVert = obbVertices;

					collider.min.x = transform.Pos().x - halfWidth;
					collider.min.y = transform.Pos().y - halfHeight;
					collider.max.x = transform.Pos().x + halfWidth;
					collider.max.y = transform.Pos().y + halfHeight;
				}
			


		}

	}

	/**
	 * @brief Retrieves and calculates the vertices for the Oriented Bounding Box (OBB) of a given entity.
	 *
	 * This function computes the OBB vertices based on the entity's transform, including its position, scale,
	 * and rotation. The calculated vertices are stored in the collider component for use in collision detection.
	 *
	 * @param obj The entity whose OBB vertices are being calculated.
	 */
	void CollisionSystem::GetOBBVertices(Entity& obj)
	{

		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& collider = componentManager->GetComponent<Collider2D>(obj);

		float halfWidth = collider.customWidth * 0.5f * transform.Scale().x;
		float halfHeight = collider.customHeight * 0.5f * transform.Scale().y;

		float angleInRadians = transform.Rot() * (PI / 180.0f); // Convert to radians
		float cosTheta = cos(angleInRadians);
		float sinTheta = sin(angleInRadians);

		collider.objVert.clear(); // Clear previous vertices

		// Calculate OBB corners relative to center
		Vec2f center = transform.Pos();
		std::vector<Vec2f> obbVertices;
		obbVertices.emplace_back(center.x + halfWidth * cosTheta - halfHeight * sinTheta,
			center.y + halfWidth * sinTheta + halfHeight * cosTheta); // Top-right
		obbVertices.emplace_back(center.x - halfWidth * cosTheta - halfHeight * sinTheta,
			center.y - halfWidth * sinTheta + halfHeight * cosTheta); // Top-left
		obbVertices.emplace_back(center.x - halfWidth * cosTheta + halfHeight * sinTheta,
			center.y - halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-left
		obbVertices.emplace_back(center.x + halfWidth * cosTheta + halfHeight * sinTheta,
			center.y + halfWidth * sinTheta - halfHeight * cosTheta); // Bottom-right

		// Update the collider with OBB vertices
		collider.objVert = obbVertices;


	}

	/**
	 * @brief Calculates the edges and normals for the OBB of a given entity.
	 *
	 * This function iterates through the vertices of the OBB to compute the edges and their corresponding
	 * normals. These edges and normals are stored in the collider component, which are essential for collision
	 * detection algorithms.
	 *
	 * @param obj The entity whose OBB edges and normals are being calculated.
	 */
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

	/**
	 * @brief Computes the projection interval of an entity's vertices onto a given edge normal.
	 *
	 * This function calculates the minimum and maximum values of the projections of the entity's vertices
	 * onto the specified edge normal. The results are stored in the provided reference parameters `min` and `max`.
	 * This is useful for determining how far apart two entities are along a specific axis, which is a key step
	 * in collision detection algorithms, particularly in the Separating Axis Theorem (SAT).
	 *
	 * @param obj The entity whose projection interval is being calculated.
	 * @param edgeNormal The normal vector of the edge onto which the vertices will be projected.
	 * @param min Reference to a float that will hold the minimum projection value.
	 * @param max Reference to a float that will hold the maximum projection value.
	 */
	void CollisionSystem::ComputeProjInterval(Entity& obj, Vec2f edgeNormal, float& min, float& max)
	{
			auto* componentManager = ComponentManager::GetInstance();
			
			auto& collider = componentManager->GetComponent<Collider2D>(obj);

			//min = max = edgeNormal.dot(collider.objEdges[0]);
			min = max = edgeNormal.dot(collider.objVert[0]);

			for (size_t i = 1; i < collider.objVert.size(); i++)
			{

				float value = edgeNormal.dot(collider.objVert[i]);

				if (value < min)
				{
					min = value;
				}
				else if (value > max)
				{
					max = value;
				}
			}
	}


	/**
	 * @brief Determines which side of a line defined by a point and outward normal a set of vertices lies on.
	 *
	 * This function projects each vertex onto the outward normal and counts how many vertices lie on each side
	 * of the line. It returns +1 if all vertices are on one side, -1 if they are on the opposite side, or 0 if
	 * they are mixed or on the line.
	 *
	 * @param otherVertices The vertices to be tested against the line.
	 * @param point A point defining one end of the line.
	 * @param outwardNorm The outward normal defining the direction of the line.
	 * @return An integer indicating which side of the line the vertices lie on (+1, -1, or 0).
	 */
	int CollisionSystem::WhichSide(std::vector<Vec2f>& otherVertices, Vec2f& point, Vec2f& outwardNorm)
	{
		int positive;
		int negative;
		int zero;

		positive = negative = zero = 0;

		for (size_t i = 0; i < otherVertices.size(); i++)
		{
			float projection = outwardNorm.dot(otherVertices[i] - point);

			if (projection > 0)
			{
				positive++;
			}
			else if (projection < 0)
			{
				negative++;
			}
			else
			{
				zero++;
			}

			if ((positive && negative) || zero)
			{
				return 0;
			}

		}

		return (positive > 0) ? 1 : -1;
	}

	/**
	 * @brief Calculates the penetration depth between two entities that are colliding.
	 *
	 * This function computes how deep two entities overlap based on their positions and dimensions. It returns
	 * the maximum penetration depth along either axis if they intersect, or 0 if there is no overlap.
	 *
	 * @param obj1 The first entity involved in the collision.
	 * @param obj2 The second entity involved in the collision.
	 * @return The penetration depth as a float value. Returns 0 if no collision occurs.
	 */
	float CollisionSystem::CalculatePenetrationDepth(Entity& obj1, Entity& obj2)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);
		auto& collider1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& collider2 = componentManager->GetComponent<Collider2D>(obj2);

		float halfWidth1 = (collider1.GetCustomWidth() ) * 0.5f;
		float halfHeight1 = (collider1.GetCustomHeight() ) * 0.5f;

		float halfWidth2 = (collider2.GetCustomWidth() ) * 0.5f;
		float halfHeight2 = (collider2.GetCustomHeight() ) * 0.5f;

		Vec2f distance = transform1.Pos() - transform2.Pos();

		Vec2f overlap;

		overlap.x = halfWidth1 + halfWidth2 - std::abs(distance.x);
		overlap.y = halfHeight1 + halfHeight2 - std::abs(distance.y);

		if (overlap.x > 0 && overlap.y > 0)
		{
			return std::max(overlap.x, overlap.y);
		}

		return 0.0f;
	}

	/**
	 * @brief Calculates the collision normal vector between two colliding entities.
	 *
	 * This function determines the direction from one entity to another at the point of collision. The result is
	 * a normalized vector that can be used to resolve collisions or apply forces.
	 *
	 * @param obj1 The first entity involved in the collision.
	 * @param obj2 The second entity involved in the collision.
	 * @return A normalized vector representing the collision normal direction from obj1 to obj2.
	 */
	Vec2f CollisionSystem::CalculateCollisionNormal(Entity& obj1, Entity& obj2)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);

		return (transform2.Pos() - transform1.Pos()).normalize();
	}

	/**
	 * @brief Resolves penetration between two colliding entities by adjusting their positions.
	 *
	 * This function calculates how much two entities have penetrated into each other and adjusts their positions
	 * accordingly to eliminate overlap. It handles dynamic and kinematic object types differently during this process.
	 *
	 * @param obj1 The first entity involved in the collision resolution.
	 * @param obj2 The second entity involved in the collision resolution.
	 */
	void CollisionSystem::ResolvePenetration(Entity& obj1, Entity& obj2)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform1 = componentManager->GetComponent<Transform>(obj1);
		auto& transform2 = componentManager->GetComponent<Transform>(obj2);
		auto& collider1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& collider2 = componentManager->GetComponent<Collider2D>(obj2);
		UNUSED(collider2);

		float penetrationDepth = CalculatePenetrationDepth(obj1, obj2);

		float halfWidth1 = (collider1.customWidth * collider1.localScale) * 0.5f;
		float halfHeight1 = (collider1.customHeight * collider1.localScale) * 0.5f;

		Vec2f correction;

		correction.x = (transform1.Pos().x - transform2.Pos().x) * (penetrationDepth / halfWidth1);
		correction.y = (transform1.Pos().y - transform2.Pos().y) * (penetrationDepth / halfHeight1);

		transform1.PosXAdd(correction.x * 0.5f);
		transform1.PosYAdd(correction.y * 0.5f);

		transform2.PosXAdd(-(correction.x * 0.5f));
		transform2.PosYAdd(-(correction.y * 0.5f));

	}

	/**
	 * @brief Checks for intersection or overlap between two OBB objects.
	 *
	 * This function determines whether two entities' OBBs intersect. It evaluates each edge of both OBBs against
	 * each other using their normals. Depending on object types (dynamic, static, kinematic), it may trigger
	 * different responses upon detection of a collision.
	 *
	 * @param obj1 The first entity in the collision check.
	 * @param obj2 The second entity in the collision check.
	 * @return True if there is an intersection; otherwise false.
	 */
	bool CollisionSystem::TestIntersection(Entity& obj1, Entity& obj2)
	{

		auto* componentManager = ComponentManager::GetInstance();
		auto& collider1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& collider2 = componentManager->GetComponent<Collider2D>(obj2);

		// Check if either object has no vertices (invalid collider)
		if (collider1.objVert.empty() || collider2.objVert.empty())
		{
			return false;
		}

		// Test edges of collider1
		for (size_t i = 0; i < collider1.objEdges.size(); i++)
		{
			Vec2f outwardNormal = collider1.objNormals[i];

			float min0, max0, min1, max1;
			ComputeProjInterval(obj1, outwardNormal, min0, max0);
			ComputeProjInterval(obj2, outwardNormal, min1, max1);

			// If projections do not overlap, return false (no collision)
			if (max0 < min1 || max1 < min0)
			{
				return false;
			}
		}

		// Test edges of collider2
		for (size_t i = 0; i < collider2.objEdges.size(); i++)
		{
			Vec2f outwardNormal = collider2.objNormals[i];

			float min0, max0, min1, max1;
			ComputeProjInterval(obj1, outwardNormal, min0, max0);
			ComputeProjInterval(obj2, outwardNormal, min1, max1);

			// If projections do not overlap, return false (no collision)
			if (max0 < min1 || max1 < min0)
			{
				return false;
			}
		}

		// No separating axis found, objects are intersecting
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

			if (rigidbody1.collided == false)
			{
				//CM_CORE_INFO(" COLLISION");

				EntityCollidedMessage newMsg(obj1, obj2, CollideType::ON_COLLIDE);
				SendSysMessage(&newMsg);
				rigidbody1.collided = true;
			}
			else
			{

			}

			//gGOFactory->Destroy(obj1);

		}
		else if (rigidbody1.objectType == rbTypes::DYNAMIC && rigidbody2.objectType == rbTypes::DYNAMIC)
		{
			/*transform1.PosX(rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.velocity.x * tFirst + rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.velocity.y * tFirst + rigidbody2.posPrev.y);*/

			transform1.PosX(rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.posPrev.y);


			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			if (rigidbody1.collided == false)
			{
				//CM_CORE_INFO(" COLLISION");

				EntityCollidedMessage newMsg(obj1, obj2, CollideType::ON_COLLIDE);
				SendSysMessage(&newMsg);
				rigidbody1.collided = true;
			}
			else
			{

			}
			/*rigidbody2.velocity.x = 0;
			rigidbody2.velocity.y = 0;*/


		}
		else if (rigidbody1.objectType == rbTypes::KINEMATIC && rigidbody2.objectType == rbTypes::DYNAMIC || rigidbody1.objectType == rbTypes::DYNAMIC && rigidbody2.objectType == rbTypes::KINEMATIC)
		{
			/*transform1.PosX(rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.velocity.x * tFirst + rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.velocity.y * tFirst + rigidbody2.posPrev.y);*/

			transform1.PosX(rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.posPrev.y);

			transform2.PosX(rigidbody2.posPrev.x);
			transform2.PosY(rigidbody2.posPrev.y);



			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;
			//CM_CORE_INFO("Obj 1 {}, Obj 2 {}", obj1, obj2);
			if (rigidbody1.collided == false)
			{
				//CM_CORE_INFO(" COLLISION");

				EntityCollidedMessage newMsg(obj1, obj2, CollideType::ON_COLLIDE);
				SendSysMessage(&newMsg);
				rigidbody1.collided = true;
			}
			else
			{

			}

			/*rigidbody2.velocity.x = 0;
			rigidbody2.velocity.y = 0;*/


		}
		else if (rigidbody1.objectType == rbTypes::KINEMATIC && rigidbody2.objectType == rbTypes::STATIC)
		{
			// Update position based on the first time of collision (tFirst)
			transform1.PosX(rigidbody1.posPrev.x);
			transform1.PosY(rigidbody1.posPrev.y);


			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;
			
			// hasn't collided with this entity before
			if (mEntityTriggerMap[obj1].count(obj2) == 0)
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_ENTER);
				SendSysMessage(&newMsg);

				mEntityTriggerMap[obj1].insert(obj2);
			}
			else
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_STAY);
				SendSysMessage(&newMsg);
			}

			////CM_CORE_INFO("ID in CollisionSystem 2nd {}", obj1);
			//if (rigidbody1.collided == false)
			//{
			//	EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_ENTER);
			//	SendSysMessage(&newMsg);
			//	rigidbody1.collided = true;
			//}
			//else if (rigidbody1.collided)
			//{
			//	EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_STAY);
			//	SendSysMessage(&newMsg);
			//}
		}
		else if (rigidbody1.objectType == rbTypes::KINEMATIC && rigidbody2.objectType == rbTypes::KINEMATIC)
		{
			//CM_CORE_INFO("Obj 1 {}, Obj 2 {}", obj1, obj2);

			// trigger
			/*if (rigidbody1.triggerCollide == false)
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_ENTER);
				SendSysMessage(&newMsg);
				rigidbody1.triggerCollide = true;

			}
			else if (rigidbody1.triggerCollide)
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_STAY);
				SendSysMessage(&newMsg);
			}*/

			if (mEntityTriggerMap[obj1].count(obj2) == 0)
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_ENTER);
				SendSysMessage(&newMsg);

				mEntityTriggerMap[obj1].insert(obj2);
			}
			else
			{
				EntityCollidedMessage newMsg(obj1, obj2, CollideType::TRIGGER_STAY);
				SendSysMessage(&newMsg);
			}
		}

		// set collided to true at the end
		// so that we can differentiate on trigger enter
		// vs on trigger stay
		//rigidbody1.collided = true;
		//rigidbody2.collided = true;
	}

	/**
	 * @brief Checks for collisions between all entities within the system.
	 *
	 * This function iterates over all dynamic entities and checks for collisions with other entities (static, dynamic, or kinematic).
	 */
	void CollisionSystem::CollisionCheck()
	{
		ClearGrid(); // Reset grid

		auto* componentManager = ComponentManager::GetInstance();

		// Insert all entities into the grid
		for (auto entity : mEntitiesSet)
		{
			auto& transform = componentManager->GetComponent<Transform>(entity);
			InsertEntityToGrid(entity, transform.Pos());
		}

		bool collided = false;
		bool triggerCollide = false;

		// Perform collision detection
		for (auto entity1 : mEntitiesSet)
		{
			auto& transform1 = componentManager->GetComponent<Transform>(entity1);
			auto& rigidbody1 = componentManager->GetComponent<RigidBody>(entity1);

			// for duck taping purposes
			std::unordered_set<Entity> collidedEntities;

			if (rigidbody1.objectType == rbTypes::DYNAMIC)
			{
				std::vector<Entity> nearbyEntities = GetPotentialCollisions(entity1, transform1.Pos());

				for (Entity entity2 : nearbyEntities)
				{
					auto& transform2 = componentManager->GetComponent<Transform>(entity2);
					auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
					if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
					{
						if (TestIntersection(entity1, entity2))
						{
							//if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
							//{
							//	CM_CORE_INFO("why tf is it colliding");
							//}

							CollisionResponse(entity1, entity2);
							collided = true;
						}
					}
				}
			}
			else if (rigidbody1.objectType == rbTypes::KINEMATIC)
			{
				std::vector<Entity> nearbyEntities = GetPotentialCollisions(entity1, transform1.Pos());
				
				for (Entity entity2 : nearbyEntities)
				{

					auto& transform2 = componentManager->GetComponent<Transform>(entity2);
					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);
					auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
					if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
					{
						if (TestIntersection(entity1, entity2))
						{
							//if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
							//{
							//	CM_CORE_INFO("why tf is it colliding");
							//}

							
							CollisionResponse(entity1, entity2);
							if (rigidbody2.objectType == rbTypes::KINEMATIC)
							{
								collidedEntities.insert(entity2);
								//triggerCollide = true;
							}
							/*else
							{
								collided = true;
							}*/
							//collided = true;
						}
					}
				}
			}

			// check which entities entity 1 is no longer colliding with
			for (auto it : mEntityTriggerMap[entity1])
			{
				// if its not collided but it is part of entity 1's trigger map
				// that means we need to trigger on exit
				if (collidedEntities.count(it) == 0)
				{
					EntityCollidedMessage newMsg(entity1, it, CollideType::TRIGGER_EXIT);
					SendSysMessage(&newMsg);

					mEntityTriggerMap.erase(it);
				}
			}

			// if entity 1 was already colliding but no longer
				// then call on trigger exit
			//if (triggerCollide != true)
			//{
			//	if (rigidbody1.triggerCollide)
			//	{
			//		// OKAY TECHNICALLY ANY OBJECT WILL GET THIS CALL IF IT HAS THIS FUNCTION
			//		// JUST DONT USE IT OK? 
			//		// I KINDA LAZY TO FIND A WAY TO DIFFERENTIATE KINEMATIC w KINEMATIC COLLISION EXIT
			//		// VS KINEMATIC w OTHER TYPE COLLISION EXIT
			//		//CM_CORE_INFO("ENDING COLLISION");

			//		// 0 on 2nd entity cause trigger exit doesnt care about collided entity
			//		EntityCollidedMessage newMsg(entity1, 0, CollideType::TRIGGER_EXIT);
			//		SendSysMessage(&newMsg);
			//		rigidbody1.triggerCollide = false;
			//	}
			//}

			if (collided != true)
			{
				if (rigidbody1.collided)
				{
					rigidbody1.collided = false;
				}
			}
		}
	}

	/// <summary>
	/// If this function works, bless
	/// im just ducktaping something to move without linear forces so its more snappy
	/// </summary>
	/// <param name="entity1"></param>
	/// <param name="displacement"></param>
	/// <returns></returns>
	bool CollisionSystem::CollisionCheck(Entity entity1, Vec2f displacement)
	{
		ClearGrid(); // Reset grid

		auto* componentManager = ComponentManager::GetInstance();

		// Insert all entities into the grid
		for (auto entity : mEntitiesSet)
		{
			auto& transform = componentManager->GetComponent<Transform>(entity);
			InsertEntityToGrid(entity, transform.Pos() + displacement);
		}

		auto& transform1 = componentManager->GetComponent<Transform>(entity1);
		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(entity1);

		if (rigidbody1.objectType == rbTypes::DYNAMIC)
		{
			std::vector<Entity> nearbyEntities = GetPotentialCollisions(entity1, transform1.Pos() + displacement);

			for (Entity entity2 : nearbyEntities)
			{
				auto& transform2 = componentManager->GetComponent<Transform>(entity2);
				auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
				if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
				{
					if (TestIntersection(entity1, entity2))
					{
						//if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
						//{
						//	CM_CORE_INFO("why tf is it colliding");
						//}

						return true;
					}
				}
			}
		}
		else if (rigidbody1.objectType == rbTypes::KINEMATIC)
		{
			std::vector<Entity> nearbyEntities = GetPotentialCollisions(entity1, transform1.Pos() + displacement);

			for (Entity entity2 : nearbyEntities)
			{

				auto& transform2 = componentManager->GetComponent<Transform>(entity2);
				auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);
				auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
				if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
				{
					if (TestIntersection(entity1, entity2))
					{
						//if (transformSys->CheckLayerInteraction(transform1.collisionMask, transform2.collisionMask))
						//{
						//	CM_CORE_INFO("why tf is it colliding");
						//}


						return true;
					}
				}
			}
		}

		return false;
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

		//InitGrid(currentEntityCount);

	}

	/**
	 * @brief Updates the CollisionSystem by updating the AABB of each entity and performing collision checks.
	 */
	void CollisionSystem::Update()
	{
		//UpdateEntityCount(mEntitiesSet.size());

		for (auto entity : mEntitiesSet)
		{

			UpdateOBB(entity);
			//GetOBBVertices(entity);
			CalculateEdges(entity);
		}

		//CollisionCheck();



	}

	/**
	 * @brief Cleans up any resources or state when exiting the CollisionSystem.
	 */
	void CollisionSystem::Exit()
	{

	}

}
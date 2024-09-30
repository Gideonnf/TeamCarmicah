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

	void CollisionSystem::PrintEntities()
	{
		std::cout << "Entities in collision system: " << mEntitiesSet.size() << std::endl;
	}

	void CollisionSystem::UpdateAABB(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& transform = componentManager->GetComponent<Transform>(obj);
		auto& AABB = componentManager->GetComponent<Collider2D>(obj);
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);

		AABB.min.x = rigidbody.posPrev.x - (transform.xScale);
		AABB.min.y = rigidbody.posPrev.y - (transform.yScale);
		AABB.max.x = rigidbody.posPrev.x + (transform.xScale);
		AABB.max.y = rigidbody.posPrev.y + (transform.yScale);
	}

	bool CollisionSystem::CollisionIntersect(Entity& obj1, Entity& obj2, float tFirst)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(obj1);
		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(obj2);

		auto& AABB1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& AABB2 = componentManager->GetComponent<Collider2D>(obj2);

		float firstTimeOfCollision = 0, tLast = CarmicahTimer::GetDt();

		// Check for no overlap on the x-axis
		if (AABB2.max.x < AABB1.min.x || AABB2.min.x > AABB1.max.x || AABB2.max.x == AABB1.min.x || AABB2.min.x == 0.0f) {

			if (AABB2.max.y < AABB1.min.y || AABB2.min.y > AABB1.max.y || AABB2.max.y == AABB1.min.y || AABB2.min.y == 0.0f) {

				return false;

			}

		}

		//Dynamic Collision Checking from the right for x-axis
		if ((rigidbody1.velocity.x - rigidbody2.velocity.x) < 0)
		{
			if (AABB2.max.x < AABB1.min.x)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB2.max.x - AABB1.min.x) / rigidbody1.velocity.x);
			}

			if (AABB2.min.x < AABB1.max.x)
			{
				tLast = std::min((AABB2.min.x - AABB1.max.x) / rigidbody1.velocity.x, tLast);
			}

			if (AABB2.min.x > AABB1.max.x)
			{
				return false;
			}
		}
		//Dynamic Collision Checking from the left for the x-axis
		else if ((rigidbody1.velocity.x - rigidbody2.velocity.x) > 0)
		{
			if (AABB2.min.x > AABB1.max.x)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB2.min.x - AABB1.max.x) / rigidbody1.velocity.x);
			}

			if (AABB2.max.x > AABB1.min.x)
			{
				tLast = std::min((AABB2.max.x - AABB1.min.x) / rigidbody1.velocity.x, tLast);
			}

			if (AABB2.max.x < AABB1.min.x)
			{
				return false;
			}
		}
		//Velocity is equal to zero
		else
		{
			if (AABB2.max.x < AABB1.min.x)
			{
				return false;
			}

			if (AABB2.min.x < AABB1.max.x)
			{
				return false;
			}
		}

		//Dynamic Collision Checking from the right for the y-axis
		if ((rigidbody1.velocity.y - rigidbody2.velocity.y) < 0)
		{
			if (AABB2.max.y < AABB1.min.y)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB2.max.y - AABB1.min.y) / rigidbody1.velocity.y);
			}
			if (AABB2.min.y < AABB1.max.y)
			{
				tLast = std::min((AABB2.min.y - AABB1.max.y) / rigidbody1.velocity.y, tLast);
			}

			if (AABB2.min.y < AABB1.max.y)
			{
				return false;
			}
		}
		//Dynamic Collision Checking from the left for the y-axis
		else if ((rigidbody1.velocity.y - rigidbody2.velocity.y) > 0)
		{
			if (AABB2.min.y > AABB1.max.y)
			{
				firstTimeOfCollision = std::max(firstTimeOfCollision, (AABB2.min.y - AABB1.max.y) / rigidbody1.velocity.y);
			}

			if (AABB2.max.y > AABB1.min.y)
			{
				tLast = std::min((AABB2.max.y - AABB1.min.y) / rigidbody1.velocity.y, tLast);
			}

			if (AABB2.max.y < AABB1.min.y)
			{
				return false;
			}
		}
		else
		{
			if (AABB2.max.y < AABB1.min.y)
			{
				return false;
			}

			if (AABB2.min.y > AABB1.max.y)
			{
				return false;
			}
		}

		if (firstTimeOfCollision > tLast)
		{
			return false;
		}

		tFirst = firstTimeOfCollision;

		return true;

	}

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
		if (rigidbody1.objectType == "Dynamic" && rigidbody2.objectType == "Static")
		{
			// Update position based on the first time of collision (tFirst)
			transform1.xPos = rigidbody1.velocity.x * tFirst + rigidbody1.posPrev.x;
			transform1.yPos = rigidbody1.velocity.y * tFirst + rigidbody1.posPrev.y;

			// Zero out both velocity components (or apply bounce/rest)
			rigidbody1.velocity.x = 0;
			rigidbody1.velocity.y = 0;

			//gGOFactory->EntityDestroyed(obj1);

		}


	}



	void CollisionSystem::CollisionCheck()
	{
		auto* componentManager = ComponentManager::GetInstance();

		// Separate static and dynamic entities
		for (auto it1 = mEntitiesSet.begin(); it1 != mEntitiesSet.end(); ++it1)
		{
			Entity entity1 = *it1;
			auto& rigidbody1 = componentManager->GetComponent<RigidBody>(entity1);

			// Handle Dynamic vs Static Collision
			//if (rigidbody1.objectType == DYNAMIC)
			//{
			//	for (auto it2 = mEntitiesSet.begin(); it2 != mEntitiesSet.end(); ++it2)
			//	{
			//		Entity entity2 = *it2;
			//		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);

			//		// Only check for collisions with static objects
			//		if (rigidbody2.objectType == STATIC)
			//		{
			//			float tFirst = 0.0f;
			//			if (CollisionIntersect(entity1, entity2, tFirst))
			//			{
			//				CollisionResponse(entity1, entity2, tFirst);
			//			}
			//		}
			//	}
			//}

			for (auto it2 = mEntitiesSet.begin(); it2 != mEntitiesSet.end(); ++it2)
			{
				Entity entity2 = *it2;

				if (entity2 == entity1)
				{
					continue;
				}
				auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);



				// Only check for collisions with static objects
				/*if (rigidbody2.objectType == STATIC)
				{*/
				float tFirst = 0.0f;
				if (CollisionIntersect(entity1, entity2, tFirst))
				{
					CollisionResponse(entity1, entity2, tFirst);
				}
				//}
			}
		}
	}

	void CollisionSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());

		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<CollisionSystem>(mSignature);
	}

	void CollisionSystem::Update()
	{
		//std::cout << mSignature << std::endl;



		for (auto entity : mEntitiesSet)
		{

			UpdateAABB(entity);



			CollisionCheck();
		}


	}

	void CollisionSystem::Exit()
	{

	}

}

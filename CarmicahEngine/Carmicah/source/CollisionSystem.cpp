#include "pch.h"
#include "Systems/CollisionSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

namespace Carmicah
{

	void CollisionSystem::PrintEntities()
	{
		std::cout << "Entities in collision system: " << mEntitiesSet.size() << std::endl;
	}

	void CollisionSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());

		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<CollisionSystem>(mSignature);
	}

	void CollisionSystem::Update()
	{
		//std::cout << mSignature << std::endl;

		std::vector<Entity> tmp(mEntitiesSet.begin(), mEntitiesSet.end());

		std::sort(tmp.begin(), tmp.end(), [](Entity a, Entity b) 
		{
				auto& transformA = ComponentManager::GetInstance()->GetComponent<Transform>(a);
				auto& transformB = ComponentManager::GetInstance()->GetComponent<Transform>(b);
				return transformA.xPos < transformB.xPos;
		});

		for (auto entity : tmp) 
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			auto& AABB = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			//transform.yPos += 1;
			//std::cout << "Entity xPos : " << transform.xPos << std::endl;

			//Form AABB bounding box
			AABB.minX = transform.xPos - (transform.scaleX * 0.5f);
			AABB.minY = transform.yPos - (transform.scaleY * 0.5f);
			AABB.maxX = transform.xPos + (transform.scaleX * 0.5f);
			AABB.maxY = transform.yPos + (transform.scaleY * 0.5f);
		}

		for (size_t i = 0; i < tmp.size(); ++i)
		{
			Entity entity1 = tmp[i];

			auto& transform1 = ComponentManager::GetInstance()->GetComponent<Transform>(entity1);
			auto& AABB1 = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity1);

			// Inner loop: Compare entity1 with subsequent entities
			for (size_t j = i + 1; j < tmp.size(); ++j)
			{
				Entity entity2 = tmp[j];

				auto& transform2 = ComponentManager::GetInstance()->GetComponent<Transform>(entity2);
				auto& AABB2 = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity2);

				// Perform AABB collision check between entity1 and entity2
				if (AABB1.maxX > AABB2.minX && AABB1.minX < AABB2.maxX &&
					AABB1.maxY > AABB2.minY && AABB1.minY < AABB2.maxY)
				{
					// Handle collision between entity1 and entity2
					std::cout << "Collision detected between Entity " << entity1 << " and Entity " << entity2 << std::endl;
				}
			}
		}
	}

	void CollisionSystem::Exit()
	{

	}

}

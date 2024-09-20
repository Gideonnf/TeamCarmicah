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

		for (auto entity : mEntitiesSet) 
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			auto& AABB = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			//transform.yPos += 1;
			//std::cout << "Entity xPos : " << transform.xPos << std::endl;

			//Form AABB bounding box
			AABB.minX = transform.xPos - (transform.xScale * 0.5f);
			AABB.minY = transform.yPos - (transform.yScale * 0.5f);
			AABB.maxX = transform.xPos + (transform.xScale * 0.5f);
			AABB.maxY = transform.yPos + (transform.yScale * 0.5f);
		}

		//for (auto it1 = mEntitiesSet.begin(); it1 != mEntitiesSet.end(); ++it1)
		//{
		//	Entity entity1 = *it1;
		//	auto& AABB1 = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity1);

		//	for (auto it2 = std::next(it1); it2 != mEntitiesSet.end(); ++it2)
		//	{
		//		Entity entity2 = *it2;
		//		auto& AABB2 = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity2);

		//		// Perform AABB collision check
		//		if (AABB1.maxX > AABB2.minX && AABB1.minX < AABB2.maxX &&
		//			AABB1.maxY > AABB2.minY && AABB1.minY < AABB2.maxY)
		//		{
		//			// Handle collision between entity1 and entity2
		//			std::cout << "Collision detected between Entity " << entity1 << " and Entity " << entity2 << std::endl;
		//		}
		//	}
		//}
	}

	void CollisionSystem::Exit()
	{

	}

}

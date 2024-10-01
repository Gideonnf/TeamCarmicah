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

		AABB.min.x = rigidbody.posPrev.x - (transform.xScale *0.5f);
		AABB.min.y = rigidbody.posPrev.y - (transform.yScale *0.5f);

		AABB.max.x = rigidbody.posPrev.x + (transform.xScale *0.5f);
		AABB.max.y = rigidbody.posPrev.y + (transform.yScale *0.5f);
	}

	bool CollisionSystem::CollisionIntersect(Entity& obj1, Entity& obj2, float tFirst)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody1 = componentManager->GetComponent<RigidBody>(obj1);
		auto& rigidbody2 = componentManager->GetComponent<RigidBody>(obj2);

		auto& AABB1 = componentManager->GetComponent<Collider2D>(obj1);
		auto& AABB2 = componentManager->GetComponent<Collider2D>(obj2);

		float firstTimeOfCollision = 0.0f, tLast = CarmicahTimer::GetDt();

		

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

			//gGOFactory->Destroy(obj1);

		}


	}

	void StaticDynamicCollisionCheck(Entity& obj1, Entity& obj2) 
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


	}

	void CollisionSystem::CollisionCheck()
	{
		auto* componentManager = ComponentManager::GetInstance();

		// Separate static and dynamic entities
		for (auto it1 = mEntitiesSet.begin(); it1 != mEntitiesSet.end(); ++it1)
		{
			Entity entity1 = *it1;
			auto& rigidbody1 = componentManager->GetComponent<RigidBody>(entity1);

			if (rigidbody1.objectType == "Dynamic") 
			{
				for (auto it2 = std::next(it1); it2 == mEntitiesSet.end(); ++it2) 
				{
					Entity entity2 = *it2;

					auto& rigidbody2 = componentManager->GetComponent<RigidBody>(entity2);

					if (rigidbody2.objectType == "Static") 
					{
						StaticDynamicCollisionCheck(entity1, entity2);
					}
				}
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

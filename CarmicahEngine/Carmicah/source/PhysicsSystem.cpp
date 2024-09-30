#include "pch.h"
#include "Systems/PhysicsSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Collider2D.h"
#include "Systems/GOFactory.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"

namespace Carmicah
{

	void PhysicsSystem::UpdatePosition(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		rigidbody.posPrev.x = transform.xPos;
		rigidbody.posPrev.y = transform.yPos;

		rigidbody.zposPrev = transform.zPos;
	}

	void PhysicsSystem::ApplyForce(Entity& obj)
	{
		auto* componentManager = ComponentManager::GetInstance();
		auto& rigidbody = componentManager->GetComponent<RigidBody>(obj);
		auto& transform = componentManager->GetComponent<Transform>(obj);

		float deltaTime = CarmicahTimer::GetDt();

		if (rigidbody.objectType == "Dynamic")
		{

			//rigidbody.velocity.y += gravity.gravity * deltaTime;

			transform.xPos += rigidbody.velocity.x * deltaTime;
			//transform.yPos += rigidbody.velocity.y * deltaTime;

		}
		else if (rigidbody.objectType == "Kinematic")
		{
			if (rigidbody.velocity.x != 0)
			{
				transform.xPos += rigidbody.velocity.x * deltaTime;
			}
		}
	}

	void PhysicsSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<PhysicsSystem>(mSignature);



	}

	void PhysicsSystem::Update()
	{


		for (auto entity : mEntitiesSet)
		{
			ApplyForce(entity);
			UpdatePosition(entity);

		}
	}

	void PhysicsSystem::Exit()
	{

	}
}

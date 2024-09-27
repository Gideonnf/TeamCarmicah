#include "pch.h"
#include "Systems/PhysicsSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Gravity.h"
#include "Systems/GOFactory.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"

namespace Carmicah 
{
	void PhysicsSystem::ApplyForce(Entity& obj) 
	{
		auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(obj);
		auto& jump = ComponentManager::GetInstance()->GetComponent<Gravity>(obj);
		auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(obj);

		if (!rigidbody.isStatic && rigidbody.isKinematic) 
		{
			rigidbody.velocity.y += jump.gravity * CarmicahTimer::GetDt();

			transform.xPos += rigidbody.velocity.x * CarmicahTimer::GetDt();
			transform.yPos += rigidbody.velocity.y * CarmicahTimer::GetDt();
		}
	}
	
	void PhysicsSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Gravity>());

		SystemManager::GetInstance()->SetSignature<PhysicsSystem>(mSignature);

		

	}

	void PhysicsSystem::Update() 
	{
		for (auto entity : mEntitiesSet) 
		{
			
			ApplyForce(entity);

		}
	}

	void PhysicsSystem::Exit() 
	{

	}
}
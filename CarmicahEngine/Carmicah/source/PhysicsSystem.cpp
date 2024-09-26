#include "pch.h"
#include "Systems/PhysicsSystem.h"
#include <ECS/ECSTypes.h>
#include "Components/Transform.h"
#include "Components/Body.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

namespace Carmicah 
{

	void PhysicsSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Body>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<PhysicsSystem>(mSignature);
	}

	void PhysicsSystem::Update() 
	{

	}

	void PhysicsSystem::Exit() 
	{

	}
}
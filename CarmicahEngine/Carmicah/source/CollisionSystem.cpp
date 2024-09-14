#include "pch.h"
#include "CollisionSystem.h"
#include <ECSTypes.h>
#include "Transform.h"
#include "Collider2D.h"
#include "SystemManager.h"
#include "ComponentManager.h"

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
		transform.yPos += 1;		
	}
}

void CollisionSystem::Exit()
{

}

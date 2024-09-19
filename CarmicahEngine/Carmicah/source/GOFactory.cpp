#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"

namespace Carmicah
{

	GOFactory::GOFactory()
	{
		//Create pointer to entity manager
		mEntityManager = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
	}

	GOFactory::~GOFactory()
	{
		// Destroy all objects if factory is deleted
		DestroyAll();
	}

#pragma region GameObject Functions

	void GOFactory::CreateGO(std::string name)
	{
		GameObject go;
		go.mID = mEntityManager->CreateEntity(name);
	}

	void GOFactory::CreateGO(GameObject* go)
	{
		go->mID = mEntityManager->CreateEntity(go->mName);
	}

	void GOFactory::DestroyGameObject(GameObject* go)
	{
		mEntityManager->DeleteEntity(go->mID);
		mComponentManager->EntityDestroyed(go->mID);
		SystemManager::GetInstance()->EntityDestroyed(go->mID);
	}

	void GOFactory::DestroyAll()
	{

	}

#pragma endregion

#pragma region Component Functions
	template <typename T>
	void GOFactory::CreateComponent()
	{
		ComponentManager::GetInstance()->RegisterComponent<T>();
	}

	template <typename T>
	void GOFactory::AddComponent()
	{

	}





#pragma endregion
}
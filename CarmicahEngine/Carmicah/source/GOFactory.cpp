#include "pch.h"
#include "Systems/GOFactory.h"

namespace Carmicah
{
	GOFactory* gGOFactory = NULL;

	GOFactory::GOFactory()
	{
		if (gGOFactory == NULL)
			gGOFactory = this;
		//Create pointer to entity manager
		//mEntityManager = std::make_unique<EntityManager>();
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
		go.mID = EntityManager::GetInstance()->CreateEntity(name);
	}

	void GOFactory::CreateGO(GameObject* go)
	{
		go->mID = EntityManager::GetInstance()->CreateEntity(go->mName);
	}

	void GOFactory::DestroyGameObject(GameObject* go)
	{
		EntityManager::GetInstance()->DeleteEntity(go->mID);
		ComponentManager::GetInstance()->EntityDestroyed(go->mID);
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



#pragma endregion
}
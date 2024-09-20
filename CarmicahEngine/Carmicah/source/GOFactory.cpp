#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "log.h"

namespace Carmicah
{
	// Trying this method for instantiating gofactory
	// subject to change
	GOFactory* gGOFactory = NULL;

	GOFactory::GOFactory()
	{
		//
		if (gGOFactory == NULL)
			gGOFactory = this;

		//Create pointer to entity manager
		//mEntityManager = std::make_unique<EntityManager>();
		//mComponentManager = std::make_unique<ComponentManager>();
	}

	GOFactory::~GOFactory()
	{
		// Destroy all objects if factory is deleted
		DestroyAll();
	}

#pragma region GameObject Functions

	GameObject GOFactory::CreateGO(std::string name)
	{
		GameObject go;
		go.mID = EntityManager::GetInstance()->CreateEntity(name);
		go.mName = name;
		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(go.mName, go.mID));
		mIDToGO.insert(std::make_pair(go.mID, go));

		// All entities created is stored in GOFactory
		SystemManager::GetInstance()->UpdateSignatures(go.mID, EntityManager::GetInstance()->GetSignature(go.mID));
		
		Carmicah::log::getCoreLogger()->info("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));

		return go;
	}

	GameObject GOFactory::CloneGO(GameObject const& go)
	{
		GameObject newGO(go);
		newGO.mName = go.mName + "_Clone"; // TODO: Find a way to create clones like clone1, clone2, clone3, etc
		newGO.mID = EntityManager::GetInstance()->CreateEntity(newGO.mName);
		EntityManager::GetInstance()->CloneEntity(go.mID, newGO.mID);

		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(newGO.mName, newGO.mID));
		mIDToGO.insert(std::make_pair(newGO.mID, newGO));

		Carmicah::log::getCoreLogger()->info("Creating a clone of game object with name: " + newGO.mName + " with id: " + std::to_string(newGO.mID));

		//Updating of signature done in the cloning process in entity manager

		return newGO;
	}

	void GOFactory::CreateGO(GameObject go)
	{
		go.mID = EntityManager::GetInstance()->CreateEntity(go.mName);
	}

	void GOFactory::EntityDestroyed(Entity id) 
	{
		mDeleteList.insert(id);

		//EntityManager::GetInstance()->DeleteEntity(id);
		//ComponentManager::GetInstance()->EntityDestroyed(id);
	}

	void GOFactory::DestroyAll()
	{
		// Since GOFactory holds all the entities
		// Can just use entitiesSet to destroy all
		mDeleteList = mEntitiesSet;
	}

	void GOFactory::UpdateDestroyed()
	{
		std::set<Entity>::iterator iter;
		for (iter = mDeleteList.begin(); iter != mDeleteList.end(); ++iter)
		{
			EntityManager::GetInstance()->DeleteEntity(*iter);
			ComponentManager::GetInstance()->EntityDestroyed(*iter);
		}

		// Clear delete list afterwards
		mDeleteList.clear();
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
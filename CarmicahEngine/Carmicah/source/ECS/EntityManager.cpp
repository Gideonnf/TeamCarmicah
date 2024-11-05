/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        EntityManager.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A singleton manager to handle the creation and management of Entity IDs and their Entity Signature which is a bitset that represents
				the components that the entity has

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "EntityManager.h"
#include "ComponentManager.h"
namespace Carmicah
{
	EntityManager::EntityManager()
	{
		// entity ID 0 is used for scenes only 


		m_EntityCount = 0;
		// Initialize free entities with the total number of max entities that we can have
		//mFreeEntities.reserve(MAX_ENTITIES);
		for (Entity i_Entity = 1; i_Entity < MAX_ENTITIES; ++i_Entity)
		{
			// push back so itll go from MAX_ENTITIES at the front to 1 at the back
			mFreeEntities.push_back(i_Entity);
		}
	}

	// entityName is always defaulted to gameobject when an entity is created
	Entity EntityManager::CreateEntity()
	{
		assert(m_EntityCount <= MAX_ENTITIES && "Too many entities");

		// Get the front most id in the queue
		Entity entityId = mFreeEntities.front();
		// Set the active flag
		mEntityActive[entityId] = true;
		// Pop it to remove it afterwards
		mFreeEntities.pop_front();
		// Increment to keep track of the current number of entities
		m_EntityCount++;

		return entityId;
	}

	Entity EntityManager::LoadEntity(Entity entityID)
	{

		if (entityID == mFreeEntities.front())
		{
			Entity entID = mFreeEntities.front();
			mFreeEntities.pop_front();
			mEntityActive[entID] = true;
			m_EntityCount++;
			return entID;
		}
		else
		{
			//mFreeEntities.erase()
			for (auto it = mFreeEntities.begin(); it != mFreeEntities.end(); it++)
			{
				if (*it == entityID)
				{
					Entity entID = *it;
					mFreeEntities.erase(it);
					mEntityActive[entID] = true;
					m_EntityCount++;
					return entID;
				}
			}
		}

		return Entity{};
	}

	void EntityManager::DeleteEntity(Entity entity)
	{
		// Reset the signature of the entity when destroyed
		mEntitySignatures[entity].reset();
		mEntityActive[entity] = false;
		// Put the id back to the deque to be reused
		mFreeEntities.push_back(entity);
		m_EntityCount--;
	}

	void EntityManager::CloneEntity(Entity entityToClone, Entity newEntity)
	{
		// CloneEntity will create a copy of all the component data from the entity to clone
		// and attach it to the new entity's id in component manager
		ComponentManager::GetInstance()->CloneEntity(entityToClone, newEntity, mEntitySignatures[entityToClone]);

		// this part handles updating of signature so itll reflect in systems
		mEntitySignatures[newEntity] = mEntitySignatures[entityToClone];
	}

	void EntityManager::SetSignature(Entity entity, Signature entitySignature)
	{
		mEntitySignatures[entity] = entitySignature;
	}

	Signature EntityManager::GetSignature(Entity entity) const
	{
		return mEntitySignatures[entity];
	}

	bool EntityManager::DoesEntityExist(Entity entity)
	{
		return mEntityActive[entity];
	}

}
#include "pch.h"
#include "Entity.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
	// Initialize free entities with the total number of max entities that we can have
	for (Entity i_Entity = 0; i_Entity < MAX_ENTITIES; ++i_Entity)
	{
		m_FreeEntities.push(i_Entity);
	}
}

Entity EntityManager::CreateEntity()
{
	if (m_EntityCount > MAX_ENTITIES)
	{
		// Too many entities
		// can output an error here
	}
	// Get the front most id in the queue
	Entity entityId = m_FreeEntities.front();
	// Pop it to remove it afterwards
	m_FreeEntities.pop();
	// Increment to keep track of the current number of entities
	m_EntityCount++;
	return entityId;
}

void EntityManager::DeleteEntity(Entity entity)
{
	// Reset the signature of the entity when destroyed
	m_EntitySignatures[entity].reset();
	// Put the id back to the queue to be reused
	m_FreeEntities.push(entity);
	m_EntityCount--;
}

void EntityManager::SetSignature(Entity entity, EntitySignature entitySignature)
{
	m_EntitySignatures[entity] = entitySignature;
}

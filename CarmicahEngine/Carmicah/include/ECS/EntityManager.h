#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "ECSTypes.h"
#include <array>
#include <queue>
#include <unordered_map>
#include "Singleton.h"
#include "GameObject.h"

namespace Carmicah
{
	class EntityManager : public Singleton<EntityManager>
	{
	private:
		// Keep track of the current queue of free entity IDs that can be used
		std::queue<Entity> m_FreeEntities;
		// Maps the entity signature to the entity ID
		std::array<Signature, MAX_ENTITIES> m_EntitySignatures;
		// Keep track of the number of active entities
		unsigned int m_EntityCount;

	public:
		EntityManager()
		{
			m_EntityCount = 0;
			// Initialize free entities with the total number of max entities that we can have
			for (Entity i_Entity = 0; i_Entity < MAX_ENTITIES; ++i_Entity)
			{
				m_FreeEntities.push(i_Entity);
			}
		}

		// entityName is always defaulted to gameobject when an entity is created
		Entity CreateEntity(std::string entityName)
		{
			if (m_EntityCount > MAX_ENTITIES)
			{
				//TODO: Add error response
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

		void DeleteEntity(Entity entity)
		{
			// Reset the signature of the entity when destroyed
			m_EntitySignatures[entity].reset();
			// Put the id back to the queue to be reused
			m_FreeEntities.push(entity);
			m_EntityCount--;
		}

		void CloneEntity(Entity entityToClone, Entity newEntity)
		{
			// CloneEntity will create a copy of all the component data from the entity to clone
			// and attach it to the new entity's id in component manager
			ComponentManager::GetInstance()->CloneEntity(entityToClone, newEntity, m_EntitySignatures[entityToClone]);

			// this part handles updating of signature so itll reflect in systems
			m_EntitySignatures[newEntity] = m_EntitySignatures[entityToClone];
			SystemManager::GetInstance()->UpdateSignatures(newEntity, m_EntitySignatures[newEntity]);
		}

		void SetSignature(Entity entity, Signature entitySignature)
		{
			m_EntitySignatures[entity] = entitySignature;
		}

		Signature GetSignature(Entity entity)
		{
			return m_EntitySignatures[entity];
		}

		template<typename T>
		void AddComponent(Entity id, T component)
		{
			Signature entitySignature = m_EntitySignatures[id];
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), true);
			m_EntitySignatures[id] = entitySignature;
			SystemManager::GetInstance()->UpdateSignatures(id, entitySignature);
		}

		template<typename T>
		void RemoveComponent(Entity id)
		{
			Signature entitySignature = m_EntitySignatures[id];
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), false);
			m_EntitySignatures[id] = entitySignature;
			SystemManager::GetInstance()->UpdateSignatures(id, entitySignature);
		}
	};
}

#endif
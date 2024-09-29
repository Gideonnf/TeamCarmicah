#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "ECSTypes.h"
#include <array>
#include <deque>
#include <unordered_map>
#include "Singleton.h"
#include "GameObject.h"

namespace Carmicah
{
	class EntityManager : public Singleton<EntityManager>
	{
	private:
		// Keep track of the current queue of free entity IDs that can be used
		//std::queue<Entity> m_FreeEntities;
		std::deque<Entity> mFreeEntities;
		// Maps the entity signature to the entity ID
		std::array<Signature, MAX_ENTITIES> mEntitySignatures;
		// Keep track of the number of active entities
		unsigned int m_EntityCount;

	public:
		EntityManager()
		{
			m_EntityCount = 0;
			// Initialize free entities with the total number of max entities that we can have
			//mFreeEntities.reserve(MAX_ENTITIES);
			for (Entity i_Entity = 0; i_Entity < MAX_ENTITIES; ++i_Entity)
			{
				// push back so itll go from MAX_ENTITIES at the front to 0 at the back
				mFreeEntities.push_back(i_Entity);
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
			Entity entityId = mFreeEntities.front();
			// Pop it to remove it afterwards
			mFreeEntities.pop_front();
			// Increment to keep track of the current number of entities
			m_EntityCount++;

			return entityId;
		}

		Entity LoadEntity(Entity entityID)
		{

			if (entityID == mFreeEntities.front())
			{
				Entity entID = mFreeEntities.front();
				mFreeEntities.pop_front();
				m_EntityCount++;
				return entID;
			}
			else
			{
				//mFreeEntities.erase()
				for (auto& it = mFreeEntities.begin(); it != mFreeEntities.end(); it++)
				{
					if (*it == entityID)
					{
						Entity entID = *it;
						mFreeEntities.erase(it);
						m_EntityCount++;
						return entID;
					}
				}
			}
		}

		void DeleteEntity(Entity entity)
		{
			// Reset the signature of the entity when destroyed
			mEntitySignatures[entity].reset();
			// Put the id back to the vector to be reused
			mFreeEntities.push_front(entity);
			m_EntityCount--;
		}

		void CloneEntity(Entity entityToClone, Entity newEntity)
		{
			// CloneEntity will create a copy of all the component data from the entity to clone
			// and attach it to the new entity's id in component manager
			ComponentManager::GetInstance()->CloneEntity(entityToClone, newEntity, mEntitySignatures[entityToClone]);

			// this part handles updating of signature so itll reflect in systems
			mEntitySignatures[newEntity] = mEntitySignatures[entityToClone];
		}

		void SetSignature(Entity entity, Signature entitySignature)
		{
			mEntitySignatures[entity] = entitySignature;
		}

		Signature GetSignature(Entity entity) const
		{
			return mEntitySignatures[entity];
		}

		template<typename T>
		void AddComponent(Entity id)
		{
			Signature entitySignature = mEntitySignatures[id];
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), true);
			mEntitySignatures[id] = entitySignature;
			SystemManager::GetInstance()->UpdateSignatures(id, entitySignature);
		}

		template<typename T>
		void RemoveComponent(Entity id)
		{
			Signature entitySignature = mEntitySignatures[id];
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), false);
			mEntitySignatures[id] = entitySignature;
			SystemManager::GetInstance()->UpdateSignatures(id, entitySignature);
		}
	};
}

#endif
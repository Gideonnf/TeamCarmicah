/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:      EntityManager.h

 author:    Gideon Francis (100%)

 email:     g.francis@digipen.edu

 brief:     A singleton manager to handle the creation and management of Entity IDs and their Entity Signature which is a bitset that represents
		    the components that the entity has

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
		// Keep a map that keeps track if an entity ID is active or inactive
		std::array<bool, MAX_ENTITIES> mEntityActive;
		// Keep track of the number of active entities
		unsigned int m_EntityCount;

	public:
		EntityManager();

		// entityName is always defaulted to gameobject when an entity is created
        /*!*************************************************************************
        \brief
        	Create a Entity object
        
        \return Entity
        	
        ***************************************************************************/
		Entity CreateEntity();

        /*!*************************************************************************
        \brief
        	Load an entity ID. Allow for replacing an entity ID from anywhere in mFreeEntities
        
        \param[in] entityID
        	
        
        \return Entity - ID 
        	
        ***************************************************************************/
		Entity LoadEntity(Entity entityID);

        /*!*************************************************************************
        \brief
        	Called when deleting an entity
        
        \param[in] entity
        	
        
        ***************************************************************************/
		void DeleteEntity(Entity entity);

        /*!*************************************************************************
        \brief
        	Used to clone Entity
        
        \param[in] entityToClone
        	Entity to clone
        
        \param[in] newEntity
        	
        
        ***************************************************************************/
		void CloneEntity(Entity entityToClone, Entity newEntity);

        /*!*************************************************************************
        \brief
        	Set the Signature object
        
        \param[in] entity
        	
        
        \param[in] entitySignature
        	
        
        ***************************************************************************/
		void SetSignature(Entity entity, Signature entitySignature);

        /*!*************************************************************************
        \brief
        	Get the Signature object
        
        \param[in] entity
        	
        
        \return Signature
        	
        ***************************************************************************/
		Signature GetSignature(Entity entity) const;

        /*!*************************************************************************
        \brief
        	Check if entity exist
        
        \param[in] entity
        	
        
        \return true
        	
        \return false
        	
        ***************************************************************************/
		bool DoesEntityExist(Entity entity);

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
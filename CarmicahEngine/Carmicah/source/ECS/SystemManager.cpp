/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:      SystemManager.cpp

 author:	Gideon Francis (100%)

 email:     g.francis@digipen.edu

 brief:     A singleton manager class that controls all systems in the engine. Stores them in a map based on the string representation of the system.
			Also acts as a message dispatcher to systems that subscribe to specific systems to receive messages from them. Also updates all systems when
			an entity has a new component added to map them to their entitiesSet or if an entity was destroyed and needs to be removed

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "SystemManager.h"
#include "Systems/SceneSystem.h"
#include "ComponentManager.h"
#include "EntityManager.h"

namespace Carmicah
{
	void SystemManager::EntityDestroyed(Entity entity)
	{
		std::unordered_map<std::string, std::shared_ptr<BaseSystem>>::iterator iSystemIterator = mSystems.begin();
		for (; iSystemIterator != mSystems.end(); ++iSystemIterator)
		{
			// It exist within that system's entity set
			if (iSystemIterator->second->mEntitiesSet.count(entity) != 0)
			{
				iSystemIterator->second->EntityDestroyed(entity);
				iSystemIterator->second->mEntitiesSet.erase(entity);
			}
		}


		EntityManager::GetInstance()->DeleteEntity(entity);
		ComponentManager::GetInstance()->EntityDestroyed(entity);
	}

	void SystemManager::UpdateSignatures(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			// Get the name and system from mSystems
			auto const& name = pair.first;
			auto const& system = pair.second;
			// Get the system's signature
			auto const& systemSignature = mSystemSignatures[name];

			// Check if the entities signature matches the system's
			if ((entitySignature & systemSignature) == systemSignature) // bit comparison 
			{
				// Just incase it adds more than once, dont let it lmao
				if (system->mEntitiesSet.count(entity) == 0)
				{
					system->mEntitiesSet.insert(entity);
					system->EntityAdded(entity);
				}
			}
			else
			{
				// erase it from the system's signature if it exist within the entities set
				if (system->mEntitiesSet.find(entity) != system->mEntitiesSet.end())
				{
					system->mEntitiesSet.erase(entity);
					system->EntityRemoved(entity);
				}
			}
		}
	}

	void SystemManager::BroadcastMessage(Message* msg)
	{
		for (auto const& pair : mSystems)
		{
			if (msg->mMsgType != MSG_NONE)
			{
				pair.second->ReceiveMessage(msg);
			}
		}
	}

	void SystemManager::ChangeScene(std::string sceneName)
	{
		//GetSystem<SceneSystem>();
		GetSystem<SceneSystem>()->ChangeScene(sceneName);
	}

}
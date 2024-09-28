#include "pch.h"
#include "ECS/SystemManager.h"

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
				system->mEntitiesSet.insert(entity);
			}
			else
			{
				// erase it from the system's signature if it exist within the entities set
				if (system->mEntitiesSet.find(entity) != system->mEntitiesSet.end())
				{
					system->mEntitiesSet.erase(entity);
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

}
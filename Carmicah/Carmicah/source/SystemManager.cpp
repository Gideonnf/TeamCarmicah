#include "pch.h"
#include "SystemManager.h"

template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
	// Get the name of the system
	const char* sysName = typeid(T).name();

	auto newSystem = std::make_shared<T>();
	mSystems.insert({ sysName, newSystem });
	return newSystem;
}

template<typename T>
void SystemManager::SetSignature(Signature sig)
{
	// Get the name of the system
	const char* sysName = typeid(T).name();

	mSystemSignatures.insert({ sysName, sig });
}

void SystemManager::EntityDestroyed(Entity entity)
{
	std::unordered_map<const char*, std::shared_ptr<BaseSystem>>::iterator iSystemIterator = mSystems.begin();
	for (; iSystemIterator != mSystems.end(); ++iSystemIterator)
	{
		iSystemIterator->second->mEntitiesSet.erase(entity);
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

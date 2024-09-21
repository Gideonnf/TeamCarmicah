#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "BaseSystem.h"
#include <unordered_map>
#include <memory>
#include "Singleton.h"

namespace Carmicah
{
	class SystemManager : public Singleton<SystemManager>
	{
	private:
		std::unordered_map<std::string, Signature> mSystemSignatures{};
		std::unordered_map<std::string, std::shared_ptr<BaseSystem>> mSystems{};

		//std::unordered_map< std::shared_ptr<BaseSystem>, std::shared_ptr<BaseSystem>> ObserverMap;
		//std::unordered_map< std::shared_ptr<BaseSystem>, std::shared_ptr<BaseSystem>> ObserveredMap;


	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			// Get the name of the system
			std::string sysName = typeid(T).name();

			auto system = std::make_shared<T>();
			mSystems.insert({ sysName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature sig)
		{
			// Get the name of the system
			std::string sysName = typeid(T).name();

			mSystemSignatures.insert({ sysName, sig });
		}

		template<typename T>
		void SetSignature(std::vector<const char*> componentList)
		{
			// Get the name of the system
			std::string sysName = typeid(T).name();

			Signature signature;
			// Loop through the vector
			for (auto const& component : componentList)
			{
				mSystems[sysName]->componentNames.push_back(component);
				// Set the signature for all the component it has
				signature.set(ComponentManager::GetInstance()->GetComponentID(component), true);
			}

			mSystems[sysName]->mSignature = signature;
			mSystemSignatures.insert({ sysName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			std::unordered_map<std::string, std::shared_ptr<BaseSystem>>::iterator iSystemIterator = mSystems.begin();
			for (; iSystemIterator != mSystems.end(); ++iSystemIterator)
			{
				iSystemIterator->second->EntityDestroyed(entity);
				iSystemIterator->second->mEntitiesSet.erase(entity);
			}
		}

		void UpdateSignatures(Entity entity, Signature entitySignature)
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

		void BroadcastMessage(Message* msg)
		{
			for (auto const& pair : mSystems)
			{
				if (msg->mMsgType != MSG_NONE)
				{
					pair.second->ReceiveMessage(msg);
				}
			}
		}
	};
}

#endif
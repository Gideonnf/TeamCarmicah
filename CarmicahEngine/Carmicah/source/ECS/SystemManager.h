#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "BaseSystem.h"
#include <unordered_map>
#include <memory>
#include "../Singleton.h"

namespace Carmicah
{
	class SystemManager : public Singleton<SystemManager>
	{
	private:
		std::unordered_map<std::string, Signature> mSystemSignatures{};
		std::unordered_map<std::string, std::shared_ptr<BaseSystem>> mSystems{};

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

		void EntityDestroyed(Entity entity);

		void UpdateSignatures(Entity entity, Signature entitySignature);

		void BroadcastMessage(Message* msg);

		void ChangeScene(std::string sceneName);
	
		template <typename T>
		std::shared_ptr<T> GetSystem()
		{
			std::string systemName = typeid(T).name();
			if (mSystems.count(systemName) != 0)
			{
				return std::static_pointer_cast<T>(mSystems[systemName]);
			}

			assert("System does not exist yet");
			return NULL;
		}
	};

#define SYSTEM SystemManager::GetInstance()
#define REGISTER_SYSTEM(system) SystemManager::GetInstance()->RegisterSystem<system>();
}

#endif
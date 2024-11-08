/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SystemManager.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A singleton manager class that controls all systems in the engine. Stores them in a map based on the string representation of the system.
				Also acts as a message dispatcher to systems that subscribe to specific systems to receive messages from them. Also updates all systems when
				an entity has a new component added to map them to their entitiesSet or if an entity was destroyed and needs to be removed

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
			CM_CORE_INFO("Making {} system", sysName);
			auto system = std::make_shared<T>();
			mSystems.insert({ sysName, system });
			return system;
		}

        /*!*************************************************************************
        \brief
        	Set the Signature object
        
        \tparam T 
        \param[in] sig
        	
        
        ***************************************************************************/
		template<typename T>
		void SetSignature(Signature sig)
		{
			// Get the name of the system
			std::string sysName = typeid(T).name();

			mSystemSignatures.insert({ sysName, sig });
		}

        /*!*************************************************************************
        \brief
        	Set the Signature object
        
        \tparam T 
        \param[in] componentList
        	
        
        ***************************************************************************/
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

        /*!*************************************************************************
        \brief
        	Called when entity is destroyed
        
        \param[in] entity
        	
        
        ***************************************************************************/
		void EntityDestroyed(Entity entity);

        /*!*************************************************************************
        \brief
        	Update entity's signature to all systems
        
        \param[in] entity
        	
        
        \param[in] entitySignature
        	
        
        ***************************************************************************/
		void UpdateSignatures(Entity entity, Signature entitySignature);

        /*!*************************************************************************
        \brief
        	Broad cast a message to all systems
        
        \param[in] msg
        	
        
        ***************************************************************************/
		void BroadcastMessage(Message* msg);

        /*!*************************************************************************
        \brief
        	called to change scene
        
        \param[in] sceneName
        	
        
        ***************************************************************************/
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
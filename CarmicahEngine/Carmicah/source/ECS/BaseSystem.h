/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseSystem.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			The base class for all systems. Holds a set of entities which stores entities that the system can interact with. Contains a signature
				that represents the components that the system uses for it. 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
#include <set>
#include "ECSTypes.h"
#include <chrono>
#include "../Messaging/Message.h"
#include "../Messaging/InputMessage.h"
#include <assert.h>

namespace Carmicah
{
	class BaseSystem
	{
	public:
		// contain a set that holds what entities it holds
		std::set<Entity> mEntitiesSet;
		// Holds which systems are interested in messages from this current system
		std::set<BaseSystem*> mSystemObservers;
		// Keep track of the system's signature
		Signature mSignature;

		void PrintEntities(std::string systemName)
		{
			std::cout << "Entities in "<< systemName << " : " << mEntitiesSet.size() << std::endl;
		}

		void BindSystem(BaseSystem* system)
		{
			mSystemObservers.insert(system);
		}

		// this a lil cancer tbh
		void BindSystem(std::shared_ptr<BaseSystem> system)
		{
			BindSystem(std::static_pointer_cast<BaseSystem>(system).get());
		}


        /*!*************************************************************************
        \brief
        	
        
        \param[in] id
        	
        
        ***************************************************************************/
		virtual void EntityDestroyed(Entity id) {};

		virtual void EntityAdded(Entity id) {};

		virtual void EntityRemoved(Entity id) {};

		virtual void ReceiveMessage(Message* msg) {};

		virtual void OnUpdate() {}; // New virtual method for actual update logic

		void Update() {
			const std::string systemName = typeid(*this).name();
			CarmicahTime::GetInstance()->StartSystemTimer(systemName);
			OnUpdate();
			CarmicahTime::GetInstance()->StopSystemTimer(systemName);
		}

		void SendSysMessage(Message* msg) 
		{
			for (auto const& system : mSystemObservers)
			{
				system->ReceiveMessage(msg);
			}
		};

		// Keep track of time spent within system
		std::chrono::milliseconds mMilliseconds{};
		// Holds a vector of what components it uses. TODO: Verify if its actually useful for anything
		std::vector<const char*> componentNames{};
	};

}

#endif
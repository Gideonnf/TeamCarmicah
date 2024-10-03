/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        BaseSystem.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       The base class for all systems. Holds a set of entities which stores entities that the system can interact with. Contains a signature
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

		virtual void EntityDestroyed(Entity id) {};

		virtual void ReceiveMessage(Message* msg) {};

		void SendMessage(Message* msg) 
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
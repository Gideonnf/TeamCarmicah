#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
#include <set>
#include <ECS/ECSTypes.h>
#include <chrono>

namespace Carmicah
{
	class BaseSystem
	{
	public:
		// contain a set that holds what entities it holds
		std::set<Entity> mEntitiesSet;
		// Keep track of the system's signature
		Signature mSignature;
		// Keep track of time spent within system
		std::chrono::milliseconds mMilliseconds;
		// Holds a vector of what components it uses. TODO: Verify if its actually useful for anything
		std::vector<const char*> componentNames;
	};

}

#endif
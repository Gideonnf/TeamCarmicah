#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H
#include <set>
#include <ECSTypes.h>

class BaseSystem
{
public:
	std::set<Entity> mEntitiesSet;
};

#endif
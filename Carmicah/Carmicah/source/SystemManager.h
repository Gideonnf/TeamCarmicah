#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "BaseSystem.h"
#include "ECSTypes.h"
#include <unordered_map>
#include <memory>
class SystemManager
{
private:
	std::unordered_map<const char*, Signature> mSystemSignatures;
	std::unordered_map<const char*, std::shared_ptr<BaseSystem>> mSystems;

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem();

	template<typename T>
	void SetSignature(Signature sig);

	void EntityDestroyed(Entity entity);

	void UpdateSignatures(Entity entity, Signature entitySignature);
};

#endif
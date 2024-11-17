#ifndef PREFAB_SYSTEM_H
#define PREFAB_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "../Components/Prefab.h"

namespace Carmicah
{
	class PrefabSystem : public BaseSystem
	{
	private:
		std::unordered_map<unsigned int, std::vector<Entity>> mPrefabMap;

	public:
		PrefabSystem();

		void Update();

		void ReceiveMessage(Message* msg) override;

	
	};
}


#endif

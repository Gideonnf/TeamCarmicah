#ifndef PREFAB_SYSTEM_H
#define PREFAB_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "../Components/Prefab.h"
#include "../ECS/GameObject.h"

namespace Carmicah
{
	class PrefabSystem : public BaseSystem
	{
	private:
		std::unordered_map<unsigned int, std::vector<Entity>> mPrefabMap;
		std::deque<unsigned int> mFreePrefabIDs;
	public:
		PrefabSystem();

		void Update();

		void ReceiveMessage(Message* msg) override;

		void AddPrefab(Prefab goPrefab);

		unsigned int NewPrefab();

		void SavePrefab(GameObject& go);

		template<typename T>
		void UpdateComponent(std::pair<const std::string, std::any>& component, Entity entityID)
		{
			std::string componentName = component.first;
			std::any componentData = component.second;

			if (componentName == typeid(T).name())
			{
				T prefabComponentData = std::any_cast<T>(componentData);
				T& entityComponentData = ComponentManager::GetInstance()->GetComponent<T>(entityID);

				// change the component data
				entityComponentData = prefabComponentData;
			}
		}
	};
}


#endif

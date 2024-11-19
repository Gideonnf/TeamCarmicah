#ifndef PREFAB_SYSTEM_H
#define PREFAB_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "../Components/Prefab.h"
#include "../ECS/GameObject.h"
#include "AssetTypes.h"

namespace Carmicah
{
	const unsigned int MAX_PREFABS = 100;

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

		void SavePrefab(Entity id);

		Prefab MakePrefab(GameObject& go);

		template<typename T>
		bool MakeAny(std::string componentName, GameObject& go, std::any& component)
		{
			if (componentName == typeid(T).name() && go.HasComponent<T>())
			{
				component = go.GetComponent<T>();
				return true;
			}
			return false;
		}

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

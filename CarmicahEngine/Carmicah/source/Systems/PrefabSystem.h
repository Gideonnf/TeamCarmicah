#ifndef PREFAB_SYSTEM_H
#define PREFAB_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "../Components/Prefab.h"
#include "../ECS/GameObject.h"
#include "AssetTypes.h"
#include "GOFactory.h"

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

			// Check if its the correct component to be modified
			// Check if it has the component incase the player removed it
			if (componentName == typeid(T).name() && ComponentManager::GetInstance()->HasComponent<T>(entityID))
			{
				T prefabComponentData = std::any_cast<T>(componentData);
				T& entityComponentData = ComponentManager::GetInstance()->GetComponent<T>(entityID);

				// change the component data
				entityComponentData = prefabComponentData;
			}
			// A new component
			else if (componentName == typeid(T).name() && !ComponentManager::GetInstance()->HasComponent<T>(entityID))
			{
				GameObject& go = gGOFactory->FetchGO(entityID);
				// attach it to the go
				gGOFactory->AttachComponents(go, component);
			}
		}

		template<typename T>
		void RemoveComponent(std::string componentName, Entity entityID)
		{
			GameObject& go = gGOFactory->FetchGO(entityID);

			// if it has the component
			if (componentName == typeid(T).name() && go.HasComponent<T>())
			{
				// remove it
				go.RemoveComponent<T>();
			}
		}
	};
}


#endif

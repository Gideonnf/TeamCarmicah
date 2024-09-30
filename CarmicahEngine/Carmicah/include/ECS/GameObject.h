#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "ECSTypes.h"
#include "SystemManager.h"

namespace Carmicah
{

	class GameObject
	{
	private:
		Entity mID{};
		std::string mName{};
	public:
		//Game object factory is the only class allowed to
	//create and destroy game objects.
		friend class GOFactory;

		GameObject();

		GameObject(const Entity& id);

		~GameObject();

		void Destroy();

		Entity GetID() const;

		void SetID(const Entity& id);

		std::string GetName() const;

		void SetName(const std::string& name);

		template<typename T>
		void AddComponent(T Component)
		{
			ComponentManager::GetInstance()->AddComponent<T>(mID, Component);
			EntityManager::GetInstance()->AddComponent<T>(mID);

		}

		template<typename T>
		void AddComponent()
		{
			ComponentManager::GetInstance()->AddComponent<T>(mID);
			EntityManager::GetInstance()->AddComponent<T>(mID);

		}

		template<typename T>
		void RemoveComponent()
		{
			ComponentManager::GetInstance()->RemoveComponent<T>(mID);
			//gGOFactory->
			EntityManager::GetInstance()->RemoveComponent<T>(mID);

		}

		template <typename T>
		bool HasComponent()
		{
			return ComponentManager::GetInstance()->HasComponent<T>(mID);
		}

		template<typename T>
		T& GetComponent()
		{
			return ComponentManager::GetInstance()->GetComponent<T>(mID);
		}
	};
}


#endif
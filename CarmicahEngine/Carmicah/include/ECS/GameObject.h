#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "ECSTypes.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace Carmicah
{

	class GameObject
	{
	private:
		Entity mID;
		std::string mName;
	public:
		GameObject()
		{
			// default construct the name based on "GameObject" + id
			mID = EntityManager::GetInstance()->CreateEntity("GameObject");
		}
		GameObject(const Entity& id)
		{
			mID = id;
		}

		~GameObject() {}

		void Destroy()
		{
			EntityManager::GetInstance()->DeleteEntity(mID);
			ComponentManager::GetInstance()->EntityDestroyed(mID);
			SystemManager::GetInstance()->EntityDestroyed(mID);
		}

		Entity GetID()
		{
			return mID;
		}

		void SetID(const Entity& id)
		{
			mID = id;
		}

		std::string GetName()
		{
			return mName;
		}

		void SetName(const std::string& name)
		{
			mName = name;
		}

		template<typename T>
		void AddComponent(T Component)
		{
			ComponentManager::GetInstance()->AddComponent(mID, Component);
			auto entitySignature = EntityManager::GetInstance()->GetSignature(mID);
			// Set the component's signature pos within entity signature to true
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), true);
			// Set the new siganture of hte entity to show that it has this component now
			EntityManager::GetInstance()->SetSignature(mID, entitySignature);
			// Update the systems that the signature changed
			SystemManager::GetInstance()->UpdateSignatures(mID, entitySignature);
		}

		template<typename T>
		void RemoveComponent()
		{
			ComponentManager::GetInstance()->RemoveComponent<T>(mID);

			auto entitySignature = EntityManager::GetInstance()->GetSignature(mID);
			// Set the component's signature pos within entity signature to false
			entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), false);
			// Set the new siganture of hte entity to show that it does not have this component
			EntityManager::GetInstance()->SetSignature(mID, entitySignature);
			// Update the systems that the signature changed
			SystemManager::GetInstance()->UpdateSignatures(mID, entitySignature);
		}

		template<typename T>
		T& GetComponent()
		{
			return ComponentManager::GetInstance()->GetComponent<T>(mID);
		}
	};
}


#endif
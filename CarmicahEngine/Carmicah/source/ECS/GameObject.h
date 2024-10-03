/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        GameObject.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       Wraps the entity ID into a game object class that allows adding, removing and retrieving component data that relates to the entity's id.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "ECSTypes.h"
#include "SystemManager.h"
#include "log.h"

namespace Carmicah
{

	class GameObject
	{
	private:
		Entity mID{};
		std::string mName{};
		bool mActive;
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

		bool IsActive();

		void SetActive(bool active);

		template<typename T>
		void AddComponent(T Component)
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(mID))
			{
				CM_CORE_ERROR("Adding to an entity that does not exist");
				return;
			}

			ComponentManager::GetInstance()->AddComponent<T>(mID, Component);
			EntityManager::GetInstance()->AddComponent<T>(mID);
		}

		template<typename T>
		void AddComponent()
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(mID))
			{
				CM_CORE_ERROR("Adding to an entity that does not exist");
				return;
			}

			ComponentManager::GetInstance()->AddComponent<T>(mID);
			EntityManager::GetInstance()->AddComponent<T>(mID);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(mID))
			{
				CM_CORE_ERROR("Revmoing from an entity that does not exist");
				return;
			}

			ComponentManager::GetInstance()->RemoveComponent<T>(mID);
			//gGOFactory->
			EntityManager::GetInstance()->RemoveComponent<T>(mID);

		}

		template <typename T>
		bool HasComponent()
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(mID))
			{
				CM_CORE_ERROR("Entity does not exist");
				return false;
			}

			return ComponentManager::GetInstance()->HasComponent<T>(mID);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(mID))
			{
				assert("Getting component when Entity does not exist");
				CM_CORE_ERROR("Entity does not exist");
				//return T{};
			}

			return ComponentManager::GetInstance()->GetComponent<T>(mID);
		}
	};
}


#endif
/* File Documentation -----------------------------------------------------------------------------
file:           PrefabSystem.cpp

author:			Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file implements the PrefabSystem class, which manages the creation, saving,
				and modification of prefabs in the game engine. Prefabs are reusable templates for
				game objects. The system tracks prefabs, updates entities using prefabs, and ensures
				synchronization between prefab definitions and instantiated entities. It supports
				recursive child prefab handling and component updates or removals.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#include <pch.h>
#include "PrefabSystem.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/EntityManager.h"
#include "AssetManager.h"
#include "SerializerSystem.h"

namespace Carmicah
{
	PrefabSystem::PrefabSystem()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<PrefabData>());
		SystemManager::GetInstance()->SetSignature<PrefabSystem>(mSignature);
		for (unsigned int i = 1; i < MAX_PREFABS; ++i)
		{
			mFreePrefabIDs.push_back(i);
		}
	}

	void PrefabSystem::Update()
	{

	}

	void PrefabSystem::EntityDestroyed(Entity id)
	{
		PrefabData prefabData = ComponentManager::GetInstance()->GetComponent<PrefabData>(id);
		for (auto it = mPrefabMap[prefabData.mPrefabRef].begin(); it != mPrefabMap[prefabData.mPrefabRef].end(); ++it)
		{
			if (*it == id)
			{
				mPrefabMap[prefabData.mPrefabRef].erase(it);
				break;
			}
		}
	}

	void PrefabSystem::EntityRemoved(Entity id)
	{
		PrefabData prefabData = ComponentManager::GetInstance()->GetComponent<PrefabData>(id);
		for (auto it = mPrefabMap[prefabData.mPrefabRef].begin(); it != mPrefabMap[prefabData.mPrefabRef].end(); ++it)
		{
			if (*it == id)
			{
				mPrefabMap[prefabData.mPrefabRef].erase(it);
				break;
			}
		}
	}
	
	void PrefabSystem::AddPrefab(Prefab goPrefab)
	{
		if (goPrefab.mPrefabID == mFreePrefabIDs.front())
		{
			mFreePrefabIDs.pop_front();
		}
		else
		{
			for (auto it = mFreePrefabIDs.begin(); it != mFreePrefabIDs.end(); ++it)
			{
				if (*it == goPrefab.mPrefabID)
				{
					mFreePrefabIDs.erase(it);
					break;
				}
			}
		}
	}

	unsigned int PrefabSystem::NewPrefab()
	{
		unsigned int newID = mFreePrefabIDs.front();
		mFreePrefabIDs.pop_front();
		return newID;
	}

	void PrefabSystem::SavePrefab(Entity id)
	{
		GameObject& go = gGOFactory->FetchGO(id);

		// Check if prefab exist already. Don't want to recreate prefabs if not needed
		if (AssetManager::GetInstance()->AssetExist<Prefab>(go.GetName()))
		{
			return;
		}
		// Write a new prefab here
		Prefab newPrefab = MakePrefab(go);
		//Add to asset manager's prefab list
		AssetManager::GetInstance()->AddAsset<Prefab>(newPrefab.mName, newPrefab);
		// Make a file for it
		SerializerSystem::GetInstance()->SerializePrefab(newPrefab);
	}

	Prefab PrefabSystem::MakePrefab(GameObject& go)
	{
		Prefab newPrefab;
		newPrefab.mName = go.GetName();

		newPrefab.mPrefabID = NewPrefab();

		// Create the mComponent list of the prefab
		ComponentManager::GetInstance()->ForEachComponent([this, &newPrefab, &go](std::string componentName) {

			std::any prefabComponent;
			if (MakeAny<Transform>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<UITransform>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Collider2D>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Animation>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Renderer>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<RigidBody>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<TextRenderer>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<ParticleEmitter>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Script>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Button>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<Sound>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<PrefabData>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			else if (MakeAny<StateMachine>(componentName, go, prefabComponent))
			{
				newPrefab.mComponents.insert({ componentName, prefabComponent });
			}
			}, EntityManager::GetInstance()->GetSignature(go.GetID()));

		if (go.HasComponent<Transform>())
		{
			if (go.GetComponent<Transform>().children.size() > 0)
			{
				// Recursively go through all children GOs
				// if that child has children then itll go in to ensure all children and children of children is read properly
				for (auto entity : go.GetComponent<Transform>().children)
				{
					GameObject& childGO = gGOFactory->FetchGO(entity);
					newPrefab.childList.push_back(MakePrefab(childGO));
				}
			}
		}
		else if (go.HasComponent<UITransform>())
		{
			if (go.GetComponent<UITransform>().children.size() > 0)
			{
				for (auto entity : go.GetComponent<UITransform>().children)
				{
					GameObject& childGO = gGOFactory->FetchGO(entity);
					newPrefab.childList.push_back(MakePrefab(childGO));
				}
			}
		}

		// if it doesnt have prefab component, then add it and make it a prefab
		if (!go.HasComponent<PrefabData>())
		{
			go.AddComponent<PrefabData>();
			go.GetComponent<PrefabData>().mPrefabRef = newPrefab.mPrefabID;
			mPrefabMap[newPrefab.mPrefabID].push_back(go.GetID());
		}

		return newPrefab;
	}
	
	void PrefabSystem::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_NEWPREFABGO)
		{
			auto casted_msg = dynamic_cast<NewPrefabGOMsg*>(msg);
			// Add the entity id to the map
			mPrefabMap[casted_msg->prefabID].push_back(casted_msg->entityID);
		}

		// If a prefab was modified
		if (msg->mMsgType == MSG_MODIFYPREFAB)
		{
			// loop through to see which entities have to be updated
			auto casted_msg = dynamic_cast<ModifyPrefabMsg*>(msg);

			// If theres entities that uses this prefab
			if (mPrefabMap.count(casted_msg->prefabRef.mPrefabID) > 0)
			{
				// Loop through the entities that are made with the prefab
				for (auto entity : mPrefabMap[casted_msg->prefabRef.mPrefabID])
				{
					PrefabData& prefData = ComponentManager::GetInstance()->GetComponent<PrefabData>(entity);
					// loop through the components that it has
					for (auto& component : casted_msg->prefabRef.mComponents)
					{

						// If the component was modified by the user, then skip that component
						if (std::find(prefData.mComponentsModified.begin(), prefData.mComponentsModified.end(), component.first) != prefData.mComponentsModified.end())
						{
							continue;
						}
						// Dont check for transform
						// test with these components first
						UpdateComponent<Script>(component, entity);
						UpdateComponent<TextRenderer>(component, entity);
						UpdateComponent<RigidBody>(component, entity);
						UpdateComponent<Collider2D>(component, entity);
						UpdateComponent<Renderer>(component, entity);
						UpdateComponent<Animation>(component, entity);

					}

					// Loop through components that have been removed
					for (auto& componentName : casted_msg->prefabRef.mDeletedComponents)
					{
						RemoveComponent<Script>(componentName, entity);
						RemoveComponent<TextRenderer>(componentName, entity);
						RemoveComponent<RigidBody>(componentName, entity);
						RemoveComponent<Collider2D>(componentName, entity);
						RemoveComponent<Renderer>(componentName, entity);
						RemoveComponent<Animation>(componentName, entity);
					}

					// clear after checking
					casted_msg->prefabRef.mDeletedComponents.clear();
				}
			}
			//CM_CORE_INFO(std::to_string(casted_msg->mID));

			// Loop through which entities are using this prefab
			//for (auto it : casted_msg->prefabRef.entityWatcher)
			//{

			//}
		}
	
		if (msg->mMsgType == MSG_NEWPREFAB)
		{
			auto casted_msg = dynamic_cast<NewPrefabMsg*>(msg);

			SavePrefab(casted_msg->goEntity);
		}
	}

}
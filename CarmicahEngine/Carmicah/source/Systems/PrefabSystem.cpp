#include <pch.h>
#include "PrefabSystem.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"

namespace Carmicah
{
	PrefabSystem::PrefabSystem()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<PrefabData>());
		SystemManager::GetInstance()->SetSignature<PrefabSystem>(mSignature);
	}

	void PrefabSystem::Update()
	{

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

	void PrefabSystem::SavePrefab(GameObject& go)
	{
		// Write a new prefab here
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
					// loop through the components that it has
					for (auto& component : casted_msg->prefabRef.mComponents)
					{
						// Dont check for transform
						// test with these components first
						UpdateComponent<Script>(component, entity);
						UpdateComponent<TextRenderer>(component, entity);
						UpdateComponent<RigidBody>(component, entity);

					}
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

			SavePrefab(casted_msg->goPrefab);
		}
	}

}
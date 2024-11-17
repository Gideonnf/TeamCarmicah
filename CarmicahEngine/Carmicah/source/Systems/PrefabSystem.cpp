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
	
	void PrefabSystem::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_NEWPREFAB)
		{
			auto casted_msg = dynamic_cast<NewPrefabMsg*>(msg);
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

				}
			}
			//CM_CORE_INFO(std::to_string(casted_msg->mID));

			// Loop through which entities are using this prefab
			//for (auto it : casted_msg->prefabRef.entityWatcher)
			//{

			//}
		}
	}

}
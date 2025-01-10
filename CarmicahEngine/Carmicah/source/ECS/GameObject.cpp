/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:     GameObject.cpp

 author:   Gideon Francis (100%)

 email:    g.francis@digipen.edu

 brief:    Wraps the entity ID into a game object class that allows adding, removing and retrieving component data that relates to the entity's id.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#include "pch.h"
#include "GameObject.h"
#include "Systems/GOFactory.h"

namespace Carmicah
{
	GameObject::GameObject() : mActive(true)
	{

	}

	GameObject::GameObject(const Entity& id) : mActive(true)
	{
		mID = id;
	}

	GameObject::~GameObject() {}

	void GameObject::Destroy()
	{
		gGOFactory->Destroy(mID);
		//SystemManager::GetInstance()->EntityDestroyed(mID);
		//factoryRef->DestroyGameObject(mID);
		//gGOFactory->DestroyGameObject(mID);
	}

	Entity GameObject::GetID() const
	{
		return mID;
	}

	void GameObject::SetID(const Entity& id)
	{
		mID = id;
	}

	std::string GameObject::GetName() const
	{
		return mName;
	}

	void GameObject::SetName(const std::string& name)
	{
		mName = name; // dont use this function

	}

	bool GameObject::IsActive()
	{
		return mActive;
	}

	void GameObject::SetActive(bool active)
	{
		mActive = active;
	}

	bool GameObject::SetParent(Entity parentID)
	{
		
		// Check if the parent is the same (i.e accidentally dragging it back onto the same parent
		if (HasComponent<Transform>() && GetComponent<Transform>().parent == parentID)
			return false;

		if (HasComponent<UITransform>() && GetComponent<UITransform>().parent == parentID)
			return false;


		// If not then update the old and new parents
		gGOFactory->UpdateParent(mID, parentID);

		return true;
	}

	bool GameObject::SetParent(GameObject parentObj)
	{

		auto parentCheck = [](GameObject* currentGO, GameObject* parentGO) -> bool
			{
				auto mIDtoGOMap = gGOFactory->GetMIDToGO();
				while(parentGO)
				{
					if (parentGO->GetID() == currentGO->GetID())
					{
						return true;
					}

					if (parentGO->HasComponent<Transform>())
					{
						Entity parentID = parentGO->GetComponent<Transform>().parent;

						if (parentID == 0)
							break;
						auto it = mIDtoGOMap.find(parentID);
						if (it != mIDtoGOMap.end())
						{
							parentGO = &it->second;
						}
						else //No more parent
						{
							break;
						}
					}
					else if (parentGO->HasComponent<UITransform>())
					{
						Entity parentID = parentGO->GetComponent<UITransform>().parent;

						if (parentID == 0)
						{
							break;
						}

						auto it = mIDtoGOMap.find(parentID);
						if (it != mIDtoGOMap.end())
						{
							parentGO = &it->second;
						}
						else //No more parent
						{
							break;
						}
					}
				}
				return false;
			};

		if (parentCheck(this, &parentObj))
		{
			CM_CORE_ERROR("Trying to child into children!");
			return false;
		}
		
		// Check if the parent is the same (i.e accidentally dragging it back onto the same parent)
		if (HasComponent<Transform>() && GetComponent<Transform>().parent == parentObj.mID)
			return false;


		if (HasComponent<UITransform>() && GetComponent<UITransform>().parent == parentObj.mID)
			return false;

		//Trying to parent the GO into a child object (TODO: Grandchildren and onwards is an issue)
		if (parentObj.HasComponent<Transform>() && parentObj.GetComponent<Transform>().parent == this->GetID())
		{
			return false;
		}

		if (parentObj.HasComponent<UITransform>() && parentObj.GetComponent<UITransform>().parent == this->GetID())
		{
			return false;
		}

		// If not then update the old and new parents
		gGOFactory->UpdateParent(mID, parentObj.mID);

		return true;
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        GameObject.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       Wraps the entity ID into a game object class that allows adding, removing and retrieving component data that relates to the entity's id.

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
		mName = name;
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
		// Check if the parent is the same (i.e accidentally dragging it back onto the same parent
		if (GetComponent<Transform>().parent == parentObj.mID)
			return false;

		// If not then update the old and new parents
		gGOFactory->UpdateParent(mID, parentObj.mID);

		return true;
	}
}
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
}
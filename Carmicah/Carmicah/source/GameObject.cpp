#include "pch.h"
#include "GameObject.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

GameObject::GameObject()
{
	mID = EntityManager::GetInstance()->CreateEntity();
}

GameObject::GameObject(const Entity& id)
{
	mID = id;
}

GameObject::~GameObject()
{

}

void GameObject::Destroy()
{
	EntityManager::GetInstance()->DeleteEntity(mID);
	ComponentManager::GetInstance()->EntityDestroyed(mID);
	SystemManager::GetInstance()->EntityDestroyed(mID);
}

Entity GameObject::GetID()
{
	return mID;
}

void GameObject::SetID(const Entity& id)
{
	mID = id;
}

std::string GameObject::GetName()
{
	return mName;
}

void GameObject::SetName(const std::string& name)
{
	mName = name;
}

template<typename T>
void GameObject::AddComponent(const T& component)
{
	ComponentManager::GetInstance()->AddComponent(mID, component);
	auto entitySignature = EntityManager::GetInstance()->GetSignature(mID);
	// Set the component's signature pos within entity signature to true
	entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), true);
	// Set the new siganture of hte entity to show that it has this component now
	EntityManager::GetInstance()->SetSignature(mID, entitySignature);
	// Update the systems that the signature changed
	SystemManager::GetInstance()->UpdateSignatures(mID, entitySignature);
}

template<typename T>
void GameObject::RemoveComponent(const T& component)
{
	ComponentManager::GetInstance()->AddComponent(mID, component);
	auto entitySignature = EntityManager::GetInstance()->GetSignature(mID);
	// Set the component's signature pos within entity signature to false
	entitySignature.set(ComponentManager::GetInstance()->GetComponentID<T>(), false);
	// Set the new siganture of hte entity to show that it does not have this component
	EntityManager::GetInstance()->SetSignature(mID, entitySignature);
	// Update the systems that the signature changed
	SystemManager::GetInstance()->UpdateSignatures(mID, entitySignature);
}

template<typename T>
T& GameObject::GetComponent()
{
	return ComponentManager::GetInstance()->GetComponent<T>(mID);
}


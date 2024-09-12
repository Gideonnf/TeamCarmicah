#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"

template <typename T>
void ComponentManager::RegisterComponent()
{
	// Get the name of the type that was passed in for storing in the map
	const char* name = typeid(T).name();

	// add it to the component type map
	m_ComponentTypes.insert({ name, m_NextID });
	// Increment the next id var
	m_NextID++;

	//Insert into the component map
	m_ComponentMap.insert({ name, std::make_shared<Component<T>>() });
}

template<typename T>
ComponentID ComponentManager::GetComponentID()
{

}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{

}

template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{

}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{

}

void ComponentManager::EntityDestroyed(Entity entity)
{

}

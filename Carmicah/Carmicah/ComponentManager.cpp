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
	// Get the name of the component using typeid
	const char* componentName = typeid(T).name();

	// Check if it exist in the map
	if (m_ComponentTypes.find(componentName) != m_ComponentTypes.end())
	{
		return m_ComponentTypes[componentName]
	}

	return 0;
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{
	if (GetComponent<T>() != NULL)
	{
		GetComponent<T>()->AddComponent(entity, component);
	}
}

template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	if (GetComponent<T>() != NULL)
	{
		GetComponent<T>()->RemoveComponent(entity);
	}

}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{
	if (GetComponent<T>() != NULL)
	{
		return GetComponent<T>()->GetComponentData(entity);
	}
}

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (std::unordered_map<const char*, std::shared_ptr<IComponent>>::iterator i_mapIterator = m_ComponentMap.begin();
		i_mapIterator != m_ComponentMap.end(); ++i_mapIterator)
	{
		i_mapIterator->second->EntityDestroyed(entity);
	}
}

template<typename T>
std::shared_ptr<Component<T>> ComponentManager::GetComponent()
{
	// Get the name of the component using typeid
	const char* componentName = typeid(T).name();
	// Check if it exist in the map
	if (m_ComponentTypes.find(componentName) != m_ComponentTypes.end())
	{
		// if it does then retrieve the component
		return std::static_pointer_cast<Component<T>>(m_ComponentMap[componentName]);
	}
	
	// If it reaches here then itll flag an error
	return NULL;
}

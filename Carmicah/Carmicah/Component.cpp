#include "pch.h"
#include "Entity.h"
#include "Component.h"

template <typename T>
void Component<T>::AddComponent(Entity entity, T component)
{
	// use current active size to get the next component id
	unsigned int componentIndex = m_Size;

	// Set the map variables
	m_EntityToComponent[entity] = componentIndex;
	m_ComponentToEntity[componentIndex] = entity;

	// Store the component data into the array
	m_ComponentArray[componentIndex] = component;

	// increment size to ensure its always the latest number
	m_Size++;
}

/*
In order to keep the data packed in memory with valid data, shift the last valid element in the component array into
the deleted element in the component array to ensure that it only reads valid element data
*/

template <typename T>
void Component<T>::RemoveComponent(Entity entity)
{
	// Get the last active index and the entity to delete index
	unsigned int lastValidIndex = m_Size - 1;
	unsigned int entityIndex = m_EntityToComponent[entity];
	// Swap the values in the component array
	m_ComponentArray[entityIndex] = m_ComponentArray[lastValidIndex];

	// Update the maps
	Entity lastValidEntity = m_ComponentToEntity[lastValidIndex];
	m_EntityToComponent[lastValidEntity] = entityIndex; // Change the index of the last active entity to the deleted index
	m_ComponentToEntity[entityIndex] = lastValidEntity; // Change the deleted entity to the last active entity

	// erase the old ones from the maps
	m_EntityToComponent.erase(entity);
	// Last valid index now holds a copy of the last valid entity data after shifting, so we have to erase it
	m_ComponentToEntity.erase(lastValidIndex);

	// Decrement size as the last valid index is now free to be used
	m_Size--;
}

template <typename T>
T& Component<T>::GetComponentData(Entity entity)
{
	return m_ComponentArray[m_EntityToComponent[entity]];
}

template <typename T>
void Component<T>::EntityDestroyed(Entity entity)
{
	// If the entity exist within this component
	if (m_EntityToComponent.find(entity) != m_EntityToComponent.end())
	{
		// Remove the data
		RemoveComponent(entity);
	}
}

#ifndef COMPONENT_H
#define COMPONENT_H

#include "ECSTypes.h"
// Interface for components so that component manager can notify components if an entity is destroyed
class IComponent
{
public:
	virtual ~IComponent() {};
	virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class Component : public IComponent
{
private:
	// Initialize an array of components that matches the max number of entities
	// So each entity will match it's own component data
	std::array<T, MAX_ENTITIES> m_ComponentArray;

	// Map for an entity ID to a component
	std::unordered_map<Entity, unsigned int> m_EntityToComponent;

	// Map a component to an entity
	std::unordered_map<unsigned int, Entity> m_ComponentToEntity;

	// Keep track of the active components
	unsigned int m_Size;

	// NOTE:
	/*
		Possible to initialize a queue based on the number of components
		when an entity wants to add a component, itll take the first component available
		if an entity 
	*/

public:
	void AddComponent(Entity entity, T component)
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

	void RemoveComponent(Entity entity)
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


	T& GetComponentData(Entity entity)
	{
		return m_ComponentArray[m_EntityToComponent[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		// If the entity exist within this component
		if (m_EntityToComponent.find(entity) != m_EntityToComponent.end())
		{
			// Remove the data
			RemoveComponent(entity);
		}
	}

};

#endif

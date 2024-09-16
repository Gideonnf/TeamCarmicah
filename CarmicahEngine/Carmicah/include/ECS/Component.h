#ifndef COMPONENT_H
#define COMPONENT_H

#include "ECSTypes.h"
#include <unordered_map>
#include <array>
#include <vector>
namespace Carmicah
{
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
		//std::array<T, MAX_ENTITIES> m_ComponentArray;
		std::vector<T> m_ComponentArray;

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
		Component()
		{
			// Reserve memory for the component array to allow for continous memory
			m_ComponentArray.reserve(MAX_ENTITIES);
		}

		void InsertComponentData(Entity entity, T component)
		{
			// TODO: CHeck if its inserting over the limit

			// use current active size to get the next component id
			unsigned int componentIndex = m_ComponentArray.size();

			// Set the map variables
			m_EntityToComponent[entity] = componentIndex;
			m_ComponentToEntity[componentIndex] = entity;

			// Store the component data into the array
			//m_ComponentArray[componentIndex] = component;
			m_ComponentArray.emplace_back(component);

			// increment size to ensure its always the latest number
			//m_Size++;
		}

		void RemoveComponentData(Entity entity)
		{
			// Get the last active index and the entity to delete index
			unsigned int lastValidIndex = m_ComponentArray.size() - 1;
			// get the component index related to the entity thats being removed
			unsigned int componentIndex = m_EntityToComponent[entity];

			
			// Swap component data from within component array
			// Swap the values in the component array
			m_ComponentArray[componentIndex] = m_ComponentArray[lastValidIndex];
			// pop back the last element after swapping to maintain size
			m_ComponentArray.pop_back();

			// Update the maps
			// Get the entity related to the last active component
			Entity lastValidEntity = m_ComponentToEntity[lastValidIndex];
			// Map the last valid entity to the component index as it holds the data of the last entity after swapping above
			m_EntityToComponent[lastValidEntity] = componentIndex;
			// Change the component index to contain the last valid entity  as the old one is deleted now
			m_ComponentToEntity[componentIndex] = lastValidEntity;

			// erase the old ones from the maps
			m_EntityToComponent.erase(entity);
			// Last valid index now holds a copy of the last valid entity data after shifting, so we have to erase it
			m_ComponentToEntity.erase(lastValidIndex);

			// Decrement size as the last valid index is now free to be used
			//m_Size--;
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
				RemoveComponentData(entity);
			}
		}
	};
}

#endif

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h" // ECStypes in component.h
#include <unordered_map>
#include <memory>

#include "Singleton.h"

namespace Carmicah
{
	class ComponentManager : public Singleton<ComponentManager>
	{
		// Look into whether it should be a singleton/extern it out as a global variable
	private:
		// Keep track of what is the component ID 
		std::unordered_map<std::string, ComponentID> m_ComponentTypes;

		// Keep track of the components by storing a shared pointer to the interface base class
		// Shared pointer so that it can keep track of every new component by using the same pointer
		// I also cant store it as a const char*, IComponent without using shared_ptr 
		std::unordered_map<std::string, std::shared_ptr<IComponent>> m_ComponentMap;

		// Everytime a new component is registered, it uses this ID and increment it for the next component
		// Use 0 as an error flag if trying to pull a component that doesn't exist
		ComponentID m_NextID = 1;
	public:

		template<typename T>
		void RegisterComponent()
		{
			// Get the name of the type that was passed in for storing in the map
			std::string name = typeid(T).name();

			// add it to the component type map
			m_ComponentTypes.insert({ name, m_NextID });
			//std::cout << "Component ID" << m_NextID << std::endl;
			// Increment the next id var
			m_NextID++;

			//Insert into the component map
			m_ComponentMap.insert({ name, std::make_shared<Component<T>>() });
		}

		template<typename T>
		ComponentID GetComponentID()
		{
			// Get the name of the component using typeid
			std::string componentName = typeid(T).name();

			// Check if it exist in the map
			if (m_ComponentTypes.find(componentName) != m_ComponentTypes.end())
			{
				return m_ComponentTypes[componentName];
			}

			return 0;
		}

		ComponentID GetComponentID(std::string componentName)
		{

			if (m_ComponentTypes.find(componentName) != m_ComponentTypes.end())
			{
				return m_ComponentTypes[componentName];
			}

			// Only used for checking string since getting typeid of a struct is struct <name>
			// If we want to pass in a string then we have to use string find to see if <name> is in struct <name>
			for (auto const& component : m_ComponentTypes)
			{
				if (component.first.find(componentName) != std::string::npos)
				{
					return component.second;
				}
			}

			return 0;
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			if (GetComponentArray<T>() != NULL)
			{
				GetComponentArray<T>()->InsertComponentData(entity, component);
			}
		}


		template<typename T>
		void RemoveComponent(Entity entity)
		{
			if (GetComponentArray<T>() != NULL)
			{
				GetComponentArray<T>()->RemoveComponentData(entity);
			}

		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// TODO: Assert error if it tries to get a component that it doesnt have

			return GetComponentArray<T>()->GetComponentData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			for (std::unordered_map<std::string, std::shared_ptr<IComponent>>::iterator i_mapIterator = m_ComponentMap.begin();
				i_mapIterator != m_ComponentMap.end(); ++i_mapIterator)
			{
				i_mapIterator->second->EntityDestroyed(entity);
			}
		}

		// Used to get the component for inserting in new entity data
		template<typename T>
		std::shared_ptr<Component<T>> GetComponentArray()
		{
			// Get the name of the component using typeid
			std::string componentName = typeid(T).name();
			// Check if it exist in the map
			if (m_ComponentTypes.find(componentName) != m_ComponentTypes.end())
			{
				// if it does then retrieve the component
				return std::static_pointer_cast<Component<T>>(m_ComponentMap[componentName]);
			}

			// If it reaches here then itll flag an error
			return NULL;
		}
	};

	static ComponentManager componentManager;

#pragma region Accessor Functions
	template <typename T>
	void RegisterComponent()
	{
		componentManager.RegisterComponent<T>();
	}

	// TODO: Try whether creating these accessor functions are better for accessing than using singleton
	// Finish up the rest tomorrow morning

#pragma endregion
}

#endif
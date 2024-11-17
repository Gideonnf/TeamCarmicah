/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ComponentManager.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A singleton manager to handle the creation and management of Components. Stores a map of a string representation to the component.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h" // ECStypes in component.h
#include <unordered_map>
#include <memory>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <any>
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"
#include "Components/RigidBody.h"
#include "Components/Script.h"
#include "Components/Button.h"
#include "Components/Prefab.h"
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

		//std::unordered_map<std::string, std::any> mComponentRegistry;
		//std::array<Signature, MAX_COMPONENTS> m_ComponentSignatures;

		// Everytime a new component is registered, it uses this ID and increment it for the next component
		// Use 0 as an error flag if trying to pull a component that doesn't exist
		ComponentID m_NextID = 1;
	public:

        /*!*************************************************************************
        \brief
        	Register component into the component manager that is type T
        
        \tparam T 
        ***************************************************************************/
		template<typename T>
		void RegisterComponent()
		{
			// Get the name of the type that was passed in for storing in the map
			std::string name = typeid(T).name();

			// if count is 1 this will fail
			assert(m_ComponentTypes.count(name) == 0 && "Component has been registered");

			// add it to the component type map
			m_ComponentTypes.insert({ name, m_NextID });

			m_NextID++;

			CM_CORE_INFO("Component Created with name" + name);

			//Insert into the component map
			m_ComponentMap.insert({ name, std::make_shared<Component<T>>() });

			// Create a lambda function that returns an instance of T 
			// not being used atm
			//mComponentRegistry.insert({ name, []() -> T {return T{}; } });
		}

        /*!*************************************************************************
        \brief
        	Get the Component ID from a template type T
        
        \tparam T 
        \return ComponentID
        	
        ***************************************************************************/
		template<typename T>
		ComponentID GetComponentID()
		{
			// Get the name of the component using typeid
			std::string componentName = typeid(T).name();

			// Check if it exist in the map
			assert(m_ComponentTypes.count(componentName) != 0 && "Component does not exist yet");

			return m_ComponentTypes[componentName];
		}

        /*!*************************************************************************
        \brief
        	Get the Component ID from a string
        
        \param[in] componentName
        	
        
        \return ComponentID
        	
        ***************************************************************************/
		ComponentID GetComponentID(const std::string& componentName)
		{

			if (m_ComponentTypes[componentName] != 0)
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

			assert("String does not match any registered components");
		}

        /*!*************************************************************************
        \brief
        	Add a component to the entity
        
        \tparam T - Type of component
        \param[in] entity - Entity to add to
        	
        
        \param[in] component - Component
        	
        
        ***************************************************************************/
		template<typename T>
		void AddComponent(const Entity& entity, T component)
		{
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}

        /*!*************************************************************************
        \brief
        	Add component to entity
        
        \tparam T  - Type of component
        \param[in] entity - Entity to attach component to
        	
        
        ***************************************************************************/
		template<typename T>
		void AddComponent(const Entity& entity)
		{
			T component{};
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}


        /*!*************************************************************************
        \brief
        	Remove component from an entity
        
        \tparam T 
        \param[in] entity
        	
        
        ***************************************************************************/
		template<typename T>
		void RemoveComponent(const Entity& entity)
		{
			GetComponentArray<T>()->RemoveComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Get the Component object
        
        \tparam T 
        \param[in] entity
        	
        
        \return T&
        	
        ***************************************************************************/
		template<typename T>
		T& GetComponent(const Entity& entity)
		{
			return GetComponentArray<T>()->GetComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Called when entities are destroyed
        
        \param[in] entity
        	
        
        ***************************************************************************/
		void EntityDestroyed(Entity entity);

        /*!*************************************************************************
        \brief
        	Called when cloning entity
        
        \param[in] entityToClone
        	Entity to clone
        
        \param[in] newEntity
        	Entity to clone into
        
        \param[in] entitySignature
        	Signature to copy
        
        ***************************************************************************/
		void CloneEntity(Entity entityToClone, Entity newEntity, Signature entitySignature);

        /*!*************************************************************************
        \brief
        	For looping through components of an entity
        
        \param[in] func
        	Function
        
        \param[in] entitySignature
        	Entity signature
        
        ***************************************************************************/
		void ForEachComponent(const std::function<void(const std::string&)>& func, const Signature& entitySignature);

        /*!*************************************************************************
        \brief
        	Serialize the components to an entity
        
        \param[in] entity
        	Entity to serialize from
        
        \param[in] entitySignature
        	Signature of the entity
        
        \param[in] writer
        	reference to the writer
        
        ***************************************************************************/
		void SerializeEntityComponents(const Entity& entity, const Signature& entitySignature, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);


		size_t GetComponentCount();

        /*!*************************************************************************
        \brief
        	Check if the entity has the component by calling the HasComponentData
        
        \tparam T 
        \param[in] entity
        	
        
        \return true
        	
        \return false
        	
        ***************************************************************************/
		template<typename T>
		bool HasComponent(Entity entity)
		{
			return GetComponentArray<T>()->HasComponentData(entity);
		}

        /*!*************************************************************************
        \brief
        	Get the Component Array object
		    Used to get the component for inserting in new entity data
        
       \template<typename T>
        	Type of the class
        ***************************************************************************/
		template<typename T>
		std::shared_ptr<Component<T>> GetComponentArray()
		{
			// Get the name of the component using typeid
			std::string componentName = typeid(T).name();
			// Check if it exist in the map
			if (m_ComponentTypes.count(componentName) != 0)
			{
				// if it does then retrieve the component
				return std::static_pointer_cast<Component<T>>(m_ComponentMap[componentName]);
			}

			// If it reaches here then itll flag an error
			assert(m_ComponentTypes.count(componentName) != 0 && "Component does not exist yet. ");
			return NULL;
		}

        /*!*************************************************************************
        \brief
           Templated so that I don't have to write 4 lines per if/else if statements for every component
           without RTTR, I have to manually check each component
        \return template<typename T>

        ***************************************************************************/
        template<typename T>
        bool DeserializeComponent(const rapidjson::Value& val, std::any& component)
        {
           // std::any component;
            const std::string& componentName = val["Component Name"].GetString();

            if (componentName == typeid(T).name())
            {
                T componentData{}; // Default initialize
                componentData.DeserializeComponent(val);
                component = componentData;
                return true;
                //component = std::any_cast<Transform>(component).DeserializeComponent(val);
            }
            return false;
        }

        /*!*************************************************************************
        \brief
           Deserialize components and return it as an std::any so that I can store it in prefab data
           as std::any
        ***************************************************************************/
		std::any DeserializePrefabComponent(const rapidjson::Value& val);



	};

#define COMPONENTSYSTEM ComponentManager::GetInstance()
#define REGISTER_COMPONENT(Component) ComponentManager::GetInstance()->RegisterComponent<Component>()
}

#endif
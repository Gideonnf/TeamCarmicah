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

		template<typename T>
		ComponentID GetComponentID()
		{
			// Get the name of the component using typeid
			std::string componentName = typeid(T).name();

			// Check if it exist in the map
			assert(m_ComponentTypes.count(componentName) != 0 && "Component does not exist yet");

			return m_ComponentTypes[componentName];
		}

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

		template<typename T>
		void AddComponent(const Entity& entity, T component)
		{
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}

		template<typename T>
		void AddComponent(const Entity& entity)
		{
			T component{};
			GetComponentArray<T>()->InsertComponentData(entity, component);
		}


		template<typename T>
		void RemoveComponent(const Entity& entity)
		{
			GetComponentArray<T>()->RemoveComponentData(entity);
		}

		template<typename T>
		T& GetComponent(const Entity& entity)
		{
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

		void CloneEntity(Entity entityToClone, Entity newEntity, Signature entitySignature)
		{
			//Signature entitySignature = EntityManager::GetInstance()->GetSignature(entityToClone);

			for (auto const& component : m_ComponentTypes)
			{
				Signature componentSignature;
				// Set that component signature to true
				componentSignature.set(component.second, true);
				// Check if the entity's signature has this component

				// It has that component
				if ((entitySignature & componentSignature) == componentSignature)
				{
					m_ComponentMap[component.first]->CloneComponentData(entityToClone, newEntity);
				}
			}

			// copy the entity's signature since it has the same components. nvm this will be done on entity manager side
			//EntityManager::GetInstance()->SetSignature(newEntity, entitySignature);
		}

		void ForEachComponent(const std::function<void(const std::string&)>& func, const Signature& entitySignature)
		{
			for (auto const& component : m_ComponentTypes)
			{
				Signature componentSignature;
				// Set that component signature to true
				componentSignature.set(component.second, true);
				// Check if the entity's signature has this component

				// It has that component
				if ((entitySignature & componentSignature) == componentSignature)
				{
					func(component.first);
				}
			}
		}

		void SerializeEntityComponents(const Entity& entity,const Signature& entitySignature, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
		{
			for (auto const& component : m_ComponentTypes)
			{
				Signature componentSignature;
				// Set that component signature to true
				componentSignature.set(component.second, true);
				// Check if the entity's signature has this component
				
				// It has that component
				if ((entitySignature & componentSignature) == componentSignature)
				{
					writer.StartObject();
					writer.String("Component Name");
					writer.String(component.first.c_str(), static_cast<rapidjson::SizeType>(component.first.length()));
					m_ComponentMap[component.first]->SerializeData(entity, writer);
					writer.EndObject();
				}
			}

		}

		size_t GetComponentCount()
		{
			return m_ComponentTypes.size();
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			return GetComponentArray<T>()->HasComponentData(entity);
		}

		// Used to get the component for inserting in new entity data
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
			assert(m_ComponentTypes.count(componentName) == 0 && "Component does not exist yet. ");
			return NULL;
		}

		std::any DeserializePrefabComponent(const rapidjson::Value& val)
		{
			const std::string& componentName = val["Component Name"].GetString();
			std::any component;
			if (componentName == typeid(Transform).name())
			{
				Transform componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//component = std::any_cast<Transform>(component).DeserializeComponent(val);
			}
			else if (componentName == typeid(Collider2D).name())
			{
				Collider2D componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//component = std::any_cast<Collider2D>(component).
			}
			else if (componentName == typeid(Animation).name())
			{
				Animation componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//std::any_cast<Animation>(component).DeserializeComponent(val);
			}
			else if (componentName == typeid(Renderer).name())
			{
				Renderer componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//std::any_cast<Renderer>(component).DeserializeComponent(val);
			}
			else if (componentName == typeid(RigidBody).name())
			{
				RigidBody componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//std::any_cast<RigidBody>(component).DeserializeComponent(val);
			}
			else if (componentName == typeid(UITransform).name())
			{
				UITransform componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//std::any_cast<UITransform>(component).DeserializeComponent(val);
			}
			else if (componentName == typeid(TextRenderer).name())
			{
				TextRenderer componentData{}; // Default initialize
				componentData.DeserializeComponent(val);
				component = componentData;

				//std::any_cast<TextRenderer>(component).DeserializeComponent(val);
			}
			else
			{
				// incase someone added a component and forgot to write here
				assert("Component does not have a deserialize function");
			}

			return component;
		}
	};

#define COMPONENTSYSTEM ComponentManager::GetInstance()
#define REGISTER_COMPONENT(Component) ComponentManager::GetInstance()->RegisterComponent<Component>()
}

#endif
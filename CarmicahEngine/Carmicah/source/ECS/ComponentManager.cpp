/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ComponentManager.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A singleton manager to handle the creation and management of Components. Stores a map of a string representation to the component.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "ComponentManager.h"

namespace Carmicah
{
	void ComponentManager::EntityDestroyed(Entity entity)
	{
		for (std::unordered_map<std::string, std::shared_ptr<IComponent>>::iterator i_mapIterator = m_ComponentMap.begin();
			i_mapIterator != m_ComponentMap.end(); ++i_mapIterator)
		{
			i_mapIterator->second->EntityDestroyed(entity);
		}
	}

	void ComponentManager::CloneEntity(Entity entityToClone, Entity newEntity, Signature entitySignature)
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

	void ComponentManager::ForEachComponent(const std::function<void(const std::string&)>& func, const Signature& entitySignature)
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

	void ComponentManager::SerializeEntityComponents(const Entity& entity, const Signature& entitySignature, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
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

	size_t ComponentManager::GetComponentCount()
	{
		return m_ComponentTypes.size();
	}

	std::any ComponentManager::DeserializePrefabComponent(const rapidjson::Value& val)
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
		else if (componentName == typeid(Script).name())
		{
			Script componentData{}; // Default initialize
			componentData.DeserializeComponent(val);
			component = componentData;

		}
		else
		{
			// incase someone added a component and forgot to write here
			assert("Component does not have a deserialize function");
		}

		return component;
	}

}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ComponentManager.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A singleton manager to handle the creation and management of Components. Stores a map of a string representation to the component.

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
		std::any component;
		if (DeserializeComponent<Transform>(val, component)) return component;
		else if (DeserializeComponent<Collider2D>(val, component)) return component;
		else if (DeserializeComponent<Animation>(val, component)) return component;
		else if (DeserializeComponent<Renderer>(val, component)) return component;
		else if (DeserializeComponent<RigidBody>(val, component)) return component;
		else if (DeserializeComponent<UITransform>(val, component)) return component;
		else if (DeserializeComponent<TextRenderer>(val, component)) return component;
		else if (DeserializeComponent<Script>(val, component)) return component;
		else if (DeserializeComponent<Button>(val, component)) return component;
		else if (DeserializeComponent<PrefabData>(val, component)) return component;
		else if (DeserializeComponent<Sound>(val, component)) return component;
		else
		{
			// incase someone added a component and forgot to write here
			assert("Component does not have a deserialize function");
		}

		return component;
	}

}
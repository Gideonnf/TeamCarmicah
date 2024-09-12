#pragma once

// Interface for components so that component manager can notify components if an entity is destroyed
class IComponent
{
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
	void AddComponent(Entity entity, T component);

	void RemoveComponent(Entity entity);

	T& GetComponentData(Entity entity);

	void EntityDestroyed(Entity entity) override;

};


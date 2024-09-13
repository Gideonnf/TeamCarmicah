#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
class ComponentManager
{
private:
	// Keep track of what is the component ID 
	std::unordered_map<const char*, ComponentID> m_ComponentTypes;

	// Keep track of the components by storing a shared pointer to the interface base class
	// Shared pointer so that it can keep track of every new component by using the same pointer

	// I also cant store it as a const char*, IComponent without using shared_ptr 
	std::unordered_map<const char*, std::shared_ptr<IComponent>> m_ComponentMap;

	// Everytime a new component is registered, it uses this ID and increment it for the next component
	// Use 0 as an error flag if trying to pull a component that doesn't exist
	ComponentID m_NextID;
public:

	template<typename T>
	void RegisterComponent();

	template<typename T>
	ComponentID GetComponentID();

	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	void EntityDestroyed(Entity entity);

	// Used to get the component for inserting in new entity data
	template<typename T>
	std::shared_ptr<Component<T>> GetComponent();

};

#endif
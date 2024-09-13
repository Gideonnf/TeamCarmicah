#pragma once



class EntityManager
{
private:
	// Keep track of the current queue of free entity IDs that can be used
	std::queue<Entity> m_FreeEntities;
	// Maps the entity signature to the entity ID
	std::array<EntitySignature, MAX_ENTITIES> m_EntitySignatures;
	// Keep track of the number of active entities
	unsigned int m_EntityCount;

public:
	EntityManager();

	Entity CreateEntity();

	void DeleteEntity(Entity entity);

	void SetSignature(Entity entity, EntitySignature entitySignature);
};


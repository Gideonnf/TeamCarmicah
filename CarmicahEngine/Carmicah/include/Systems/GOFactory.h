#ifndef GO_FACTORY_H
#define GO_FACTORY_H
#include "ECS/BaseSystem.h"
#include "ECS/GameObject.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "../Singleton.h"
#include <memory>

namespace Carmicah
{
	// Look into other ways that doesnt require singleton
	class GOFactory : public BaseSystem
	{
	private:
		// So that players can search for game objects by their name
		std::unordered_map<std::string, Entity> mNameToID;
		std::unordered_map<Entity, GameObject> mIDToGO;

		// Holds game objects that require deletion at the end of updating
		std::set<Entity> mDeleteList;

		//std::unique_ptr<EntityManager> mEntityManager;
		//std::unique_ptr<ComponentManager> mComponentManager;
		/*
			NOTE: Not sure whats the best implementation right now.
			1) Whether ot hold the entity and component manager within GOFactory 
			2) Create a pointer that hold it within its class and extend it into this system to be used
			3) Leave it as a singleton/Create it as a namespace global variable and create functions to use it
		*/
	public:
		GOFactory();
		~GOFactory();


#pragma region GameObject Functions
		// Default name for GOs
		GameObject CreateGO(std::string name = "GameObject");
		GameObject CloneGO(GameObject const& go);
		void CreateGO(GameObject);
		void EntityDestroyed(Entity) override;
		void DestroyAll();
		void UpdateDestroyed();
#pragma endregion

#pragma region Component Functions
		template<typename T>
		void CreateComponent();
#pragma endregion

	};

	extern GOFactory* gGOFactory;
}

#endif
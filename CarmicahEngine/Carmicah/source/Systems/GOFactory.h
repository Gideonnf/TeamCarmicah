/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        GOFactory.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A system to handle creation and deletion of game objects. Allows for cloning, prefab creation, Importing and exporting of game objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GO_FACTORY_H
#define GO_FACTORY_H
#include "ECS/BaseSystem.h"
#include "ECS/GameObject.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "../Singleton.h"
#include <memory>
#include "Systems/SerializerSystem.h"

namespace Carmicah
{
	struct SceneGO
	{
		std::string sceneName{};
		Entity sceneID = 0; // Default 0 for scenes
		std::set<Entity> children;
		//Transform sceneTransform{};
	};

	
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

#pragma region SceneData
		Entity mainCam{};
		SceneGO sceneGO;
#pragma endregion

#pragma region GameObject Functions
		// Default name for GOs
		GameObject CreateGO(std::string name = "GameObject");
		GameObject CloneGO(GameObject const& go);
		GameObject LoadGO(std::string name, Entity entityID);
		GameObject CreatePrefab(std::string prefab);
		void CreateSceneObject(std::string sceneName);
		void FetchGO(std::string GOName, GameObject& go);
		void AttachComponents(GameObject& obj, std::pair<std::string, std::any> component );
		void EntityDestroyed(Entity) override;
		void Destroy(Entity);
		void DestroyAll();
		void UpdateDestroyed();
		void UpdateGOName(GameObject& go, std::string newName); // TODO: Make a function to update the GO names		
		// To fix the issue of all gameobjects having the same name
		// Add a number to the back until it has a unique name
		std::string CreateGOName(std::string goName);

		/*!
			Used for updating the sceneGO if the entity's parent was changed so that scene hierarchy can update
		*/
		void UpdateParent(Entity entityID, Entity parentID);

#pragma endregion

		// FOR TESTING
		void ParentAllGO();

#pragma region Importing and Exporting
		void ForAllGO(const std::function<void(GameObject&)>& op);
		void ImportGO(const rapidjson::Value& go);
		void ExportGOs(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
		void ExportEntity(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer, Entity id);
		Entity ImportEntity(const rapidjson::Value& go, Entity parentID);
#pragma endregion

#pragma region Component Functions
		template<typename T>
		void CreateComponent();
#pragma endregion
		void ReceiveMessage(Message* msg) override;

		//void SendMessage(Message* msg) override;
	};

	extern GOFactory* gGOFactory;
}

#endif
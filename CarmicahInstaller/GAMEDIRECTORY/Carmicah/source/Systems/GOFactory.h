/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GOFactory.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A system to handle creation and deletion of game objects. Allows for cloning, prefab creation, Importing and exporting of game objects

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
	enum class TRANSFORMTYPE
	{
		NONE,
		TRANSFORM,
		UITRANSFORM
	};
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
		GameObject CreateGO(std::string name = "GameObject", TRANSFORMTYPE flag = TRANSFORMTYPE::TRANSFORM);
		GameObject CloneGO(GameObject const& go, Entity parentID = 0);
		GameObject LoadGO(std::string name, Entity entityID);
		GameObject CreatePrefab(std::string prefab);
		std::vector<Entity> GetEntitiesWithTag(std::string tagName);
		void CreatePrefabChild(Prefab& prefab, Entity parentID);
		void UpdatePrefabTransform(Entity entityID);
		void UpdatePrefabChildTransform(Entity entityID);

		void CreateSceneObject(std::string sceneName);
		void FetchGO(std::string GOName, GameObject& go);
		GameObject& FetchGO(Entity id);
		void AttachComponents(GameObject& obj, std::pair<std::string, std::any> component );
		void EntityDestroyed(Entity) override;
		void Destroy(Entity);
		Entity DestroyEntity(Entity); // For recursively destroying all children attached
		void DestroyAll();
		void UpdateDestroyed();
		void UpdateGOName(GameObject& go, std::string newName); // TODO: Make a function to update the GO names		
		// To fix the issue of all gameobjects having the same name
		// Add a number to the back until it has a unique name
		std::string CreateGOName(std::string goName);

		//Used for updating the sceneGO if the entity's parent was changed so that scene hierarchy can update
		void UpdateParent(Entity entityID, Entity parentID, bool toDelete = false);

		template<typename T>
		void AddComponent(GameObject& obj, std::string componentName, std::any componentData)
		{
			if (componentName == typeid(T).name())
			{
				obj.AddComponent(std::any_cast<T>(componentData));
			}
		}

#pragma endregion

		// FOR TESTING
		void ParentAllGO();

#pragma region IMGUI Accessor functions
		void ForAllGO(const std::function<void(GameObject&)>& op);
		void ForAllSceneGOs(const std::function<void(GameObject&)>& op);
		void GOFactory::ForAllSceneUIGOs(const std::function<void(GameObject&)>& func);
		void ForGOChildren(GameObject& parentGO, const std::function<void(GameObject&)>& go);
		std::unordered_map<Entity, GameObject>& GetMIDToGO();

#pragma endregion

#pragma region Importing and Exporting
		void ImportGO(const rapidjson::Value& go); // Start the recursive importing 
		Entity ImportEntity(const rapidjson::Value& go, Entity parentID); // For recursively importing entities
		void ExportGOs(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer); // Start the recursive exporting
		void ExportEntity(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer, Entity id); // For recursively exporting entities
#pragma endregion

		void ReceiveMessage(Message* msg) override;

		//void SendMessage(Message* msg) override;
	};

	extern GOFactory* gGOFactory;
}

#endif
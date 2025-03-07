/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GOFactory.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A system to handle creation and deletion of game objects. Allows for cloning, prefab creation, Importing and exporting of game objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"
#include "Editor/Editor.h"

#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"
#include "Components/ParticleEmitter.h"
#include "Components/RigidBody.h"
#include "Components/Script.h"
#include "Components/Prefab.h"
#include "log.h"

namespace Carmicah
{
	// Trying this method for instantiating gofactory
	// subject to change
	GOFactory* gGOFactory = NULL;

	GOFactory::GOFactory()
	{
		//
		if (gGOFactory == NULL)
			gGOFactory = this;

		//Create pointer to entity manager
		//mEntityManager = std::make_unique<EntityManager>();
		//mComponentManager = std::make_unique<ComponentManager>();
	}

	GOFactory::~GOFactory()
	{
		// Destroy all objects if factory is deleted
		//DestroyAll();
	}

#pragma region GameObject Functions

	GameObject GOFactory::CreateGO(std::string name, TRANSFORMTYPE flag)
	{
		GameObject go;
		std::string goName = CreateGOName(name);
		go.mID = EntityManager::GetInstance()->CreateEntity();
		go.mName = goName;
		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(go.mName, go.mID));
		mIDToGO.insert(std::make_pair(go.mID, go));

		// All entities created is stored in GOFactory
		
		// By default add transform to a blank component
		if(flag == TRANSFORMTYPE::TRANSFORM)
		{
			go.AddComponent<Transform>();
			UpdateParent(go.mID, sceneGO.sceneID);
		}
		else if(flag == TRANSFORMTYPE::UITRANSFORM)
		{
			go.AddComponent<UITransform>();
			UpdateParent(go.mID, sceneGO.sceneID);
		}
		
		//SystemManager::GetInstance()->UpdateSignatures(go.mID, EntityManager::GetInstance()->GetSignature(go.mID));
		//else //Case for empty GOs
		//{
		//	//Empty GOs should only act as folders, so always will be under the main scene hierarchy (for now until sub-folders :sadge:)
		//	//sceneGO.children.insert(go.mID);
		//}

		CM_CORE_INFO("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));

		// Parent it to the scene on creation
		

		return go;
	}

	GameObject GOFactory::LoadGO(std::string name, Entity entityID)
	{
		GameObject go;
		go.mID = EntityManager::GetInstance()->LoadEntity(entityID);
		go.mName = CreateGOName(name);

		mNameToID.insert(std::make_pair(go.mName, go.mID));
		mIDToGO.insert(std::make_pair(go.mID, go));

		SystemManager::GetInstance()->UpdateSignatures(go.mID, EntityManager::GetInstance()->GetSignature(go.mID));
		CM_CORE_INFO("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));
		return go;
	}

	GameObject GOFactory::CloneGO(GameObject const& go, Entity parentID)
	{
		GameObject newGO(go);
		newGO.mName = CreateGOName(go.mName);
		// TODO: Find a way to create clones like clone1, clone2, clone3, etc
		// can probably while loop then check if the map with .count instead of using .find
		// if it has 1, then increment a counter to go to clone2, clone3, etc until it finds no counts
		newGO.mID = EntityManager::GetInstance()->CreateEntity();
		EntityManager::GetInstance()->CloneEntity(go.mID, newGO.mID);

		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(newGO.mName, newGO.mID));
		mIDToGO.insert(std::make_pair(newGO.mID, newGO));

		Carmicah::Log::GetCoreLogger()->info("Creating a clone of game object with name: " + newGO.mName + " with id: " + std::to_string(newGO.mID));

		SystemManager::GetInstance()->UpdateSignatures(newGO.mID, EntityManager::GetInstance()->GetSignature(newGO.mID));
		if (ComponentManager::GetInstance()->HasComponent<Transform>(newGO.mID))
		{
			Transform& newTransform = ComponentManager::GetInstance()->GetComponent<Transform>(newGO.mID);
			newTransform.children.clear(); // if they copy they'll copy the children list also

			// Update parent will replace parent so dont have to reseet parent
			// by default, if creating a new clone the value of parentID is 0,
			// so it will be parented to the scene unless specified other wies by passing in a parent
			UpdateParent(newGO.mID, parentID);

			Transform& originalTransform = ComponentManager::GetInstance()->GetComponent<Transform>(go.mID);
			// Check for children
			if (originalTransform.children.size() > 0)
			{
				// Clone the child and parent it to this
				for (auto it : originalTransform.children)
				{
					GameObject newGo = gGOFactory->FetchGO(it);
					// Clone this go and parent it to the cloned go
					CloneGO(newGo, newGO.mID);
				}
			}
		}
		// UITransform has no parent and child yet so we do it differently than Transform
		else if (ComponentManager::GetInstance()->HasComponent<UITransform>(newGO.mID))
		{
			UpdateParent(newGO.mID, parentID);


		}

		return newGO;
	}

	GameObject GOFactory::CreatePrefab(std::string prefab)
	{
		GameObject newGO;
		std::string goName = CreateGOName(prefab);
		newGO.mID = EntityManager::GetInstance()->CreateEntity();
		//CM_CORE_INFO("Creating GO with ID {}", newGO.mID);
		newGO.mName = goName;
		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(newGO.mName, newGO.mID));
		mIDToGO.insert(std::make_pair(newGO.mID, newGO));

		if (AssetManager::GetInstance()->AssetExist<Prefab>(prefab))
		{
			Prefab& goPrefab = AssetManager::GetInstance()->GetAsset<Prefab>(prefab);
			// Since this entity is being made from this prefab, add it to the prefab EntityWatcher
			//goPrefab.entityWatcher.push_back(newGO.mID);

			// Loop through the components within asset manager
			for (auto& component : goPrefab.mComponents)
			{
				AttachComponents(newGO, component);
				// Same if checks as component manager, but we're adding components here instead of deserializing
			}
			// Add the prefab component since this is a prefab obj
			newGO.AddComponent<PrefabData>();

			// update prefab Reference
		// update prefab Reference
			if (newGO.HasComponent<PrefabData>())
			{
				newGO.GetComponent<PrefabData>().mPrefabRef = goPrefab.mPrefabID;
			}

			NewPrefabGOMsg msg(newGO.mID, goPrefab.mPrefabID);
			SendSysMessage(&msg);

			// Parent it to the scene on creation
			UpdateParent(newGO.mID, sceneGO.sceneID);

			//CM_CORE_INFO("Creating prefab " + newGO.mName + " with ID " + std::to_string(newGO.mID) + " parenting to " + std::to_string(sceneGO.sceneID));

			// Now check for children
			// Has children
			if (goPrefab.childList.size() > 0)
			{
				for (auto& it : goPrefab.childList)
				{
					CreatePrefabChild(it, newGO.mID);
				}
			}
		}
		else
		{
			//TODO: Decide if this should assert or give an error warning and return an empty GO instead
			assert("Prefab does not exist");
		}

		return newGO;
	}

	void GOFactory::CreatePrefabChild(Prefab& prefab, Entity parentID)
	{
		GameObject newGO;
		std::string goName = CreateGOName(prefab.mName);
		newGO.mID = EntityManager::GetInstance()->CreateEntity();
		//CM_CORE_INFO("Creating GO with ID {}", newGO.mID);

		newGO.mName = goName;
		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(newGO.mName, newGO.mID));
		mIDToGO.insert(std::make_pair(newGO.mID, newGO));

		for (auto& component : prefab.mComponents)
		{
			AttachComponents(newGO, component);
		}

		//CM_CORE_INFO("Position After attach: {}, {}", newGO.GetComponent<Transform>().GetPos().x, newGO.GetComponent<Transform>().GetPos().y);
		// Add the prefab component since this is a prefab obj
		newGO.AddComponent<PrefabData>();

		// update prefab Reference
		if (newGO.HasComponent<PrefabData>())
		{
			newGO.GetComponent<PrefabData>().mPrefabRef = prefab.mPrefabID;
		}

		NewPrefabGOMsg msg(newGO.mID, prefab.mPrefabID);
		SendSysMessage(&msg);
		// Set the child to parent the original GO
		UpdateParent(newGO.mID, parentID);

		// NOTE: Update parent fks the child's transform position back to 0, 0
		// so im forcefully setting it back here

		if (newGO.HasComponent<Transform>())
			newGO.GetComponent<Transform>().Pos(prefab.GetComponent<Transform>().Pos());
		else if (newGO.HasComponent<UITransform>())
			newGO.GetComponent<UITransform>().Pos(prefab.GetComponent<UITransform>().Pos());

		//CM_CORE_INFO("Creating prefab child " + newGO.mName + " with ID " + std::to_string(newGO.mID) + " parenting to " + std::to_string(parentID));
		//CM_CORE_INFO("Position After Update Parent: {}, {}", newGO.GetComponent<Transform>().GetPos().x, newGO.GetComponent<Transform>().GetPos().y);

		// If there is a child in this prefab also, then go through the process again
		if (prefab.childList.size() > 0)
		{
			for (auto& it : prefab.childList)
			{
				CreatePrefabChild(it, newGO.mID);
			}
		}
	}

	void GOFactory::CreateSceneObject(std::string sceneName)
	{
		sceneGO.sceneName = sceneName;
		sceneGO.children.clear();
		// Blank transform
		//sceneGO.sceneTransform = Transform{};
	}

	std::vector<Entity> GOFactory::GetEntitiesWithTag(std::string tagName)
	{
		std::vector<Entity> entityIDs;
		for (auto& entity : mEntitiesSet)
		{
			if (ComponentManager::GetInstance()->HasComponent<Transform>(entity))
			{
				if (ComponentManager::GetInstance()->GetComponent<Transform>(entity).transformTag == tagName)
				{
					entityIDs.push_back(entity);
				}
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(entity))
			{
				// NOTE: I dont know if we need to support UITransform also
				// i think since this is transform itll just be normal transform and not UI transform
			}
		}

		return entityIDs;
	}


	void GOFactory::FetchGO(std::string GOName, GameObject& go)
	{
		if (mNameToID.count(GOName) == 0)
		{		
			CM_CORE_ERROR("GameObject does not exist");
			return;
			//assert(mNameToID.count(GOName) != 0 && "GameObject does not exist");
		}

		go = mIDToGO[mNameToID[GOName]];
		//return mIDToGO[mNameToID[GOName]];
	}

	GameObject& GOFactory::FetchGO(Entity id)
	{
		if (mIDToGO.count(id) <= 0)
		{
			CM_CORE_ERROR("GO Doesn't Exist");
			assert("Doesn't exist");
		}

		return mIDToGO[id];
	}

	void GOFactory::AttachComponents(GameObject& obj, std::pair<std::string, std::any> component)
	{
		std::string componentName = component.first;
		std::any componentData = component.second;

		AddComponent<Transform>(obj, componentName, componentData);
		AddComponent<UITransform>(obj, componentName, componentData);
		AddComponent<Collider2D>(obj, componentName, componentData);
		AddComponent<Animation>(obj, componentName, componentData);
		AddComponent<Renderer>(obj, componentName, componentData);
		AddComponent<RigidBody>(obj, componentName, componentData);
		AddComponent<TextRenderer>(obj, componentName, componentData);
		AddComponent<ParticleEmitter>(obj, componentName, componentData);
		AddComponent<Script>(obj, componentName, componentData);
		AddComponent<Button>(obj, componentName, componentData);
		AddComponent<PrefabData>(obj, componentName, componentData);
		AddComponent<Sound>(obj, componentName, componentData);
		AddComponent<StateMachine>(obj, componentName, componentData);
	}

	void GOFactory::EntityDestroyed(Entity entity)
	{
		// can't do here cause if the parent is deleted before the child, calling parent transform to remove child will break
		//UpdateParent(entity, sceneGO.sceneID, true);

		// erase from the maps
		mNameToID.erase(mIDToGO[entity].mName);
		mIDToGO.erase(entity);


	}

	void GOFactory::Destroy(Entity entity)
	{
		if (mEntitiesSet.count(entity) <= 0)
		{
			CM_CORE_ERROR("Trying to delete a gameobject that does not exist");
			return;
		}
		CM_CORE_INFO("Deleting GO with ID {}", entity);

		// To destroy at the end of update
		mDeleteList.insert(DestroyEntity(entity));

	}

	Entity GOFactory::DestroyEntity(Entity entity)
	{
		GameObject& go = mIDToGO[entity];

		// Remove it from the parent since its being destroyed
		//if(go.HasComponent<Transform>())
		//{
		//	UpdateParent(entity, go.GetComponent<Transform>().parent, true);
		//}

		//else if (go.HasComponent<UITransform>())
		//{
		//	UpdateParent(entity, go.GetComponent<UITransform>().parent, true);
		//}

		// Check if has any children
		if (go.HasComponent<Transform>())
		{
			if (go.GetComponent<Transform>().children.size() > 0)
			{
				for (auto& id : go.GetComponent<Transform>().children)
				{
					// Recall the function so any children within that child is also inserted
					// NOTE: I have only tested this with 1 layer of parent-child, so if it dies next time, cehck this part 
					mDeleteList.insert(DestroyEntity(id));

				}
			}
		}
		else if (go.HasComponent<UITransform>())
		{
			CM_CORE_INFO("Children size {}", go.GetComponent<UITransform>().children.size());
			if (go.GetComponent<UITransform>().children.size() > 0)
			{
				for (auto& id : go.GetComponent<UITransform>().children)
				{
					// Recall the function so any children within that child is also inserted
					// NOTE: I have only tested this with 1 layer of parent-child, so if it dies next time, cehck this part 
					
					mDeleteList.insert(DestroyEntity(id));
				}
			}
		}

		return entity;
	}

	void GOFactory::DestroyAll()
	{
		// Since GOFactory holds all the entities
		// Can just use entitiesSet to destroy all
		mDeleteList = mEntitiesSet;
	}

	void GOFactory::UpdateDestroyed()
	{
		for (auto& entity : mDeleteList)
		{
			//CM_CORE_INFO("Destroying GO with ID {}", entity);

			SystemManager::GetInstance()->EntityDestroyed(entity);
			EntityKilledMessage msg(entity);
			SendSysMessage(&msg);
		}

		mDeleteList.clear();
	}

	void GOFactory::UpdateGOName(GameObject& go, std::string newName)
	{
		mNameToID.erase(go.mName);
		mNameToID.insert(std::make_pair(newName, go.mID));
		go.mName = newName;
	}

	std::string GOFactory::CreateGOName(std::string goName)
	{
		std::string newGOName = goName;
		int counter = 1;
		while (mNameToID.count(newGOName) != 0)
		{
			newGOName = goName + "_" + std::to_string(counter);
			counter++;
		}


		return newGOName;
	}

	/// <summary>
	///  Updates the parent entity. Removes it from its child list and add it to the new one.
	/// Currently the issue with having two different transform and a base transform, I have to check
	/// which transform it has and when i get the base transform, i have to cast it
	/// So currently it requires a lot of if and else if to check whether it has or not and cast to the respective base transform.
	/// I'm not sure if I should move parent and child hirerachy into another component on its own
	/// </summary>
	/// <param name="entityID">Current Entity ID</param>
	/// <param name="newParentID">The new parent ID</param>
	/// <param name="toDelete"> If the object is being deleted, only need to remove from parent instead </param>
	void GOFactory::UpdateParent(Entity entityID, Entity newParentID, bool toDelete) 
	{
		/*Technically this can be done first cause updateParent should always work so, let the editor update its hierarchy copy side
		using Transform.parent (getting the oldParent) so it can edit*/
		UpdateHierarchyMessage msg2(entityID, newParentID);
		SendSysMessage(&msg2);

		GameObject& go = mIDToGO[entityID];

		// Remove entityID from it's current parent
		// Check if its part of sceneGO

		//Removing from sceneGO's hierarchy
		if (sceneGO.children.count(entityID) > 0)
		{
			sceneGO.children.erase(entityID);
		}

		//Only runs for new GameObjects
		else if (go.HasComponent<Transform>() && go.GetComponent<Transform>().ParentID() == 0 && newParentID == 0)
		{
			// its a new object
			sceneGO.children.insert(entityID);
			return;
		}

		else if (go.HasComponent<UITransform>() && go.GetComponent<UITransform>().ParentID() == 0 && newParentID == 0)
		{
			// its a new object
			sceneGO.children.insert(entityID);
			return;
		}
		
		// Find out what is the current parent
		else
		{
			
			// Handle UI Trasnform uniquely for now
			//if (go.HasComponent<UITransform>())
			//{
			//	sceneGO.children.insert(go.mID);
			//	go.GetComponent<UITransform>().parent = newParentID;

			//	// UI Transform for now can only add directly to scene
			//	return;
			//}

			Entity parentID = 0;
			// Get the old parent ID
			if (go.HasComponent<Transform>())
			{
				// Get the parent ID
				parentID = go.GetComponent<Transform>().ParentID();
			}
			else if (go.HasComponent<UITransform>())
			{
				parentID = go.GetComponent<UITransform>().ParentID();
			}

			// if not deleting then we need to update the transform based on the new parent's
			if (!toDelete)
			{
				// Send msg to UpdateTransform
				// Important to send here because if parenting back to scene
				// we need the original parent's transform so that we can convert the entity's local transform
				// back to world transform
				UpdateTransformMessage msg(entityID, newParentID);
				SendSysMessage(&msg);
			}


			// It isnt being parented to scene for the first time on creation
			if (parentID == 0 && newParentID != 0)
			{
				assert("Parent ID does not exist");
				return;
			}

			// Get the parent's transform
			if (ComponentManager::GetInstance()->HasComponent<Transform>(parentID))
			{
				// Get the transform
				Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(parentID);
				// Erase from parent's child ids
				for (auto it = parentTransform.children.begin(); it != parentTransform.children.end(); it++)
				{
					if (*it == entityID)
					{
						parentTransform.children.erase(it);
						break;
					}
				}
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(parentID))
			{
				UITransform& parentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(parentID);
				for (auto it = parentTransform.children.begin(); it != parentTransform.children.end(); it++)
				{
					if (*it == entityID)
					{
						parentTransform.children.erase(it);
						break;
					}
				}
			}
		}

		// Only need to change to a new parent/update parent's child if its not being deleted from the scene
		if (!toDelete)
		{
			//Change the parent after removing from the old parent
			// If its being parented to the scene
			if (newParentID == sceneGO.sceneID)
			{

				sceneGO.children.insert(entityID);
				if (ComponentManager::GetInstance()->HasComponent<Transform>(entityID))
				{
					// Get the transform
					BaseTransform<Transform>& entityTransform = static_cast<BaseTransform<Transform>&>(ComponentManager::GetInstance()->GetComponent<Transform>(entityID));
					// Change the parent
					entityTransform.SetParent(sceneGO.sceneID, 0);
				}
				else if (ComponentManager::GetInstance()->HasComponent<UITransform>(entityID))
				{
					// Get the transform
					BaseTransform<UITransform>& entityTransform = static_cast<BaseTransform<UITransform>&>(ComponentManager::GetInstance()->GetComponent<UITransform>(entityID));
					// Change the parent
					entityTransform.SetParent(sceneGO.sceneID, 0);
				}

			}
			// If it isnt then have to find the new parent
			else
			{
				unsigned int parentLevel{};
				if (ComponentManager::GetInstance()->HasComponent<Transform>(newParentID))
					parentLevel = ComponentManager::GetInstance()->GetComponent<Transform>(newParentID).GrandChildLevel();
				else if (ComponentManager::GetInstance()->HasComponent<UITransform>(newParentID))
					parentLevel = ComponentManager::GetInstance()->GetComponent<UITransform>(newParentID).GrandChildLevel();

				// Send msg to UpdateTransform
				// Important to send here because if parenting back to scene
				// we need the original parent's transform so that we can convert the entity's local transform
				// back to world transform
				UpdateTransformMessage msg(entityID, newParentID);
				SendSysMessage(&msg);

				

				// Change the current transform parent ID
				//if (go.HasComponent<Transform>())
				//{
				//	// Change the parent
				//	go.GetComponent<Transform>().SetParent(newParentID, parentLevel);
				//}
				//else if (go.HasComponent<UITransform>())
				//{
				//	// Change the parent
				//	go.GetComponent<UITransform>().SetParent(newParentID, parentLevel);
				//}

				//if (ComponentManager::GetInstance()->HasComponent<Transform>(newParentID))
				//{
				//	// Get the transform
				//	Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(newParentID);
				//	// Add to the child list
				//	parentTransform.children.push_back(entityID);

				//	//CM_CORE_INFO("Parenting entity: " + std::to_string(entityID) + " to " + std::to_string(newParentID));
				//}
				//else if (ComponentManager::GetInstance()->HasComponent<UITransform>(newParentID))
				//{
				//	// Get the transform
				//	UITransform& parentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(newParentID);
				//	// Add to the child list
				//	parentTransform.children.push_back(entityID);
				//}
			}
		}
	}
#pragma endregion

	// TODO: Remove after setting up scene hirerachy
	void GOFactory::ParentAllGO()
	{
		for (std::set<Entity>::iterator it = mEntitiesSet.begin(); it != mEntitiesSet.end(); ++it)
		{
			sceneGO.children.insert(*it);

			if (ComponentManager::GetInstance()->HasComponent<Transform>(*it))
			{			
				BaseTransform<Transform>& entityTransform = static_cast<BaseTransform<Transform>&>(ComponentManager::GetInstance()->GetComponent<Transform>(*it));
				entityTransform.SetParent(sceneGO.sceneID, 0);
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(*it))
			{
				BaseTransform<UITransform>& entityTransform = static_cast<BaseTransform<UITransform>&>(ComponentManager::GetInstance()->GetComponent<UITransform>(*it));
				entityTransform.SetParent(sceneGO.sceneID, 0);
			}
		
			//entityTransform.parent = &sceneGO.sceneTransform; // Set all GOs to be children of scene
			//sceneGO.sceneTransform.childrens.push_back(&entityTransform); // add the transform to the child

			// this function is just because our current version of scene isnt parented
			// so i'm trying to make it all parented for now and once its done ill remove this function
		}
	}

#pragma region IMGUI Accessor functions
	std::unordered_map<Entity, GameObject>& GOFactory::GetMIDToGO()
	{
		return mIDToGO;
	}

	void GOFactory::ForAllGO(const std::function<void(GameObject&)>& func)
	{
		if (mIDToGO.size() > 0)
		{
			for (auto& i : mIDToGO)
				func(i.second);
		}
	}


	//REMINDER FOR ME TO CHANGE THIS AGAIN LATER
	void GOFactory::ForAllSceneGOs(const std::function<void(GameObject&)>& func)
	{
		if (Editor::mSceneHierarchy.size() > 0)
		{
			for (auto& child : Editor::mSceneHierarchy)
			{
				func(mIDToGO[child]);
			}
		}
	}

	void GOFactory::ForAllSceneUIGOs(const std::function<void(GameObject&)>& func)
	{
		if (Editor::mSceneUIHierarchy.size() > 0)
		{
			for (auto& child : Editor::mSceneUIHierarchy)
			{
				func(mIDToGO[child]);
			}
		}
	}

	void GOFactory::ForGOChildren(GameObject& parentGO, const std::function<void(GameObject&)>& func)
	{
		if (parentGO.HasComponent<Transform>() && parentGO.GetComponent<Transform>().children.size() > 0)
		{
			
			for (auto& child : Editor::mChildrenHierarchy[parentGO.GetID()])
			{
				func(mIDToGO[child]);
			}
		}
		else if (parentGO.HasComponent<UITransform>() && parentGO.GetComponent<UITransform>().children.size() > 0)
		{
			for (auto& child : Editor::mChildrenHierarchy[parentGO.GetID()])
			{
				func(mIDToGO[child]);
			}

		}

	}
#pragma endregion

#pragma region Importing and Exporting

	void GOFactory::ImportGO(const rapidjson::Value& doc)
	{
		std::string sceneName = std::string(doc["Scene"].GetString());
		CreateSceneObject(sceneName);

		if (doc.HasMember("SceneObjects"))
		{
			const rapidjson::Value& sceneObjects = doc["SceneObjects"];
			for (rapidjson::SizeType i = 0; i < sceneObjects.Size(); ++i)
			{
				const rapidjson::Value& go = sceneObjects[i];
				sceneGO.children.insert(ImportEntity(go, sceneGO.sceneID));
			}
		}
	}

	void GOFactory::ExportGOs(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
	{
		writer.StartObject();
		writer.String("Scene");
		writer.String(sceneGO.sceneName.c_str(), static_cast<rapidjson::SizeType>(sceneGO.sceneName.length()));
		writer.String("SceneObjects");
		writer.StartArray();
		for (auto& id : sceneGO.children)
		{
			ExportEntity(writer, id);
		}
		writer.EndArray();
		writer.EndObject();
	}

	void GOFactory::ExportEntity(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer, Entity id)
	{
			GameObject& obj = mIDToGO[id];
			writer.StartObject();

			writer.String("GameObject");
			writer.String(obj.GetName().c_str(), static_cast<rapidjson::SizeType>(obj.GetName().length()));

			writer.String("ID");
			writer.Int(obj.GetID());

			writer.String("Components");

			writer.StartArray();
			ComponentManager::GetInstance()->SerializeEntityComponents(obj.GetID(), EntityManager::GetInstance()->GetSignature(obj.GetID()), writer);
			writer.EndArray();

			// Check for children. Only loop if there obj has children
			if (obj.HasComponent<Transform>())
			{
				//if (obj.GetComponent<Transform>().children.size())
				writer.String("Children");
				writer.StartArray();

				for (auto id2 : obj.GetComponent<Transform>().children)
				{
					ExportEntity(writer, id2);
				}

				writer.EndArray();
			}
			else if (obj.HasComponent<UITransform>() && obj.GetComponent<UITransform>().children.size() > 0)
			{
				writer.String("Children");
				writer.StartArray();

				for (auto id2 : obj.GetComponent<UITransform>().children)
				{
					ExportEntity(writer, id2);
				}

				writer.EndArray();
			}

			writer.EndObject();

	}
	
	Entity GOFactory::ImportEntity(const rapidjson::Value& go, Entity parentID)
	{

		std::string name = std::string(go["GameObject"].GetString());
		int entityID = go["ID"].GetInt();
		//mainCam = newObj.GetID
		GameObject newObj = LoadGO(name, entityID);

		if (name == "MainCamera")
			mainCam = newObj.GetID();

		// Add all components to the obj
		const rapidjson::Value& componentList = go["Components"];
		for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
		{
			// Get the name of the current component being deserialized
			const std::string& componentName = (*it)["Component Name"].GetString();
			// Deserialize the json file that contains the component's data
			std::any componentData = ComponentManager::GetInstance()->DeserializePrefabComponent(*it);

			// Attach it to the game object
			AttachComponents(newObj, std::make_pair(componentName, componentData));
		}


		if (entityID == 20 || entityID == 21)
		{
			CM_CORE_INFO("TESTING IF ITS HERE");
			CM_CORE_INFO("ballon position {}, {}", newObj.GetComponent<Transform>().Pos().x, newObj.GetComponent<Transform>().Pos().y);
		}
		unsigned int parentChildLevel{};
		if (parentID != 0)
		{
			if (mIDToGO[parentID].HasComponent<Transform>())
				parentChildLevel = mIDToGO[parentID].GetComponent<Transform>().GrandChildLevel();
			else if (mIDToGO[parentID].HasComponent<UITransform>())
				parentChildLevel = mIDToGO[parentID].GetComponent<UITransform>().GrandChildLevel();
		}

		// Attach to the parent
		if (newObj.HasComponent<Transform>())
		{
			newObj.GetComponent<Transform>().SetParent(parentID, parentChildLevel);
		}
		else if (newObj.HasComponent<UITransform>())
		{
			newObj.GetComponent<UITransform>().SetParent(parentID, parentChildLevel);
		}

		// Now check if that obj has children that we need to deserialize
		// if it does then we have to recursively add it
		if (go.HasMember("Children"))
		{
			const rapidjson::Value& childrenList = go["Children"];
			for (rapidjson::SizeType i = 0; i < childrenList.Size(); ++i)
			{
				const rapidjson::Value& childGO = childrenList[i];
				if (newObj.HasComponent<Transform>())
				{
					newObj.GetComponent<Transform>().children.push_back(ImportEntity(childGO, entityID));
				}
				else if (newObj.HasComponent<UITransform>())
				{
					newObj.GetComponent<UITransform>().children.push_back(ImportEntity(childGO, entityID));
				}
			}
		}

		UpdateHierarchyMessage msg(newObj.GetID(),parentID);
		SendSysMessage(&msg);
		return entityID;
	}

#pragma endregion

	void GOFactory::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_KEYPRESS)
		{			
			Carmicah::Log::GetCoreLogger()->info("Msg Recevied in GOFactory containing :" + std::to_string(static_cast<KeyMessage*>(msg)->mKeypress));
		}

		// If a prefab was modified
		//if (msg->mMsgType == MSG_MODIFYPREFAB)
		//{
		//	// loop through to see which entities have to be updated
		//	auto casted_msg = dynamic_cast<ModifyPrefabMsg*>(msg);
		//	CM_CORE_INFO(std::to_string(casted_msg->mID));

		//	// Loop through which entities are using this prefab
		//	//for (auto it : casted_msg->prefabRef.entityWatcher)
		//	//{

		//	//}
		//}
	}

	
}
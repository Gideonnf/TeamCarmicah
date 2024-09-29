#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
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

	GameObject GOFactory::CreateGO(std::string name)
	{
		GameObject go;
		std::string goName = CreateGOName(name);
		go.mID = EntityManager::GetInstance()->CreateEntity();
		go.mName = goName;
		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(go.mName, go.mID));
		mIDToGO.insert(std::make_pair(go.mID, go));

		// All entities created is stored in GOFactory
		SystemManager::GetInstance()->UpdateSignatures(go.mID, EntityManager::GetInstance()->GetSignature(go.mID));
		
		CM_CORE_INFO("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));

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

	GameObject GOFactory::CloneGO(GameObject const& go)
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

		return newGO;
	}

	GameObject GOFactory::CreatePrefab(std::string prefab)
	{
		GameObject newGO = CreateGO("Duck");
		if (AssetManager::GetInstance()->prefabFiles.count(prefab) > 0)
		{
			const rapidjson::Document goFile = SerializerSystem::GetInstance()->DeserializePrefab(AssetManager::GetInstance()->prefabFiles[prefab]);
			std::string name = std::string(goFile["GameObject"].GetString());

			const rapidjson::Value& componentList = goFile["Components"];
			for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
			{
				const std::string& componentName = (*it)["Component Name"].GetString();
				if (componentName == "struct Carmicah::Transform")
				{
					Transform t;
					t.DeserializeComponent((*it));
					newGO.AddComponent<Transform>(t);
				}
				else if (componentName == "struct Carmicah::Collider2D")
				{
					Collider2D t;
					t.DeserializeComponent((*it));
					newGO.AddComponent<Collider2D>(t);

				}
				else if (componentName == "struct Carmicah::Renderer")
				{
					Renderer t;
					t.DeserializeComponent((*it));
					newGO.AddComponent<Renderer>(t);
				}
				else if (componentName == "struct Carmicah::Animation")
				{
					Animation t{};
					t.DeserializeComponent((*it));
					newGO.AddComponent<Animation>(t);
				}
			}

		}
		else
		{
			// assert an error here cause prefab doesnt exist in asset manager
		}

		return newGO;
	}

	GameObject GOFactory::FetchGO(std::string GOName)
	{
		return mIDToGO[mNameToID[GOName]];
	}

	void GOFactory::EntityDestroyed(Entity entity)
	{
		// erase from the maps
		mNameToID.erase(mIDToGO[entity].mName);
		mIDToGO.erase(entity);

		EntityManager::GetInstance()->DeleteEntity(entity);
		ComponentManager::GetInstance()->EntityDestroyed(entity);
	}

	void GOFactory::Destroy(Entity entity)
	{
		// To destroy at the end of update
		mDeleteList.insert(entity);
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
			SystemManager::GetInstance()->EntityDestroyed(entity);
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
			newGOName = goName + std::to_string(counter);
			counter++;
		}

		return newGOName;
	}
#pragma endregion

#pragma region Importing and Exporting
	void GOFactory::ForAllGO(const std::function<void(GameObject&)>& func)
	{
		if (mIDToGO.size() > 0)
		{
			for (auto& i : mIDToGO)
				func(i.second);
		}
	}

	void GOFactory::ImportGO(const rapidjson::Value& go)
	{
		std::string name = std::string(go["GameObject"].GetString());
		int entityID = go["ID"].GetInt();
			//mainCam = newObj.GetID
		GameObject newObj = LoadGO(name, entityID);

		if (name == "MainCamera")
			mainCam = newObj.GetID();

		const rapidjson::Value& componentList = go["Components"];
		for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
		{
			const std::string& componentName = (*it)["Component Name"].GetString();
			if (componentName == "struct Carmicah::Transform")
			{
				Transform t;
				t.DeserializeComponent((*it));
				newObj.AddComponent<Transform>(t);
			}
			else if (componentName == "struct Carmicah::Collider2D")
			{
				Collider2D t;
				t.DeserializeComponent((*it));
				newObj.AddComponent<Collider2D>(t);

			}
			else if (componentName == "struct Carmicah::Renderer")
			{
				Renderer t;
				t.DeserializeComponent((*it));
				newObj.AddComponent<Renderer>(t);
			}
			else if (componentName == "struct Carmicah::Animation")
			{
				Animation t{};
				t.DeserializeComponent((*it));
				newObj.AddComponent<Animation>(t);
			}
		}
	}

	void GOFactory::ExportGOs(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
	{
		writer.StartArray();
		for (auto& obj : mIDToGO)
		{
			writer.StartObject();

			writer.String("GameObject");
			writer.String(obj.second.GetName().c_str(), static_cast<rapidjson::SizeType>(obj.second.GetName().length()));

			writer.String("ID");
			writer.Int(obj.second.GetID());

			writer.String("Components");

			writer.StartArray();
			ComponentManager::GetInstance()->SerializeEntityComponents(obj.second.GetID(), EntityManager::GetInstance()->GetSignature(obj.second.GetID()), writer);
			writer.EndArray();

			writer.EndObject();
		}
		writer.EndArray();
		//gGOFactory->ForAllGO([&](GameObject& o) {
		//	
		//	});
		

	}
#pragma endregion

#pragma region Component Functions
	template <typename T>
	void GOFactory::CreateComponent()
	{
		ComponentManager::GetInstance()->RegisterComponent<T>();
	}
#pragma endregion

	void GOFactory::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_KEYPRESS)
		{			
			Carmicah::Log::GetCoreLogger()->info("Msg Recevied in GOFactory containing :" + std::to_string(static_cast<KeyMessage*>(msg)->mKeypress));
		}
	}
}
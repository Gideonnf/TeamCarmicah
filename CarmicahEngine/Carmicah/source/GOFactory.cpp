#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"

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
		go.mID = EntityManager::GetInstance()->CreateEntity(name);
		go.mName = name;
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
		go.mName = name;

		mNameToID.insert(std::make_pair(go.mName, go.mID));
		mIDToGO.insert(std::make_pair(go.mID, go));

		// for now idk if i want to update here or in ImportGO 
		//SystemManager::GetInstance()->UpdateSignatures(go.mID,)
		CM_CORE_INFO("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));
		return go;
	}

	GameObject GOFactory::CloneGO(GameObject const& go)
	{
		GameObject newGO(go);
		newGO.mName = go.mName + "_Clone"; 
		// TODO: Find a way to create clones like clone1, clone2, clone3, etc
		// can probably while loop then check if the map with .count instead of using .find
		// if it has 1, then increment a counter to go to clone2, clone3, etc until it finds no counts
		newGO.mID = EntityManager::GetInstance()->CreateEntity(newGO.mName);
		EntityManager::GetInstance()->CloneEntity(go.mID, newGO.mID);

		// Store in two maps. Testing use for fetching GO by name
		mNameToID.insert(std::make_pair(newGO.mName, newGO.mID));
		mIDToGO.insert(std::make_pair(newGO.mID, newGO));

		Carmicah::Log::GetCoreLogger()->info("Creating a clone of game object with name: " + newGO.mName + " with id: " + std::to_string(newGO.mID));

		//Updating of signature done in the cloning process in entity manager

		return newGO;
	}

	void GOFactory::CreateGO(GameObject go)
	{
		go.mID = EntityManager::GetInstance()->CreateEntity(go.mName);
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

	void GOFactory::ForAllGO(const std::function<void(GameObject&)>& func)
	{
		if (mIDToGO.size() > 1)
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
				t.min.x = static_cast<float>((*it)["minX"].GetDouble());
				t.min.y = static_cast<float>((*it)["minY"].GetDouble());
				t.max.x = static_cast<float>((*it)["maxX"].GetDouble());
				t.max.y = static_cast<float>((*it)["maxY"].GetDouble());
				t.shape = (*it)["shape"].GetString();
				newObj.AddComponent<Collider2D>(t);

			}
			else if (componentName == "struct Carmicah::Renderer")
			{
				Renderer t;
				t.model = (*it)["model"].GetString();
				t.texture = (*it)["texture"].GetString();
				t.texureMat = glm::mat3(1);
				newObj.AddComponent<Renderer>(t);
			}
			else if (componentName == "struct Carmicah::Animation")
			{
				Animation t{};
				t.xSlice = (*it)["xSlice"].GetInt();
				t.ySlice = (*it)["ySlice"].GetInt();
				t.maxTime = static_cast<float>((*it)["timeBetween"].GetDouble());
				newObj.AddComponent<Animation>(t);
			}
		}
	
		//SystemManager::GetInstance()->UpdateSignatures(newObj.mID, EntityManager::GetInstance()->GetSignature(newObj.mID));
	}

	void GOFactory::ExportGOs(rapidjson::Writer<rapidjson::OStreamWrapper>& writer)
	{
		writer.StartArray();
		gGOFactory->ForAllGO([&](GameObject& o) {
			writer.StartObject();

			writer.String("GameObject");
			writer.String(o.GetName().c_str(), static_cast<rapidjson::SizeType>(o.GetName().length()));

			writer.String("ID");
			writer.Int(o.GetID());

			writer.String("Components");
			writer.StartArray();
			ComponentManager::GetInstance()->ForEachComponent([&](const std::string componentName)
				{
					writer.StartObject();
					writer.String("Component Name");
					writer.String(componentName.c_str(), static_cast<rapidjson::SizeType>(componentName.length()));
					if (componentName == "struct Carmicah::Transform")
					{
						o.GetComponent<Transform>().SerializeComponent(writer);
					}
					else if (componentName == "struct Carmicah::Collider2D")
					{
						Collider2D& t = o.GetComponent<Collider2D>();
						writer.String("minX");
						writer.Double(t.min.x);
						writer.String("minY");
						writer.Double(t.min.y);
						writer.String("maxX");
						writer.Double(t.max.x);
						writer.String("maxY");
						writer.Double(t.max.y);
						writer.String("shape");
						writer.String(t.shape.c_str());
					}
					else if (componentName == "struct Carmicah::Renderer")
					{
						Renderer& t = o.GetComponent<Renderer>();
						writer.String("model");
						writer.String(t.model.c_str());
						writer.String("texture");
						writer.String(t.texture.c_str());
					}
					else if (componentName == "struct Carmicah::Animation")
					{
						Animation& t = o.GetComponent<Animation>();
						writer.String("xSlice");
						writer.Int(t.xSlice);
						writer.String("ySlice");
						writer.Int(t.xSlice);
						writer.String("timeBetween");
						writer.Double(t.maxTime);
					}
					writer.EndObject();

				}, EntityManager::GetInstance()->GetSignature(o.GetID()));
			writer.EndArray();

			writer.EndObject();
			});
		writer.EndArray();

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
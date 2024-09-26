#include "pch.h"
#include "Systems/GOFactory.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>
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
		DestroyAll();
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
		
		Carmicah::Log::GetCoreLogger()->info("Creating a new game object with name: " + name + " and id: " + std::to_string(go.mID));

		return go;
	}

	GameObject GOFactory::CloneGO(GameObject const& go)
	{
		GameObject newGO(go);
		newGO.mName = go.mName + "_Clone"; // TODO: Find a way to create clones like clone1, clone2, clone3, etc
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

	void GOFactory::EntityDestroyed(Entity id) 
	{
		mDeleteList.insert(id);

		//EntityManager::GetInstance()->DeleteEntity(id);
		//ComponentManager::GetInstance()->EntityDestroyed(id);
	}

	void GOFactory::DestroyAll()
	{
		// Since GOFactory holds all the entities
		// Can just use entitiesSet to destroy all
		mDeleteList = mEntitiesSet;
	}

	void GOFactory::UpdateDestroyed()
	{
		std::set<Entity>::iterator iter;
		for (iter = mDeleteList.begin(); iter != mDeleteList.end(); ++iter)
		{
			EntityManager::GetInstance()->DeleteEntity(*iter);
			ComponentManager::GetInstance()->EntityDestroyed(*iter);
		}

		// Clear delete list afterwards
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

	void GOFactory::ImportGOs(std::string sceneName)
	{
		std::ifstream ifs{ sceneName, std::ios::binary };
		if (ifs)
		{
			rapidjson::IStreamWrapper iws(ifs);
			rapidjson::Document document;
			document.ParseStream(iws);
			ifs.close();

			assert(document.IsArray());
			for (rapidjson::SizeType i{}; i < document.Size(); ++i)
			{
				const rapidjson::Value& go = document[i];
				std::string name = std::string(go["GameObject"].GetString());
				int id = go["ID"].GetInt();
				if (id == i)
				{
					GameObject newObj = gGOFactory->CreateGO();
					if (name == "MainCamera")
						mainCam = newObj.GetID();

					const rapidjson::Value& componentList = go["Components"];
					for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
					{
						const std::string& componentName = (*it)["Component Name"].GetString();
						if (componentName == "struct Carmicah::Transform")
						{
							Transform t;
							t.xPos = static_cast<float>((*it)["xPos"].GetDouble());
							t.yPos = static_cast<float>((*it)["yPos"].GetDouble());
							t.zPos = static_cast<float>((*it)["zPos"].GetDouble());
							t.rot = static_cast<float>((*it)["rot"].GetDouble());
							t.xScale = static_cast<float>((*it)["xScale"].GetDouble());
							t.yScale = static_cast<float>((*it)["yScale"].GetDouble());
							t.isUpdated = true;
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
							t.xSlice = 3;
							t.ySlice = 3;
							t.maxTime = 1.f;
							newObj.AddComponent<Animation>(t);
						}
					}
				}
			}
		}
	}

	void GOFactory::ExportGOs(std::string sceneName)
	{
		std::ofstream ofs{ sceneName, std::ios::binary };
		if (ofs)
		{
			rapidjson::OStreamWrapper osw(ofs);
			rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

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
							Transform& t = o.GetComponent<Transform>();
							writer.String("xPos");
							writer.Double(t.xPos);
							writer.String("yPos");
							writer.Double(t.yPos);
							writer.String("zPos");
							writer.Double(t.zPos);
							writer.String("rot");
							writer.Double(t.rot);
							writer.String("xScale");
							writer.Double(t.xScale);
							writer.String("yScale");
							writer.Double(t.yScale);

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
							
						}
						writer.EndObject();

					}, EntityManager::GetInstance()->GetSignature(o.GetID()));
				writer.EndArray();

				writer.EndObject();
				});
			writer.EndArray();
			ofs.close();
		}

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
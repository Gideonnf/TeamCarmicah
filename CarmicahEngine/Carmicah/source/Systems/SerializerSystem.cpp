/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SerializerSystem.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A system to handle serializing of data and deserializing of data

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "Systems/SerializerSystem.h"
#include "log.h"
#include "Systems/GOFactory.h"
#include "AssetManager.h"

namespace Carmicah
{
	using namespace rapidjson;

	EngineConfig SerializerSystem::LoadConfig(const std::string& filePath)
	{
		std::ifstream ifs{ filePath, std::ios::binary };
		Document doc;
		if (ifs)
		{
			IStreamWrapper isw(ifs);
			doc.ParseStream(isw);
			ifs.close();
		}
		else
		{
			assert("Unable to open config.json");
			//CM_CORE_ERROR("Unable to open config.json");
		}
		EngineConfig enConfig;
		const Value& config = doc;
		enConfig.Width = config["Width"].GetInt();
		enConfig.Height = config["Height"].GetInt();
		enConfig.defaultScene = config["Default Scene"].GetString();
		enConfig.defaultShader = config["Default Shader"].GetString();
		enConfig.debugShader = config["Debug Shader"].GetString();
		enConfig.fontShader = config["Font Shader"].GetString();
		enConfig.assetLoc = config["Asset Loc"].GetString();
		enConfig.fontSize = config["Font Size"].GetInt();
		enConfig.minImgDepth = static_cast<float>(config["Image Min Depth"].GetDouble());
		enConfig.maxImgDepth = static_cast<float>(config["Image Max Depth"].GetDouble());
		enConfig.maxNumTextures = config["Max Textures"].GetInt();
		enConfig.maxTexSize = config["Max Texture Size"].GetInt();
		enConfig.batchRenderSize = config["Batch Render Size"].GetInt();

		
		configFilePath = filePath;
		return enConfig;
	}

	void SerializerSystem::WriteConfig()
	{
		std::ofstream ofs{ configFilePath, std::ios::binary };
		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open config.json");
			return;
		}

		OStreamWrapper osw(ofs);
		PrettyWriter<OStreamWrapper> writer(osw);

		EngineConfig& enConfig = AssetManager::GetInstance()->enConfig;

		writer.StartObject();

		writer.String("Width");
		writer.Int(enConfig.Width);

		writer.String("Height");
		writer.Int(enConfig.Height);

		writer.String("Default Scene");
		writer.String(enConfig.defaultScene.c_str(), static_cast<SizeType>(enConfig.defaultScene.length()));

		writer.String("Default Shader");
		writer.String(enConfig.defaultShader.c_str(), static_cast<SizeType>(enConfig.defaultShader.length()));

		writer.String("Debug Shader");
		writer.String(enConfig.debugShader.c_str(), static_cast<SizeType>(enConfig.debugShader.length()));

		writer.String("Font Shader");
		writer.String(enConfig.fontShader.c_str(), static_cast<SizeType>(enConfig.fontShader.length()));

		writer.String("Font Size");
		writer.Int(enConfig.fontSize);

		writer.String("Asset Loc");
		writer.String(enConfig.assetLoc.c_str(), static_cast<SizeType>(enConfig.assetLoc.length()));

		writer.String("Image Min Depth");
		writer.Double(static_cast<double>(enConfig.minImgDepth));

		writer.String("Image Max Depth");
		writer.Double(static_cast<double>(enConfig.maxImgDepth));

		writer.String("Max Textures");
		writer.Int(enConfig.maxNumTextures);

		writer.String("Max Texture Size");
		writer.Int(enConfig.maxTexSize);

		writer.String("Batch Render Size");
		writer.Int(enConfig.batchRenderSize);

		writer.EndObject();
	}

	bool SerializerSystem::DeserializeScene(std::string sceneFile)
	{
		std::ifstream ifs{ sceneFile, std::ios::binary };
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return false;
		}

		IStreamWrapper isw(ifs);
		Document doc;
		doc.ParseStream(isw);
		ifs.close();

		if (doc.IsNull() || !doc.IsObject())
		{
			CM_CORE_ERROR("Scenefile is empty");
			return false;
		}

		gGOFactory->ImportGO(doc);

		//for (SizeType i = 0; i < doc.Size(); ++i)
		//{
		//	const Value& go = doc[i];
		//	gGOFactory->ImportGO(go);
		//}
		return true;
	}

	bool SerializerSystem::SerializeScene(std::string sceneFile)
	{
		std::ofstream ofs{ sceneFile, std::ios::binary };
		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return false;
		}

		OStreamWrapper osw(ofs);
		PrettyWriter<OStreamWrapper> writer(osw);
		gGOFactory->ExportGOs(writer);
		ofs.close();
		return true;
	}

	void SerializerSystem::SerializePrefab(Prefab prefab)
	{
		// Get the file path to the asset
		std::string filePath = AssetManager::GetInstance()->enConfig.assetLoc + "/Prefabs/" + prefab.mName + ".json";
		std::ofstream ofs{ filePath, std::ios::binary };
		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open prefab file");
			//return false;
		}

		OStreamWrapper osw(ofs);
		PrettyWriter<OStreamWrapper> writer(osw);
		// Serialize prefab
		WritePrefab(prefab, writer);

		ofs.close();
	}

	void SerializerSystem::WritePrefab(Prefab prefab, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
	{
		writer.StartObject();
		
		writer.String("GameObject");
		writer.String(prefab.mName.c_str(), static_cast<rapidjson::SizeType>(prefab.mName.length()));
		
		writer.String("ID");
		writer.Int(prefab.mPrefabID);
		
		writer.String("Components");
		writer.StartArray();
			for (const auto& [name, component] : prefab.mComponents)
			{
				writer.StartObject();
				writer.String("Component Name");
				writer.String(name.c_str(), static_cast<rapidjson::SizeType>(name.length()));
				if (name == typeid(Transform).name())
				{
					std::any_cast<Transform>(component).SerializeComponent(writer);
				}
				if (name == typeid(UITransform).name())
				{
					std::any_cast<UITransform>(component).SerializeComponent(writer);
				}
				if (name == typeid(Renderer).name())
				{
					std::any_cast<Renderer>(component).SerializeComponent(writer);
				}
				if (name == typeid(Collider2D).name())
				{
					std::any_cast<Collider2D>(component).SerializeComponent(writer);
				}
				if (name == typeid(RigidBody).name())
				{
					std::any_cast<RigidBody>(component).SerializeComponent(writer);
				}
				if (name == typeid(Script).name())
				{
					std::any_cast<Script>(component).SerializeComponent(writer);
				}
				if (name == typeid(TextRenderer).name())
				{
					std::any_cast<TextRenderer>(component).SerializeComponent(writer);
				}
				if (name == typeid(PrefabData).name())
				{
					std::any_cast<PrefabData>(component).SerializeComponent(writer);
				}
				writer.EndObject();
			}
		writer.EndArray();

		//writer.String("EntityWatcher");
		//writer.StartArray();
		//for (auto it : prefab.entityWatcher)
		//{
		//	// TODO: I dont know if i should be storing each entity as an object. can try without if the system works next time
		//	writer.StartObject();
		//	writer.Int(it);
		//	writer.EndObject();
		//}
		//writer.EndArray();

		if (prefab.childList.size() > 0)
		{
			writer.String("Children");
			writer.StartArray();
			for (auto it : prefab.childList)
			{
				// write each child
				// this will write the child of the child also
				WritePrefab(it, writer);
			}
			writer.EndArray();
		}

		writer.EndObject();
	}

	Prefab SerializerSystem::DeserializePrefab(std::string prefabFile)
	{
		std::ifstream ifs{ prefabFile, std::ios::binary };
		if (!ifs)
		{
			CM_CORE_ERROR("Error opening prefab file");
		}

		IStreamWrapper isw(ifs);
		Document doc;
		Prefab prefab{};

		doc.ParseStream(isw);
		ifs.close();

		if (doc.HasParseError())
		{
			CM_CORE_ERROR("Failed to load " + prefabFile);
			// return an empty prefab
			return prefab;
		}

		// Get the name of the prefab
		prefab.mName = std::string(doc["GameObject"].GetString());

		CM_CORE_INFO("Creating prefab with name: " + prefab.mName);

		// get the ID attached to the prefab
		prefab.mPrefabID = static_cast<Entity>(doc["ID"].GetInt());

		// Loop through the components and store it into the map
		const rapidjson::Value& componentList = doc["Components"];
		for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
		{
			std::string componentName = (*it)["Component Name"].GetString();
			// Retrieve component data as an std::any object as there can be multiple types of components
			std::any component = ComponentManager::GetInstance()->DeserializePrefabComponent(*it);
			
			//if (componentName == typeid(Transform).name())
			//{
			//	Transform data = std::any_cast<Transform>(component);
			//	std::cout << data.pos.x << std::endl;
			//}
			
			// Insert it into the component map
			prefab.mComponents.insert({ componentName, component });
		}

		if (doc.HasMember("Children"))
		{
			const rapidjson::Value& childrenList = doc["Children"];
			for (rapidjson::Value::ConstValueIterator it = childrenList.Begin(); it != childrenList.End(); ++it)
			{
				prefab.childList.push_back(GetChildren(*it));
			}
		}


		//const Value go = doc;
		return prefab;
	}

	Prefab SerializerSystem::GetChildren(const rapidjson::Value& doc)
	{
		Prefab prefab{};
		// Get the name of the prefab
		prefab.mName = std::string(doc["GameObject"].GetString());

		// get the ID attached to the prefab
		prefab.mPrefabID = static_cast<Entity>(doc["ID"].GetInt());

		CM_CORE_INFO("Creating prefab child with name: " + prefab.mName);

		// Loop through the components and store it into the map
		const rapidjson::Value& componentList = doc["Components"];
		for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
		{
			std::string componentName = (*it)["Component Name"].GetString();
			// Retrieve component data as an std::any object as there can be multiple types of components
			std::any component = ComponentManager::GetInstance()->DeserializePrefabComponent(*it);

			// Insert it into the component map
			prefab.mComponents.insert({ componentName, component });
		}

		if (doc.HasMember("Children"))
		{
			const rapidjson::Value& childrenList = doc["Children"];
			for (rapidjson::Value::ConstValueIterator it = childrenList.Begin(); it != childrenList.End(); ++it)
			{
				prefab.childList.push_back(GetChildren(*it));
			}

		}

		return prefab;
	}

}
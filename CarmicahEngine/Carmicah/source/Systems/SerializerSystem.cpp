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
		enConfig.fontShader = config["Font Shader"].GetString();
		enConfig.assetLoc = config["Asset Loc"].GetString();
		enConfig.fontSize = config["Font Size"].GetInt();
		
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

		writer.String("Font Shader");
		writer.String(enConfig.fontShader.c_str(), static_cast<SizeType>(enConfig.fontShader.length()));

		writer.String("Font Size");
		writer.Int(enConfig.fontSize);

		writer.String("Asset Loc");
		writer.String(enConfig.assetLoc.c_str(), static_cast<SizeType>(enConfig.assetLoc.length()));

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

		//const Value go = doc;
		return prefab;
	}



}
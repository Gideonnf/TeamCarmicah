/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SerializerSystem.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A system to handle serializing of data and deserializing of data

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "Systems/SerializerSystem.h"
#include "log.h"
#include "Systems/GOFactory.h"
#include "AssetManager.h"
#include "TransformSystem.h"

namespace Carmicah
{
	//std::string model = (*it)["model"].GetString();
	/*std::string model = (*it)["model"].GetString();
	document.PushBack(rapidjson::Value(model.c_str(), allocator), allocator);*/
	//std::string texture = (*it)["texture"].GetString();
	//document.PushBack(rapidjson::Value(texture.c_str(), allocator), allocator);

#define PUSH_BACK(iterator, doc, val, allocator) \
if (ValueExist(doc, (*iterator)[val].GetString()) == false) { \
	doc.PushBack(rapidjson::Value((*iterator)[val].GetString(), allocator), allocator);  \
	} \


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

		// As long as config is loaded after every system is initialized
		//auto& transformSystem = SystemManager::GetInstance()->GetSystem<TransformSystem>();
		if (config.HasMember("Collision Layers"))
		{
			const Value& collisionLayers = config["Collision Layers"];
		
			for (const auto& collisionObj : collisionLayers.GetArray())
			{
				int index = collisionObj["Layer Index"].GetInt();
				enConfig.savedLayerArr[index] = collisionObj["Layer Mask"].GetUint();
			}
		}
		else
		{
			for (int i = 0; i < MAX_LAYERS; ++i)
			{
				// set default all bits to 1
				enConfig.savedLayerArr[i] = 0xFFFFFFFF;
			}
		}

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
		const auto& transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();

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

		writer.String("Collision Layers");
		writer.StartArray();

		for (int i = 0; i < transformSys->GetMaxLayers(); ++i)
		{
			writer.StartObject();
			writer.String("Layer Index");
			writer.Int(i);
			writer.String("Layer Mask");
			writer.Uint(transformSys->GetLayerMap()[i]);
			writer.EndObject();
		}

		writer.EndArray();

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

#ifdef CM_INSTALLER
		DeserializeLevelAssets(sceneFile);
#endif

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

		SerializeLevelAssets(sceneFile);
		ofs.close();
		return true;
	}

	
	void SerializerSystem::SerializeLevelAssets(std::string sceneFile)
	{
		std::ifstream ifs{ sceneFile, std::ios::binary };
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return;
		}

		IStreamWrapper isw(ifs);
		Document doc;
		doc.ParseStream(isw);
		ifs.close();

		rapidjson::Document document;
		document.SetArray();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		if (doc.HasMember("SceneObjects"))
		{
			// Loop through every scene object to check what assets it need
			const rapidjson::Value& sceneObjects = doc["SceneObjects"];
			for (rapidjson::SizeType i = 0; i < sceneObjects.Size(); ++i)
			{
				const rapidjson::Value& go = sceneObjects[i];
				//writer.StartObject();D
				// Lop through all the components in the game object
				const rapidjson::Value& componentList = go["Components"];
				for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
				{
					// get the name of the component
					const std::string& componentName = (*it)["Component Name"].GetString();

					// check for specific type of components that need to load assets
					if (componentName == typeid(Renderer).name())
					{
						PUSH_BACK(it, document, "model", allocator);
						//PUSH_BACK(it, document, "texture", allocator);


						/*std::string model = (*it)["model"].GetString();
						document.PushBack(rapidjson::Value(model.c_str(), allocator), allocator);*/
						//
						std::string texture = (*it)["texture"].GetString();
						Texture t = AssetManager::GetInstance()->GetAsset<Texture>(texture);
						if (t.isSpriteSheet && t.spriteSheet.empty() == false)
						{
							if (ValueExist(document, t.spriteSheet.c_str()) == false)
							{
								document.PushBack(rapidjson::Value(t.spriteSheet.c_str(), allocator), allocator);
							} 
						}
						else
						{
							PUSH_BACK(it, document, "texture", allocator);
						}
						//document.PushBack(rapidjson::Value(texture.c_str(), allocator), allocator);

					}
					else if (componentName == typeid(Script).name())
					{
						// TODO: Find out how i can create custom data types for C# side
						// so i can find out if they're loading a prefab
						// or if they're loading an animation
					}
					else if (componentName == typeid(Animation).name())
					{
						PUSH_BACK(it, document, "Atlas", allocator);
						//std::string animAtlas = (*it)["Atlas"].GetString();
						//document.PushBack(rapidjson::Value(animAtlas.c_str(), allocator), allocator);

					}
					else if (componentName == typeid(TextRenderer).name())
					{
						PUSH_BACK(it, document, "font", allocator);
						//std::string font = (*it)["font"].GetString();
						//document.PushBack(rapidjson::Value(font.c_str(), allocator), allocator);

					}
				}
				//sceneGO.children.insert(ImportEntity(go, sceneGO.sceneID));
			}
		}
		/*writer.EndArray();
		writer.EndObject();*/

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		std::filesystem::path filePath(sceneFile);
		// extract out file name "sceneX/Y/Z"
		std::string fileName = filePath.stem().string();
		// extract out the "../Assets/.."
		std::string directory = filePath.parent_path().string();
		// concat together to make the asset file
		std::string assetFile = directory + "\\" + fileName + ".Asset";

		std::ofstream ofs{ assetFile, std::ios::binary };

		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return;
		}

		ofs << buffer.GetString();
		ofs.close();
	}

	void SerializerSystem::DeserializeLevelAssets(std::string sceneFile)
	{
		std::filesystem::path filePath(sceneFile);
		// extract out file name "sceneX/Y/Z"
		std::string fileName = filePath.stem().string();
		// extract out the "../Assets/.."
		std::string directory = filePath.parent_path().string();
		// concat together to make the asset file
		std::string assetFile = directory + "\\" + fileName + ".Asset";

		std::ifstream ifs{ assetFile, std::ios::binary };
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return;
		}

		std::stringstream buffer;
		buffer << ifs.rdbuf();
		std::string jsonStr = buffer.str();
		ifs.close();

		rapidjson::Document document;
		if (document.Parse(jsonStr.c_str()).HasParseError())
		{
			// error in parsing
			CM_CORE_ERROR("Unable to parse level assets");
			return;
		}

		// loop thru the document
		for (auto& asset : document.GetArray())
		{
			if (asset.IsString())
			{
				CM_CORE_INFO("Asset name {}", asset.GetString());
				
				// push the asset to load into the scene's vector of assets to load
				AssetManager::GetInstance()->enConfig.assetsToLoad[fileName].push_back(asset.GetString());
				//AssetManager::GetInstance()->LoadAsset()
			}
		}

		AssetManager::GetInstance()->fileWatcher.LoadSceneFiles(fileName);
	}

	bool SerializerSystem::ValueExist(const rapidjson::Document& doc, const char* valToCheck)
	{
		/*for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
		{
			if (doc[i].IsString() && doc[i] == valToCheck)
			{
				return true;
			}
		}*/

		if (doc.IsArray() == false)
		{
			return false;
		}

		for (auto& asset : doc.GetArray())
		{
			if (asset.IsString() && asset == valToCheck)
			{
				return true;
			}
		}

		return false;
	}

	void SerializerSystem::SerializePrefab(Prefab prefab)
	{
		// Get the file path to the asset
		std::string filePath = AssetManager::GetInstance()->enConfig.assetLoc + "/Prefabs/" + prefab.mName + ".prefab";
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
				if (name == typeid(Animation).name())
				{
					std::any_cast<Animation>(component).SerializeComponent(writer);

				}
				if (name == typeid(Button).name())
				{
					std::any_cast<Button>(component).SerializeComponent(writer);
				}
				if (name == typeid(Sound).name())
				{
					std::any_cast<Sound>(component).SerializeComponent(writer);
				}
				if (name == typeid(StateMachine).name())
				{
					std::any_cast<StateMachine>(component).SerializeComponent(writer);

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
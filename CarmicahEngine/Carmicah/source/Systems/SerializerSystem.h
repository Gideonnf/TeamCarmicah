/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SerializerSystem.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A system to handle serializing of data and deserializing of data

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef SERIALIZER_SYSTEM_H
#define SERIALIZER_SYSTEM_H
#include "Singleton.h"
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include "CarmicahCore.h"
#include "Systems/AssetTypes.h"


//berif:         This class is a system
//                 that handles the serializing of data and deserializing of data
namespace Carmicah
{
	class SerializerSystem : public Singleton<SerializerSystem>
	{
	private:
		//rapidjson::IStreamWrapper isw;
		//rapidjson::Writer<rapidjson::IStreamWrapper> mInWritter;

		//rapidjson::OStreamWrapper osw;
		//rapidjson::Writer<rapidjson::OStreamWrapper> mOutWritter;
		std::string configFilePath;

	public:
		// Read in config file from asset manager

		/*
		brief
			Loads the config data
		param[filePath]
			filepath to load from
		return
			EngineConfig
		*/
		EngineConfig LoadConfig(const std::string& filePath);

		/*
		brief
			writes to the config
		*/
		void WriteConfig();

		/// <summary>
		/// Deserialize the scene and load all the entities in it
		/// </summary>
		/// <param name="">Name of the scene to deserialize</param>
		/// <returns>true if deserialize, false if cant</returns>
		bool DeserializeScene(std::string);
		/// <summary>
		/// Serialize the scene/Save the scene.
		/// </summary>
		/// <param name="">Name of the scene to write into</param>
		/// <returns>True or false depending on success</returns>
		bool SerializeScene(std::string);
		/// <summary>
		/// Serialize the prefab if user made any changes to it in the editor
		/// </summary>
		/// <param name="">Name of the prefab</param>
		void SerializePrefab(Prefab);
		/// <summary>
		/// Used for recursively writing the prefab into the ostream wrapper. Goes to the children and recursively calls itself
		/// until it serializes all the children, children of children, etc
		/// </summary>
		/// <param name="">Prefab that is being saved</param>
		/// <param name="">Wrapper to write into</param>
		void WritePrefab(Prefab, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&);
		/// <summary>
		/// Deserialize the prefab and store it as a prefab type that can be used to create prefabs by GOFactory
		/// </summary>
		/// <param name="">Name of the prefab file</param>
		/// <returns>Returns the prefab after deserializing</returns>
		Prefab DeserializePrefab(std::string);
		/// <summary>
		/// To dig into the children of prefabs, and the children of the children, etc
		/// </summary>
		/// <param name="doc">Document to deserialize from</param>
		/// <returns>the prefab after deserializing</returns>
		Prefab GetChildren(const rapidjson::Value& doc);

		/// <summary>
		/// Read through the scene file and extract out any assets that are to be loaded
		/// So that when we load the level in installer mode, it will only load those assets
		/// </summary>
		/// <param name="sceneFile"></param>
		void SerializeLevelAssets(std::string sceneFile);

		/// <summary>
		/// Deserialize the assets and store it in enconfig so that file watcher can pull it out?
		/// or call the function in file watcher itself
		/// </summary>
		/// <param name="scene"></param>
		void DeserializeLevelAssets(std::string scene);

	};

	static SerializerSystem& Serializer = *SerializerSystem::GetInstance();

};


#endif
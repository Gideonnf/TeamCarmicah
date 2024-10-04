/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SerializerSystem.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A system to handle serializing of data and deserializing of data

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
#include "AssetManager.h"


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
		EngineConfig LoadConfig(const std::string& filePath);
		void WriteConfig();
		bool DeserializeScene(std::string);
		bool SerializeScene(std::string);
		Prefab DeserializePrefab(std::string);


	};

	static SerializerSystem& Serializer = *SerializerSystem::GetInstance();

};


#endif
#ifndef SERIALIZER_SYSTEM_H
#define SERIALIZER_SYSTEM_H
#include "Singleton.h"
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include "CarmicahCore.h"
#include "../AssetManager.h"

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
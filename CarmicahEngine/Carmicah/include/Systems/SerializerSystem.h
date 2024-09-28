#ifndef SERIALIZER_SYSTEM_H
#define SERIALIZER_SYSTEM_H
#include "Singleton.h"
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>
#include "CarmicahCore.h"

namespace Carmicah
{
	class SerializerSystem : public Singleton<SerializerSystem>
	{
	private:
		//rapidjson::IStreamWrapper isw;
		//rapidjson::Writer<rapidjson::IStreamWrapper> mInWritter;

		//rapidjson::OStreamWrapper osw;
		//rapidjson::Writer<rapidjson::OStreamWrapper> mOutWritter;

	public:
		// Read in config file from asset manager
		void LoadConfig(Application&);
		void WriteConfig(Application&);
		void DeserializeScene(std::string);
		void SerializeScene(std::string);

	};

	static SerializerSystem& Serializer = *SerializerSystem::GetInstance();

};


#endif
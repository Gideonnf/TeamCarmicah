#include "pch.h"
#include "Systems/SerializerSystem.h"
#include "log.h"
#include "Systems/GOFactory.h"
namespace Carmicah
{
	using namespace rapidjson;

	void SerializerSystem::LoadConfig(Application& app)
	{
		std::ifstream ifs{ "../Assets/config.json", std::ios::binary };
		Document doc;
		if (ifs)
		{
			IStreamWrapper isw(ifs);
			doc.ParseStream(isw);
			ifs.close();
		}
		else
		{
			CM_CORE_ERROR("Unable to open config.json");
			return;
		}
		const Value& config = doc;
		app.Width = config["Width"].GetInt();
		app.Height = config["Height"].GetInt();
		app.defaultScene = config["Default Scene"].GetString();
	}

	void SerializerSystem::WriteConfig(Application& app)
	{
		std::ofstream ofs{ "../Assets/config.json", std::ios::binary };
		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open config.json");
			return;
		}

		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);

		writer.StartObject();

		writer.String("Width");
		writer.Int(app.Width);

		writer.String("Height");
		writer.Int(app.Height);

		writer.String("Default Scene");
		writer.String(app.defaultScene.c_str(), static_cast<SizeType>(app.defaultScene.length()));

		writer.EndObject();
	}

	void SerializerSystem::DeserializeScene(std::string sceneFile)
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

		for (SizeType i = 0; i < doc.Size(); ++i)
		{
			const Value& go = doc[i];
			gGOFactory->ImportGO(go);
		}

	}

	void SerializerSystem::SerializeScene(std::string sceneFile)
	{
		std::ofstream ofs{ sceneFile, std::ios::binary };
		if (!ofs)
		{
			CM_CORE_ERROR("Unable to open scene file");
			return;
		}

		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		gGOFactory->ExportGOs(writer);
	}


}
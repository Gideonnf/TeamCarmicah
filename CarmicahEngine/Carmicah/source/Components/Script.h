#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include "BaseComponent.h"
struct Script : BaseComponent<Script>
{
	std::string scriptName;

	Script& DeserializeComponent(const rapidjson::Value& component) override
	{
		scriptName = component["scriptName"].GetString();
		return *this;
	}

	void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
	{
		writer.String("scriptName");
		writer.String(scriptName.c_str());

	}
};

#endif

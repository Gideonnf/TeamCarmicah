#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include "BaseComponent.h"
#include <variant>
struct Script : BaseComponent<Script>
{
	using variantVar = std::variant<int, float, bool, std::string>;
	std::string scriptName;
	std::unordered_map<std::string, variantVar> scriptableFieldMap;

	Script& DeserializeComponent(const rapidjson::Value& component) override
	{
		scriptName = component["scriptName"].GetString();
		//const rapidjson::Value& fieldList = component["ScriptableFieldMap"];
		//for (rapidjson::Value::ConstValueIterator it = fieldList.Begin(); it != fieldList.End(); ++it)
		//{
		//	std::string varName = (*it)["FieldName"].GetString();
		//	if ((*it)[varName.c_str()].IsBool())
		//	{

		//	}
		//	else if ((*it)[varName.c_str()].IsFloat())
		//	{

		//	}
		//	else if ((*it)[varName.c_str()].IsInt())
		//	{

		//	}
		//	else if ((*it)[varName.c_str()].IsString())
		//	{

		//	}
		//}
		return *this;
	}

	void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
	{
		writer.String("scriptName");
		writer.String(scriptName.c_str());
		writer.String("ScriptableFieldMap");
		writer.StartArray();
		for (const auto& [name, variant] : scriptableFieldMap)
		{
			writer.StartObject();
			writer.String("FieldName");
			writer.String(name.c_str(), static_cast<rapidjson::SizeType>(name.length()));
			std::visit([](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, int>)
				{

				}
				else if constexpr (std::is_same_v<T, float>)
				{

				}
				else if constexpr (std::is_same_v<T, bool>)
				{

				}
				else if constexpr (std::is_same_v<T, std::string>)
				{

				}
			}, variant);
			writer.EndObject();
		}
		writer.EndArray();

	}
};

#endif

/* File Documentation -----------------------------------------------------------------------------
file:           Script.h

author:			Gideon Francis (100%)

email:          g.francis@digipen.edu

brief:          This file defines the Script structure, inheriting from BaseComponent. It provides
				functionality to serialize and deserialize script-related data, including script
				names and customizable scriptable fields stored in a map. The fields can have
				various types like int, float, bool, and string, leveraging std::variant for storage.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


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
		if (component.HasMember("ScriptableFieldMap"))
		{
			const rapidjson::Value& fieldList = component["ScriptableFieldMap"];

			for (const auto& fieldObject : fieldList.GetArray()) 
			{
				// Process all key-value pairs within the object
				for (auto it = fieldObject.MemberBegin(); it != fieldObject.MemberEnd(); ++it)
				{
					std::string fieldName = it->name.GetString();
					variantVar var; 

					if (it->value.IsString())
					{
						var = std::string(it->value.GetString());
					}
					else if (it->value.IsInt())
					{
						var = it->value.GetInt();
					}
					else if (it->value.IsFloat()) 
					{
						var = it->value.GetFloat();
					}
					else if (it->value.IsBool()) 
					{
						var = it->value.GetBool();
					}

					// Insert the key-value pair into the scriptableFieldMap
					scriptableFieldMap.insert({ fieldName, var });
				}
			}
		}
		return *this;
	}

	void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
	{
		writer.String("scriptName");
		writer.String(scriptName.c_str());
		writer.String("ScriptableFieldMap");
		writer.StartArray();
		writer.StartObject();
		for (const auto& [name, variant] : scriptableFieldMap)
		{
			writer.String(name.c_str(), static_cast<rapidjson::SizeType>(name.length()));
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, int>)
				{
					writer.Int(arg);
				}
				else if constexpr (std::is_same_v<T, float>)
				{
					writer.Double(arg);
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					writer.Bool(arg);
				}
				else if constexpr (std::is_same_v<T, std::string>)
				{
					std::string var = arg;
					writer.String(var.c_str());
				}
			}, variant);
		}
		writer.EndObject();
		writer.EndArray();

	}
};

#endif

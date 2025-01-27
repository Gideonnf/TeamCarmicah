/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:      BaseComponent.h

 author:	Gideon Francis (100%)

 email:     g.francis@digipen.edu

 brief:     A base component that components inherit. Only here to simplify deserializing and serializing of 
			components before runtime reflection can be implemented

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include "rapidjson/document.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <variant>

#define DESERIALIZE_IF_HAVE(var, component, member, accessor, type) \
	if (component.HasMember(member)) {															\
				var = static_cast<type>(component[member].accessor());					\
	}																													\

using variantVar = std::variant<int, float, bool, std::string>;


// For now until reflection is done, each component has to have their own function to serialize and deserialize their data
template <typename derived>
class BaseComponent
{
public:
	virtual derived& DeserializeComponent(const rapidjson::Value& component) = 0;
	virtual void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) = 0;

	/// <summary>
	/// So that ppl don't jhave to keep doing .HasMember check and then get the var
	/// this just checks and returns it if it exist
	/// </summary>
	/// <typeparam name="T">Type of variable being retrieved</typeparam>
	/// <param name="component">Reference to the rapidjson variable</param>
	/// <param name="str">name of the variable</param>
	/// <returns> the variable from json </returns>
	template <typename T>
	inline T GetVariable(const rapidjson::Value& component, const char* str)
	{
		if (component.HasMember(str))
		{
			if constexpr (std::is_same_v<T, std::string>) {
				return component[str].GetString();
			}
			else if constexpr (std::is_same_v<T, int>) {
				return component[str].GetInt();
			}
			else if constexpr (std::is_same_v<T, float>) {
				return component[str].GetFloat();
			}
			else if constexpr (std::is_same_v<T, bool>) {
				return component[str].GetBool();
			}

			// if doesn't exist then return a blank T
		}
		return T{};
	}

	variantVar ReadVariant(std::string varName, const rapidjson::Value& component)
	{
		variantVar var{};
		if (component.HasMember(varName.c_str()))
		{
			const auto& value = component[varName.c_str()];
			if (value.IsInt())
			{
				var = value.GetInt();
			}
			else if (value.IsFloat())
			{
				var = value.GetFloat();
			}
			else if (value.IsString())
			{
				var = value.GetString();
			}
			else if (value.IsBool())
			{
				var = value.GetBool();
			}
		}

		return var;
	}

	void WriteVariant(variantVar var, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
	{
		std::visit([&](auto&& value) 
		{
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, int>)
			{
				writer.Int(value);
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				writer.Double(value);
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				writer.Bool(value);
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				std::string var = value;
				writer.String(var.c_str());
			}
		}, var);
	}
};


#endif

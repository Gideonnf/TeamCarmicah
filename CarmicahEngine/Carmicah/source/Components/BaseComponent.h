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

// For now until reflection is done, each component has to have their own function to serialize and deserialize their data
template <typename derived>
class BaseComponent
{
public:
	virtual derived& DeserializeComponent(const rapidjson::Value& component) = 0;
	virtual void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) = 0;
};

#endif

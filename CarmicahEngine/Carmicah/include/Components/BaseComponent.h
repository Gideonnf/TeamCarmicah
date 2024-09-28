#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include "rapidjson/document.h"


// For now until reflection is done, each component has to have their own function to serialize and deserialize their data
template <typename derived>
class BaseComponent
{
public:
	virtual derived& DeserializeComponent(const rapidjson::Value& component) = 0;
};

#endif

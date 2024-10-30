#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Math/Matrix3x3.h"
#include "BaseTransform.h"

namespace Carmicah
{
    struct Transform : BaseTransform<Transform>
    {
        float rot;

        Matrix3x3<float> worldSpace;
        Matrix3x3<float> camSpace;

        Entity parentID; // Hold -1 if no parent
        std::vector<Entity> childIDs;

        Transform& DeserializeComponent(const rapidjson::Value& component) override
        {           
            BaseTransform::DeserializeComponentBuffer(component);
            rot = static_cast<float>(component["rot"].GetDouble());

            if (component.HasMember("parentID"))
            {
                parentID = static_cast<int>(component["parentID"].GetInt());
            }

            if (component.HasMember("childIDs"))
            {
                const rapidjson::Value& childList = component["childIDs"];

                for (rapidjson::Value::ConstValueIterator it = childList.Begin(); it != childList.End(); ++it)
                {
                    childIDs.push_back(it->GetInt());
                }
            }

            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            BaseTransform::SerializeComponent(writer);
            writer.String("rot");
            writer.Double(rot);
        }
    };
}

#endif
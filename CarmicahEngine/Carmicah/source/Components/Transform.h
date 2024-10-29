#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace Carmicah
{
    struct Transform : BaseComponent<Transform>
    {
        float xPos;
        float yPos;
        float zPos;

        float rot;

        float xScale;
        float yScale;

        bool notUpdated;
        glm::mat3 worldSpace;
        glm::mat3 camSpace;

        Entity parentID; // Hold -1 if no parent
        std::vector<Entity> childIDs;

        Transform& DeserializeComponent(const rapidjson::Value& component) override
        {           
            xPos = static_cast<float>(component["xPos"].GetDouble());
            yPos = static_cast<float>(component["yPos"].GetDouble());
            zPos = static_cast<float>(component["zPos"].GetDouble());
            rot = static_cast<float>(component["rot"].GetDouble());
            xScale = static_cast<float>(component["xScale"].GetDouble());
            yScale = static_cast<float>(component["yScale"].GetDouble());

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
            writer.String("xPos");
            writer.Double(xPos);
            writer.String("yPos");
            writer.Double(yPos);
            writer.String("zPos");
            writer.Double(zPos);
            writer.String("rot");
            writer.Double(rot);
            writer.String("xScale");
            writer.Double(xScale);
            writer.String("yScale"); 
            writer.Double(yScale);
            
            writer.String("parentID");
            writer.Int(parentID);

            writer.String("childIDs");
            writer.StartArray();
            for (size_t i = 0; i < childIDs.size(); ++i)
            {
                writer.Int(childIDs[i]);
            }
            writer.EndArray();
        }
    };
}

#endif
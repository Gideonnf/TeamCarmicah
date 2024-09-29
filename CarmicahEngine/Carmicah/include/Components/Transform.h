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

        bool isUpdated;
        glm::mat3 worldSpace;
        glm::mat3 camSpace;

        Transform& DeserializeComponent(const rapidjson::Value& component) override
        {           
            xPos = static_cast<float>(component["xPos"].GetDouble());
            yPos = static_cast<float>(component["yPos"].GetDouble());
            zPos = static_cast<float>(component["zPos"].GetDouble());
            rot = static_cast<float>(component["rot"].GetDouble());
            xScale = static_cast<float>(component["xScale"].GetDouble());
            yScale = static_cast<float>(component["yScale"].GetDouble());
            isUpdated = true;

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

        }
    };
}

#endif
#pragma once
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace Carmicah
{
    struct Renderer : BaseComponent<Renderer>
    {
        std::string model;
        std::string texture;
        glm::mat3 texureMat;

        Renderer& DeserializeComponent(const rapidjson::Value& component) override
        {
            model = component["model"].GetString();
            texture = component["texture"].GetString();
            texureMat = glm::mat3(1);
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("model");
            writer.String(model.c_str());
            writer.String("texture");
            writer.String(texture.c_str());
        }
    };
}

#endif
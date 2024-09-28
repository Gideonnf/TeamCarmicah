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
            return *this;
        }

        void SerializeComponent(rapidjson::Writer<rapidjson::OStreamWrapper>& writer) override
        {

        }
    };
}

#endif
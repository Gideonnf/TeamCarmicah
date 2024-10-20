/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Renderer.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of Rendering specific details such as the model being used, texture, and a mat3 for accessing parts of the texture

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
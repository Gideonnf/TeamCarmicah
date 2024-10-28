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
#include "BaseComponent.h"
#include "Math/Vec2.h"
#include "Math/Matrix3x3.h"

namespace Carmicah
{
    struct Renderer : BaseComponent<Renderer>
    {
        std::string model;
        std::string texture;
        Matrix3x3<float> textureMat;

        Renderer& DeserializeComponent(const rapidjson::Value& component) override
        {
            model = component["model"].GetString();
            texture = component["texture"].GetString();
            Mtx33Identity(textureMat);
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
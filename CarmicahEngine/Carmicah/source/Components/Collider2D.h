/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Collider2D.h

 author:		Lee Yong Yee(100%)

 email:			l.yongyee@digipen.edu

 brief:			Keeps track of Rendering specific details such as the model being used, texture, and a mat3 for accessing parts of the texture

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#include <Math/Vec2.h>
#include <string>
#include "BaseComponent.h"

namespace Carmicah
{
    struct Collider2D : BaseComponent<Collider2D>
    {
        Carmicah::Vector2D<float> min;
        Carmicah::Vector2D<float> max;

        float localScale = 1.0f;
        float customWidth;
        float customRotation;
        float customHeight;

        bool OBBinit;

        std::vector<Vec2f> objVert;
        std::vector<Vec2f> objNormals;
        std::vector<Vec2f> objEdges;

        std::string shape;

        Collider2D& DeserializeComponent(const rapidjson::Value& component) override
        {
            shape = component["shape"].GetString();
            customWidth = static_cast<float>(component["customWidth"].GetDouble());
            customRotation = static_cast<float>(component["customRotation"].GetDouble());
            customHeight = static_cast<float>(component["customHeight"].GetDouble());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("shape");
			writer.String(shape.c_str());
            writer.String("customWidth");
            writer.Double(customWidth);
            writer.String("customRotation");
            writer.Double(customRotation);
            writer.String("customHeight");
            writer.Double(customHeight);
        }
    };
}

#endif
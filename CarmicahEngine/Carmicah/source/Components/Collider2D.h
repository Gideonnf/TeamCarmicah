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
        std::vector<Vec2f> objVert;
        std::vector<Vec2f> objNormals;
        std::vector<Vec2f> objEdges;

        bool dirty;

        std::string shape;

        Collider2D& DeserializeComponent(const rapidjson::Value& component) override
        {
            shape = component["shape"].GetString();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("shape");
			writer.String(shape.c_str());
        }
    };
}

#endif
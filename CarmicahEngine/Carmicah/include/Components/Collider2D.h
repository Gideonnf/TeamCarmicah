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
        std::string shape;

        Collider2D& DeserializeComponent(const rapidjson::Value& component) override
        {
            min.x = static_cast<float>(component["minX"].GetDouble());
            min.y = static_cast<float>(component["minY"].GetDouble());
            max.x = static_cast<float>(component["maxX"].GetDouble());
            max.y = static_cast<float>(component["maxY"].GetDouble());
            shape = component["shape"].GetString();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("minX");
			writer.Double(min.x);
			writer.String("minY");
			writer.Double(min.y);
			writer.String("maxX");
			writer.Double(max.x);
			writer.String("maxY");
			writer.Double(max.y);
			writer.String("shape");
			writer.String(shape.c_str());
        }
    };
}

#endif
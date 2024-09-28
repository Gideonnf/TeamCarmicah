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
            return *this;
        }

        void SerializeComponent(rapidjson::Writer<rapidjson::OStreamWrapper>& writer) override
        {

        }
    };
}

#endif
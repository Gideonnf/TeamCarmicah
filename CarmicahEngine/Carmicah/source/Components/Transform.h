#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Math/Matrix3x3.h"
#include "BaseTransform.h"

namespace Carmicah
{
    struct Transform : BaseTransform<Transform>
    {
        float rot;

        Matrix3x3<float> worldSpace;
        Matrix3x3<float> localSpace;

        Transform& DeserializeComponent(const rapidjson::Value& component) override
        {           
            BaseTransform::DeserializeComponentBuffer(component);
            rot = static_cast<float>(component["rot"].GetDouble());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            BaseTransform::SerializeComponent(writer);
            writer.String("rot");
            writer.Double(rot);
        }
    };
}

#endif
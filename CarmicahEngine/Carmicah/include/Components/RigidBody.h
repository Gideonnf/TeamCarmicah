#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Math/Vec2.h>
#include <string>
#include "BaseComponent.h"

namespace Carmicah
{
    struct RigidBody : BaseComponent<RigidBody>
    {
        Vector2D<float> velocity;
        Vector2D<float> posPrev;

        float gravity;

        float zposPrev;

        std::string objectType;

        RigidBody& DeserializeComponent(const rapidjson::Value& component) override
        {
            velocity.x = static_cast<float>(component["velocityX"].GetDouble());
            velocity.y = static_cast<float>(component["velocityY"].GetDouble());
            posPrev.x = static_cast<float>(component["posPrevX"].GetDouble());
            posPrev.y = static_cast<float>(component["posPrevY"].GetDouble());
            gravity = static_cast<float>(component["gravity"].GetDouble());
            zposPrev = static_cast<float>(component["zposPrev"].GetDouble());
            objectType = (component["ObjectType"].GetString());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("velocityX");
			writer.Double(velocity.x);
			writer.String("velocityY");
			writer.Double(velocity.y);
			writer.String("posPrevX");
			writer.Double(posPrev.x);
			writer.String("posPrevY");
			writer.Double(posPrev.y);
            writer.String("gravity");
            writer.Double(gravity);
			writer.String("zposPrev");
			writer.Double(zposPrev);
            writer.String("ObjectType");
			writer.String(objectType.c_str());
        }
    };
}

#endif
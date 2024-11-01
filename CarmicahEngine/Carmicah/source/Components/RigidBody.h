/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        RigidBody.h

 author:	   Lee Yong Yee(80%)
 author:	   Gideon Francis(10%)

 email:        l.yongyee@digipen.edu, g.francis@digipen.edu

 brief:        Data members for the rigid body component. Contains and Serialize

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Math/Vec2.h>
#include <string>
#include "Systems/ForcesManager.h"
#include "BaseComponent.h"

namespace Carmicah
{
    struct RigidBody : BaseComponent<RigidBody>
    {
        Vector2D<float> velocity;
        Vector2D<float> posPrev;
        
        Vector2D<float> acceleration;

        float mass;

        float gravity;

        ForcesManager forcesManager;

        float zposPrev;

        std::string objectType;

        RigidBody& DeserializeComponent(const rapidjson::Value& component) override
        {
            velocity.x = static_cast<float>(component["velocityX"].GetDouble());
            velocity.y = static_cast<float>(component["velocityY"].GetDouble());
            posPrev.x = static_cast<float>(component["posPrevX"].GetDouble());
            posPrev.y = static_cast<float>(component["posPrevY"].GetDouble());
            mass = static_cast<float>(component["mass"].GetDouble());
            gravity = static_cast<float>(component["gravity"].GetDouble());
            zposPrev = static_cast<float>(component["zposPrev"].GetDouble());
            objectType = (component["objectType"].GetString());
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
            writer.String("mass");
            writer.Double(mass);
            writer.String("gravity");
            writer.Double(gravity);
			writer.String("zposPrev");
			writer.Double(zposPrev);
            writer.String("objectType");
			writer.String(objectType.c_str());
        }
    };
}

#endif
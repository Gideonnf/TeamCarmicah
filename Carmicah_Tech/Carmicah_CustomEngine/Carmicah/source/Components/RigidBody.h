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
#include "Physics/ForcesManager.h"
#include "BaseComponent.h"

namespace Carmicah
{
    enum rbTypes
    {
        STATIC = 0,
        KINEMATIC,
        DYNAMIC,
        MAX_TYPES
    };

    struct RigidBody : BaseComponent<RigidBody>
    {
        Vector2D<float> velocity;
        float angularVelocity;
        Vector2D<float> posPrev;
        
        Vector2D<float> acceleration;

        float angularAcceleration;

        float mass;

        float inertiaMass;

        float gravity;

        ForcesManager forcesManager;

      //  bool triggerCollide{};

        bool collided{};

        float zposPrev;

        rbTypes objectType;

        RigidBody() : velocity{ 0, 0 }, angularVelocity{ 0 }, mass{ 1 }, gravity{ 0.0f }, objectType{ rbTypes::STATIC }, collided{ false }, angularAcceleration{ 0.0f }, inertiaMass{ 0.0f }, zposPrev{ 0.0f } {}

        RigidBody& DeserializeComponent(const rapidjson::Value& component) override
        {
            velocity.x = static_cast<float>(component["velocityX"].GetDouble());
            velocity.y = static_cast<float>(component["velocityY"].GetDouble());
            if (component.HasMember("AngularVelocity"))
                angularVelocity = static_cast<float>(component["angularVelocity"].GetDouble());
            posPrev.x = static_cast<float>(component["posPrevX"].GetDouble());
            posPrev.y = static_cast<float>(component["posPrevY"].GetDouble());
            mass = static_cast<float>(component["mass"].GetDouble());
            gravity = static_cast<float>(component["gravity"].GetDouble());
            zposPrev = static_cast<float>(component["zposPrev"].GetDouble());
            objectType = static_cast<rbTypes>(component["objectType"].GetInt());

            // If it has linear forces that is applied
            if (component.HasMember("linearForces"))
            {
                const rapidjson::Value& forceList = component["linearForces"];
                for (rapidjson::Value::ConstValueIterator it = forceList.Begin(); it != forceList.End(); ++it)
                {
                    Vec2f dir;
                    dir.x = static_cast<float>((*it)["unitDirX"].GetDouble());
                    dir.y = static_cast<float>((*it)["unitDirY"].GetDouble());
                    float magnitude = static_cast<float>((*it)["magnitude"].GetDouble());
                    float lifeTime = static_cast<float>((*it)["lifetime"].GetDouble());
                    float age = static_cast<float>((*it)["age"].GetDouble());

                    // Add it to this shit
                    LinearDirectionalForce newForce{ dir, magnitude, lifeTime};
                    forcesManager.AddLinearForce(newForce);
                }
            }

            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("velocityX");
			writer.Double(velocity.x);
			writer.String("velocityY");
			writer.Double(velocity.y);
            writer.String("angularVelocity");
            writer.Double(angularVelocity);
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
			writer.Int((int)objectType);
            if (forcesManager.GetLinearForces().size() > 0)
            {
                // TODO: Check with yy if she wants non-active forces to be serialized
                writer.String("linearForces");
                writer.StartArray();
                for (const auto& it : forcesManager.GetLinearForces())
                {
                    writer.StartObject();

                    writer.String("unitDirX");
                    writer.Double(it.unitDirection.x);
                    writer.String("unitDirY");
                    writer.Double(it.unitDirection.y);
                    writer.String("magnitude");
                    writer.Double(it.magnitude);
                    writer.String("lifetime");
                    writer.Double(it.lifetime);
                    writer.String("age");
                    writer.Double(it.age);

                    writer.EndObject();
                }
                writer.EndArray();
            }
            
        }
    };
}

#endif
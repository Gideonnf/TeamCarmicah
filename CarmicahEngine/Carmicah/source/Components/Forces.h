/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        Forces.h

 author:	   Lee Yong Yee(80%)
 author:	   Gideon Francis(10%)

 email:        l.yongyee@digipen.edu, g.francis@digipen.edu

 brief:        Data members for the rigid body component. Contains and Serialize

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef FORCES_H
#define FORCES_H

#include <Math/Vec2.h>
#include <string>
#include "BaseComponent.h"

namespace Carmicah
{
    struct DragForce
    {
        float directionalDrag;
        float lifetime;
        float age = 0.0f;
        bool isActive = true;

        void Update(float deltaTime, Vector2D<float>& velocity) 
        {
            if (isActive == false) 
            {
                return;
            }
            else
            {
                velocity *= (1.0f - directionalDrag * deltaTime);

                age += deltaTime;

                if (age >= lifetime) 
                {
                    isActive = false;
                }
            }
        }
    };

    struct LinearDirectionalForce
    {
        Vector2D<float> unitDirection;
        float magnitude;
        float lifetime;
        float age = 0.0f;
        bool isActive = true;

        Vector2D<float> GetForceVector() const 
        {
            return unitDirection * magnitude;
        }

        void Update(float deltaTime) 
        {
            age += deltaTime;

            if (age >= lifetime) 
            {
                isActive = false;
            }
        }
    };

    struct Forces : BaseComponent<Forces>
    {
        DragForce dragForce;
        LinearDirectionalForce linearForce;

        Forces& DeserializeComponent(const rapidjson::Value& component) override
        {
            dragForce.directionalDrag = static_cast<float>(component["directionalDrag"].GetDouble());
            linearForce.unitDirection.x = static_cast<float>(component["unitDirectionX"].GetDouble());
            linearForce.unitDirection.y = static_cast<float>(component["unitDirectionY"].GetDouble());
            linearForce.magnitude = static_cast<float>(component["magnitude"].GetDouble());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("directionalDrag");
			writer.Double(dragForce.directionalDrag);
			writer.String("unitDirectionX");
			writer.Double(linearForce.unitDirection.x);
			writer.String("unitDirectionY");
			writer.Double(linearForce.unitDirection.y);
			writer.String("magnitude");
			writer.Double(linearForce.magnitude);
        }
    };
}

#endif
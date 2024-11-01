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
#include <vector>
#include "BaseComponent.h"

namespace Carmicah
{

   // struct Forces
   // {
   //     //LinearDirectionalForce linearForce;

   //     /*Forces& DeserializeComponent(const rapidjson::Value& component) override
   //     {
   //         linearForce.unitDirection.x = static_cast<float>(component["unitDirectionX"].GetDouble());
   //         linearForce.unitDirection.y = static_cast<float>(component["unitDirectionY"].GetDouble());
   //         linearForce.magnitude = static_cast<float>(component["magnitude"].GetDouble());
   //         return *this;
   //     }

   //     void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
   //     {
   //         writer.String("unitDirectionX");
			//writer.Double(linearForce.unitDirection.x);
			//writer.String("unitDirectionY");
			//writer.Double(linearForce.unitDirection.y);
			//writer.String("magnitude");
			//writer.Double(linearForce.magnitude);
   //     }*/
   // };
}

#endif
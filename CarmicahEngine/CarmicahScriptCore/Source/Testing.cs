/* File Documentation ----------------------------------------------------------------------------- 
file:           Testing.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `Testing` class, derived from `Entity`. It includes a test 
                variable and basic functionality for logging its creation and update events for 
                debugging purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Testing : Entity
    {
        public float testVar;

        public Entity testEntity;

        void OnCreate()
        {
            testEntity = FindEntityWithName("TestEntity");
            testEntity.As<BaseTest>().Shoot();
           // Console.WriteLine($"On Create {mID}");
        }

        void OnUpdate(float dt)
        {
          //  Console.WriteLine($"On Update {mID}");

        }

        public void SetUpPosition(Vector2 position)
        {
            Position = position;
        }

    }
}

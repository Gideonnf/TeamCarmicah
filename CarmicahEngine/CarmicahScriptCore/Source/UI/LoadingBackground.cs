/* File Documentation ----------------------------------------------------------------------------- 
file:           LoadingBackground.cs

author:		    Nicholas Lai (100%)

email:			n.lai@digipen.edu

brief:          

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class LoadingBackground : Entity
    {
        public float yOffset = 100.0f;
       // public string attachedEntityName = "TrueBackGround";


        float maxDescent;
       // Entity attachedEntity;
        //Vector2 pos;
        void OnCreate()
        {
           // attachedEntity = FindEntityWithName(attachedEntityName);
        }

        void OnUpdate(float dt)
        {
            Vector2 pos = Position;

            CMConsole.Log("Not Null!");
            CMConsole.Log($"{Position.x}, {Position.y}");
            CMConsole.Log($"{pos.y}");
            pos.y += (yOffset*dt);
            Position = pos;

        }
    }
}
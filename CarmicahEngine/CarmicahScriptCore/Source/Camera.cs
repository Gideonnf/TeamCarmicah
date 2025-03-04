/* File Documentation ----------------------------------------------------------------------------- 
file:           Camera.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `Camera` class, which allows vertical camera movement in a 
                game scene based on user input. It uses keyboard controls to adjust the camera's 
                position within predefined limits.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Camera : Entity
    {
        public float bottomLimit = -10.0f;
        public float topLimit = 10.0f;
        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyHold(Keys.KEY_R))
            {
                Vector2 pos = GetComponent<Transform>().Position;
                if (pos.y <= topLimit)
                {
                    pos.y += 4 * dt;
                    GetComponent<Transform>().Position = pos;
                }
            }
            
            if (Input.IsKeyHold(Keys.KEY_F))
            {
                Vector2 pos = GetComponent<Transform>().Position;
                if (pos.y >= bottomLimit)
                {
                    pos.y -= 4 * dt;
                    GetComponent<Transform>().Position = pos;

                }
            }
        }
    }
}

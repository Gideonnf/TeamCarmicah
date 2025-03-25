/* File Documentation ----------------------------------------------------------------------------- 
file:           BalloonBounce.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `BalloonBounce` class, which simulates a bouncing balloon 
                entity. The class manages bounce speed, height, and timing to create a smooth 
                bouncing motion with brief pauses at the peak and bottom.

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
    public class BalloonBounce : Entity
    {
        public float bounceSpeed = 2.0f; // How fast it bounces
        public float YOffset = 1.0f; // The height 
        public float holdTimer = 0.2f; // For how long it'll hold a lil bit at the top or bottom
        float maxHeight;
        float origHeight;
        float timer = 0.0f;
        bool deBounce = false;
        bool freeze = false;

        public override void OnCreate()
        {
            //CMConsole.Log($"ballon pos y {Position.y}");
            maxHeight = Position.y + YOffset;
            origHeight = Position.y;
        }

        public override void OnUpdate(float dt)
        {
            if (!freeze)
            {
                if (deBounce)
                {
                    Vector2 Pos = Position;
                    Pos.y -= bounceSpeed * dt;
                    Position = Pos;
                }
                else
                {
                    Vector2 Pos = Position;
                    Pos.y += bounceSpeed * dt;
                    Position = Pos;
                }
            }

            if (Position.y >= maxHeight)
            {
                deBounce = true;
                freeze = true;
            }
            else if (Position.y <= origHeight)
            {
                deBounce = false;
                freeze = true;
            }

            if(freeze)
            {
                timer += dt;
                if (timer >= holdTimer)
                {
                    freeze = false;
                    timer = 0.0f;
                }
            }

        }


    }
}

/* File Documentation ----------------------------------------------------------------------------- 
file:           CountDown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

brief:          This file contains the Countdown class which is responsible for displaying the countdown
                animation before the game starts.

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
    public class Credits : Entity
    {
        public Transform transform;
        public float scrollSpeed = 2.5f; // adjust speed as needed
        public float endYPosition = 45.0f; // change to match final Y position

        public override void OnCreate()
        {
            transform = GetComponent<Transform>();
        }

        public override void OnUpdate(float dt)
        {
            if (transform != null)
            {
                Vector2 position = transform.Position;

                if (position.y < endYPosition) // stop scrolling when it reaches the end
                {
                    position.y += scrollSpeed * dt;
                    transform.Position = position;
                }
            }

            if(transform.Position.y >= endYPosition)
            {
                Scene.ChangeScene("Scene3");
            }
        }
    }
}
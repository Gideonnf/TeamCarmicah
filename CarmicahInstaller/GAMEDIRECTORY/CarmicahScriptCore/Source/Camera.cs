﻿/* File Documentation ----------------------------------------------------------------------------- 
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
        public float ySpeed = 20.0f;

        public float shakeTotalTimer = 0.25f;
        public float shakeMagnitude = 0.1f;
        public float shakeTimer = 0.0f;
        public bool shake = false;
        public float xPos;

        public override void OnCreate()
        {
            xPos = Position.x;

        }

        public override void OnUpdate(float dt)
        {

            Vector2 pos = GetComponent<Transform>().Position;

            float scrollAmt = Input.GetMouseScroll() * 2.0f;

            if (Input.IsKeyHold(Keys.KEY_R) || scrollAmt > 0)
            {
                scrollAmt = Math.Max(1, scrollAmt);
                //Vector2 pos = GetComponent<Transform>().Position;
                if (pos.y <= topLimit)
                {
                    pos.y += scrollAmt * ySpeed * dt;
                    GetComponent<Transform>().Position = pos;

                }
            }
            
            if (Input.IsKeyHold(Keys.KEY_F) || scrollAmt < 0)
            {
                scrollAmt = Math.Max(1, -scrollAmt);

                //Vector2 pos = GetComponent<Transform>().Position;
                if (pos.y >= bottomLimit)
                {
                    pos.y -= scrollAmt * ySpeed * dt;
                    GetComponent<Transform>().Position = pos;

                }
            }

            Vector2 shakeOffset = Vector2.Zero;
            if (shake)
            {
                shakeTimer += dt;
                if (shakeTimer >= shakeTotalTimer)
                {
                    shake = false;
                    pos = new Vector2(xPos, pos.y);
                    //Position = originalPos;
                }
                else
                {
                    float offsetX = (CMRand.Range(0.0f, 2.0f) - 1.0f) * shakeMagnitude;
                    float offsetY = (CMRand.Range(0.0f, 2.0f) - 1.0f) * shakeMagnitude;

                    shakeOffset = new Vector2(offsetX, offsetY);
                }
            }

            Position = pos + shakeOffset;
        }

        public void ShakeCamera()
        {
            if(!shake)
            {
                shake = true;
                shakeTimer = 0.0f;
            }
        }
    }
}

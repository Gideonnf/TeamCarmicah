/* File Documentation ----------------------------------------------------------------------------- 
file:           CountDown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

brief:          This file contains the Countdown class which is responsible for displaying the countdown
                animation before the game starts. It displays the animation countdown from 3 to 1 before the game starts.

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
    public class Countdown : Entity
    {
        public int currentIndex = 0;
        public float timer = 0.0f;
        public float durationPerFrame = 1.0f; // time frame stays on screen
        public string[] countdownAnimation = { "N3", "N2", "N1", "GO" };

        private Vector2 originalScale;

        public override void OnCreate()
        {
            // display first animation
            DisplayAnimation();
        }

        void DisplayAnimation()
        {
            // run through the array of animations
            if (currentIndex <= countdownAnimation.Length)
            {
                // play 
                ChangeAnim(countdownAnimation[currentIndex]);
                timer = 0.0f;
            }
        }

        public override void OnUpdate(float dt)
        {
            timer += dt;

            if (GetComponent<Animation>().GetFrameNo() == 0)
            {
                switch (currentIndex)
                {
                    case 0:
                        Sound.PlaySFX("Countdown_3", 0.4f);

                        break;
                    case 1:
                        Sound.PlaySFX("Countdown_2", 0.4f);

                        break;
                    case 2:
                        Sound.PlaySFX("Countdown_1", 0.4f);

                        break;
                    case 3:
                        Sound.PlaySFX("Countdown_Ready", 0.4f);
                        break;
                }
            }

            // check if curr anim is finished
            if (timer >= durationPerFrame && GetComponent<Animation>().IsAnimFinished())//timer >= durationPerFrame)
            {
                timer = 0.0f;
                // increment index and display next animation
                currentIndex++;
                DisplayAnimation();
            }

            if(currentIndex == 4)
            {
                currentIndex = 0;
            }
        }
    }
}
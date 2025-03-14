/* File Documentation ----------------------------------------------------------------------------- 
file:           WaveCountdown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

brief:          This file contains the implementation of the WaveCountdown class. WaveCountdown is responsible for displaying the countdown
                animation before the game starts. It displays the wave number before each wave starts and bounces the text to make it more
                noticeable.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;


namespace Carmicah
{
    public class WaveCountdown : Entity
    {
        public string[] waveCountdownTexts = { "Wave 1!", "Wave 2!", "Wave 3!" };
        public int waveCountdownIndex = 0;
        public float timer = 0.0f;
        public float bounceDuration = 0.5f; // duration of bounce effect
        public float bounceScale = 1.5f; // maximum scale during bounce
        public float startingTime = 17.0f; // time before text appears
        public float endingTime = 20.0f; // time before text disappears

        private Vector2 originalScale;
        private bool isVisible = false; // track visibility

        void OnCreate()
        {
            originalScale = Scale;
            Scale = new Vector2(0, 0); // hide
        }

        void OnUpdate(float dt)
        {
            timer += dt;

            if (timer < 17.0f)
            {
                // hide text for first 17s
                Scale = new Vector2(0, 0);
                isVisible = false;
            }
            else if (timer < 20.0f)
            {
                // show text for last 3s
                if (!isVisible)
                {
                    GetComponent<TextRenderer>().SetText(waveCountdownTexts[waveCountdownIndex]);
                    isVisible = true;
                }

                float bounceFactor = (float)Math.Sin(((timer - 17.0f) / bounceDuration) * Math.PI) * (bounceScale - 1.0f) + 1.0f;
                Scale = new Vector2(originalScale.x * bounceFactor, originalScale.y * bounceFactor);
            }
            else
            {
                // reset everything after 20s
                waveCountdownIndex = (waveCountdownIndex + 1) % waveCountdownTexts.Length;
                timer = 0.0f;
                isVisible = false;
            }
        }
    }
}

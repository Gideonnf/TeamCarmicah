/* File Documentation ----------------------------------------------------------------------------- 
file:           CountDown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

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
    public class Countdown : Entity
    {
        //public string[] countdownTexts = { "CD_Num_N3", "CD_Num_N2", "CD_Num_N1", "CD_GO_GO"};
        public int currentIndex = 0;
        public float timer = 0.0f;
        public float durationPerText = 1.0f; // time text stays on screen
        public float bounceDuration = 0.5f; // duration of bounce effect1
        public float bounceScale = 1.5f; // maximum scale during bounce

        public float durationPerFrame = 1.0f; // time frame stays on screen
        public string[] countdownAnimation = { "N3", "N2", "N1", "GO" };

        private Vector2 originalScale;

        void OnCreate()
        {
            //originalScale = Scale;
            //DisplayText();
            DisplayAnimation();
        }

        //void DisplayText()
        //{
        //    if (currentIndex <= countdownTexts.Length)
        //    {
        //        GetComponent<TextRenderer>().SetText(countdownTexts[currentIndex]); // iterate through the string array
        //        Scale = new Vector2(originalScale.x * 0.5f, originalScale.y * 0.5f); // start small
        //        timer = 0.0f; // just follow rainne's code
        //    }
        //}

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

        void OnUpdate(float dt)
        {
            timer += dt;

            // bounce Effect
            //float bounceFactor = (float)Math.Sin((timer / bounceDuration) * Math.PI) * (bounceScale - 1.0f) + 1.0f; // thank u gpt
            //textEntity.GetComponent<TextRenderer>() = originalScale * bounceFactor; // argh
            //Scale = new Vector2(originalScale.x * bounceFactor, originalScale.y * bounceFactor); // start small

            // check if time to switch to next string
            //if (timer >= durationPerText)
            //{
            //    currentIndex++;
            //    DisplayText();
            //}
            if (GetComponent<Animation>().IsAnimFinished())//timer >= durationPerFrame)
            {
                currentIndex++;
                DisplayAnimation();
            }
        }
    }
}
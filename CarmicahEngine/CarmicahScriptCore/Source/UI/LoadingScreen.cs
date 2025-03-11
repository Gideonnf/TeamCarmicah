/* File Documentation ----------------------------------------------------------------------------- 
file:           LoadingScreen.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          This file defines the `LoadingScreen` class, a UI entity that displays a series of 
                images to introduce the game. It demonstrates the use of the `Renderer` component to 
                control the transparency of entities within the Carmicah engine.

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
    public class LoadingScreen : Entity
    {
        private string fullText = "Good morning, my name is Micah, i'm a shit coder";
        private StringBuilder currentText = new StringBuilder();
        private int currentIndex = 0;
        private float timePerChar = 0.1f;  // delay between each character (in seconds)
        private float timer = 0.0f;
        private bool isTyping = true;

        public override void OnCreate()
        {
            currentText.Clear();
            StartTyping(fullText, 10f);
        }

        public override void OnUpdate(float dt)
        {
            if (!isTyping) return;

            timer += dt;

            // check if it's time to add a new character
            if (timer >= timePerChar && currentIndex < fullText.Length)
            {
                // add next character
                currentText.Append(fullText[currentIndex]);
                currentIndex++;

                // reset timer for next character
                timer = 0.0f;

                // add print/display logic here, currently only for console
                Console.Write("\r" + currentText.ToString() + " "); // terminal output, what i wanted it to look like
                //CMConsole.Log("\r" + currentText.ToString() + " "); // console log output, not what i wanted it to look like

                // use game's text rendering system for scene, lowk embarassed how long it took me to do this logic bruh

                // check if finished typing
                if (currentIndex >= fullText.Length)
                {
                    isTyping = false;
                }
            }
        }

        // helper method to start typing a new text
        public void StartTyping(string text, float charactersPerSecond = 10f)
        {
            fullText = text;
            timePerChar = 1f / charactersPerSecond;
            currentIndex = 0;
            currentText.Clear();
            timer = 0.0f;
            isTyping = true;
        }
        
    }
}
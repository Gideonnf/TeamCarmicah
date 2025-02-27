/* File Documentation ----------------------------------------------------------------------------- 
file:           CountDown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

brief:          

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/

//{
//    "GameObject": "Countdown",
//            "ID": 5,
//            "Components": 
//            [
//                {
//        "Component Name": "struct Carmicah::Transform",
//                    "xPos": 0.0,
//                    "yPos": 0.0,
//                    "depth": 0.0,
//                    "xScale": 0.0,
//                    "yScale": 0.0,
//                    "rot": 0.0,
//                    "transformTag": "",
//                },
//                {
//        "Component Name": "struct Script",
//                    "scriptName": "Countdown",
//                    "ScriptableFieldMap": 
//                    [
//                        {
//            "currentIndex" : 0,
//                            "timer" : 0.0,
//                            "durationPerText" : 1.0,
//                            "bounceDuration" : 0.5,
//                            "bounceScale" : 1.5
//                        }
//                    ]
//                },
//            ],
//            "Children": []
//        },  


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
        public string[] countdownTexts = { "3", "2", "1", "Ready?", "Start!" };
        public int currentIndex = 0;
        public float timer = 0.0f;
        public float durationPerText = 1.0f; // time text stays on screen
        public float bounceDuration = 0.5f; // duration of bounce effect1
        public float bounceScale = 1.5f; // maximum scale during bounce

        private Vector2 originalScale;

        void OnCreate()
        {
            //textEntity = FindEntityWithName("CountdownText");
            //originalScale = textEntity.GetComponent<Renderer>(); // need to get scale here but idk why i cant call it
            originalScale = Scale;
            DisplayText();
        }

        void DisplayText()
        {
            if (currentIndex < countdownTexts.Length)
            {
                GetComponent<TextRenderer>().SetText(countdownTexts[currentIndex]); // iterate through the string array
                Scale = new Vector2(originalScale.x * 0.5f, originalScale.y * 0.5f); // start small
                timer = 0.0f; // just follow rainne's code
            }
            else
            {
                Scene.ChangeScene("GameStart"); // move to next scene after countdown
            }
        }

        void OnUpdate(float dt)
        {
            timer += dt;

            // bounce Effect
            float bounceFactor = (float)Math.Sin((timer / bounceDuration) * Math.PI) * (bounceScale - 1.0f) + 1.0f; // thank u gpt
            //textEntity.GetComponent<TextRenderer>() = originalScale * bounceFactor; // argh

            Scale = new Vector2(originalScale.x * bounceFactor, originalScale.y * bounceFactor); // start small

            // check if time to switch to next string
            if (timer >= durationPerText)
            {
                currentIndex++;
                DisplayText();
            }
        }
    }
}
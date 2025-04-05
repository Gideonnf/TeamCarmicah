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

        public string backgroundMusicTrack = "BGM_Credits"; 
        public float fadeOutDuration = 1.5f; 
        private bool isFadingOut = false;
        private float fadeTimer = 0.0f;
        private bool musicStarted = false;

        public override void OnCreate()
        {
            transform = GetComponent<Transform>();

            if (!musicStarted)
            {
                Sound.PlayBGM(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }
        }

        public override void OnUpdate(float dt)
        {
            if (transform != null)
            {
                Vector2 position = transform.Position;

                if (position.y < endYPosition) 
                {
                    position.y += scrollSpeed * dt;
                    transform.Position = position;
                }
                else 
                {
                    if (!isFadingOut)
                    {
                        isFadingOut = true;
                        fadeTimer = 0.0f;
                        //fate music
                        Sound.SwitchBGM("zero", fadeOutDuration, fadeOutDuration, false);
                    }
                    else
                    {
                        fadeTimer += dt;
                        if (fadeTimer >= fadeOutDuration)
                        {
                            Sound.StopAllSFX();
                            FindEntityWithName("SceneTransition").As<SceneTransition>().FadeOut("Scene3");
                            // Scene.ChangeScene("Scene3");
                        }
                    }
                }
            }
        }
    }
}
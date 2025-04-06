/* File Documentation ----------------------------------------------------------------------------- 
file:           Credits.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          this file defines the `Credits` class, a UI entity that displays the credits screen. It is intended for 
                testing the functionality of the Carmicah engine's UI system. It scrolls the credits text 
                upwards and fades out the background music at the end.

Copyright (C) 2025 DigiPen Institute of Technology.
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
    public class Credits : Entity
    {
        public Transform transform;
        public float initialScrollSpeed = 2.5f; // starting scroll speed
        public float currentScrollSpeed = 2.5f; // speed that gets updated
        public float slowDownYPosition = 24.0f; // begin deceleration here
        public float endYPosition = 29.0f;      // stop around here

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

                if (position.y < slowDownYPosition)
                {
                    currentScrollSpeed = initialScrollSpeed;
                    position.y += currentScrollSpeed * dt;
                    transform.Position = position;
                }
                else if (position.y < endYPosition)
                {
                    float t = (position.y - slowDownYPosition) / (endYPosition - slowDownYPosition);
                    t = Clamp01(t);
                    float easedSpeed = Lerp(initialScrollSpeed, 0.0f, SmoothStep(t));
                    currentScrollSpeed = Math.Max(0.02f, easedSpeed);
                    position.y += currentScrollSpeed * dt;
                    transform.Position = position;
                }
                else
                {
                    if (!isFadingOut)
                    {
                        isFadingOut = true;
                        fadeTimer = 0.0f;
                        Sound.SwitchBGM("zero", fadeOutDuration, fadeOutDuration, false);
                    }
                    else
                    {
                        fadeTimer += dt;
                        if (fadeTimer >= fadeOutDuration)
                        {
                            Sound.StopAllSFX();
                            FindEntityWithName("SceneTransition").As<SceneTransition>().FadeOut("Scene3");
                        }
                    }
                }
            }
        }

        private float Lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        private float SmoothStep(float t)
        {
            return t * t * (3f - 2f * t);
        }

        private float Clamp01(float val)
        {
            if (val < 0f) return 0f;
            if (val > 1f) return 1f;
            return val;
        }
    }
}

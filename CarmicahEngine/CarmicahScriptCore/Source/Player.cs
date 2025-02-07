/* File Documentation ----------------------------------------------------------------------------- 
file:           Player.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `Player` class, which manages player animation and movement. 
                It handles walking, idle, and healing states, applies forces for movement based on 
                keyboard input, and plays sound effects with a cooldown timer.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Player : Entity
    {
        float timer = 0.5f;

        public string HealAnim;
        public string IdleAnim;
        public string WalkAnim;
        public static bool GameLost = false;

        public bool isWalking = true;
        
        void OnCreate()
        {

        }

        void ToggleWalkAnim()
        {
            if (!isWalking)
            {
                isWalking = true;
                ChangeAnim(WalkAnim);
            }
        }

        void ToggleIdle()
        {
            if (isWalking)
            {
                isWalking = false;
                ChangeAnim(IdleAnim);
            }
        }

        void ToggleHeal()
        {
            isWalking = false;
            ChangeAnim(HealAnim);
        }

        void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }
            timer += dt;

            if (Input.IsKeyHold(Keys.KEY_W))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk2");
                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_A))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk2");

                Vector2 scale = Scale;
                if (scale.x < 0)
                {
                    scale.x *= -1;
                }

                Scale = scale;

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(-1, 0), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_S))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk2");

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, -1), 2.0f);
            }
            else if (Input.IsKeyHold(Keys.KEY_D))
            {
                ToggleWalkAnim();

                PlaySoundEffect("walk2");

                Vector2 scale = Scale;
                if (scale.x > 0)
                {
                    scale.x *= -1;
                }

                Scale = scale;

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(1,0), 2.0f);
            }
            else
            {
                ToggleIdle();
            }
        }

        void PlaySoundEffect(string name)
        {
            if (timer >= 0.5f)
            {
                Sound.PlaySFX(name);
                timer = 0.0f;
            }
        }

        public void TakeDamage(int damage)
        {
            this.AsChild<HealthSystem>().TakeDamage(damage);

            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                // game end
                if (!GameLost)
                {
                    CreateGameObject("LoseScreen");
                    CreateGameObject("HomeButton");
                    CreateGameObject("MC_Lose");
                    CreateGameObject("ReplayButton");
                    CreateGameObject("Break_Wand");
                    CreateGameObject("LoseText");
                    GameLost = true;
                }
            }
        }
    }
}

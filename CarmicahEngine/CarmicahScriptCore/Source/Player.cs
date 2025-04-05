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


using Carmicah;
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
        public string healthBarName = "Princess_HealthBar";

        public float walkingSpeed = 2.0f;

        public string HealAnim;
        public string IdleAnim;
        public string WalkAnim;
        public string TeleportAnim = "NPC_Teleport";
        public static bool GameLost = false;

        // God mode variable - made static so it can be accessed by other classes
        public static bool godMode = false;

        public bool isWalking = true;
        public bool isCreated = false;

        Entity healTarget;
        Entity redBorder;
        Entity healthBar;
        float healAnimTime;

        public bool damaged = false;
        public float flashTotalTime = 1.0f;
        public float flashInterval = 0.25f;
        public float flashTimer = 0.0f; // Keep track for interval flashing
        public float flashTime = 0.0f; // keep track of total time for flashTime
        public float health = 100.0f;

        bool invisible = true;
        public override void OnCreate()
        {
            redBorder = FindEntityWithName("RedBorder");
            healthBar = FindEntityWithName(healthBarName);
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

        public void HealAI(uint id)
        {
            healTarget = FindEntityWithID(id);

            // if target is on the left
            if (healTarget.Position.x < Position.x)
            {
                if (Scale.x < 0)
                {
                    Scale = new Vector2(Scale.x * -1, Scale.y);
                }
            }
            else if (healTarget.Position.x > Position.x)
            {
                if (Scale.x > 0)
                {
                    Scale = new Vector2(Scale.x * -1, Scale.y);
                }
            }

            //CMConsole.Log("Healing " + id.ToString());
            GetComponent<StateMachine>().SetStateCondition(3);
        }

        public override void OnUpdate(float dt)
        {
            //cheat code
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            // Cheat code Full health (key 8)
            if (Input.IsKeyPressed(Keys.KEY_8))
            {
                CMConsole.Log("Cheat activated: Full health restored");
                health = 100.0f;
                healthBar.As<PrincessHPBar>().percentHP = health;
            }

            // God Mode cheat code (keys 1 and 3)
            if (Input.IsKeyHold(Keys.KEY_1) && Input.IsKeyPressed(Keys.KEY_3))
            {
                godMode = !godMode;

                if (godMode)
                {
                    CMConsole.Log("Cheat activated: God Mode ON - Player invulnerable, NPCs deal 2x damage");
                    // When activating God Mode, also restore health
                    health = 100.0f;
                    healthBar.As<PrincessHPBar>().percentHP = health;
                }
                else
                {
                    CMConsole.Log("God Mode OFF");
                }
            }

            if (damaged)
            {
                flashTime += dt;

                if (flashTime >= flashTotalTime)
                {
                    damaged = false;
                    invisible = true;
                    redBorder.GetComponent<Renderer>().SetAlpha(0.0f);
                    flashTime = 0.0f;
                    return;
                }

                flashTimer += dt;

                if (flashTimer >= flashInterval)
                {
                    flashTimer = 0.0f;
                    invisible = !invisible;
                    if (invisible)
                    {
                        redBorder.GetComponent<Renderer>().SetAlpha(0.0f);
                    }
                    else
                    {
                        redBorder.GetComponent<Renderer>().SetAlpha(1.0f);
                    }
                }
            }
        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "Idle")
            {
                ChangeAnim(IdleAnim);
                //CMConsole.Log("MC IS IDLE YAY");
            }

            else if (stateName == "Walking")
            {
                ChangeAnim(WalkAnim);
            }
            else if (stateName == "Heal")
            {
                Sound.PlaySFX("Princess_Heal", 0.3f);
                ChangeAnim(HealAnim);
                healAnimTime = GetComponent<Animation>().GetMaxTime();
                timer = 0.0f;
                //CMConsole.Log("MC HEAL");
            }
            else if (stateName == "Teleport")
            {
                ChangeAnim(TeleportAnim);
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            if (stateName == "Idle")
            {
                if (Input.IsKeyHold(Keys.KEY_W) || Input.IsKeyHold(Keys.KEY_A) || Input.IsKeyHold(Keys.KEY_S) || Input.IsKeyHold(Keys.KEY_D))
                {
                    //CMConsole.Log("Should be changing to Walking State");
                    isWalking = true;
                    GetComponent<StateMachine>().SetStateCondition(2);
                }
            }

            else if (stateName == "Walking")
            {
                Vector2 position = Vector2.Zero;
                if (Input.IsKeyHold(Keys.KEY_W))
                {
                    PlaySoundEffect("walk2");
                    position.y += walkingSpeed * dt;
                }
                if (Input.IsKeyHold(Keys.KEY_A))
                {
                    PlaySoundEffect("walk2");

                    Vector2 scale = Scale;
                    if (scale.x < 0)
                    {
                        scale.x *= -1;
                    }

                    Scale = scale;
                    position.x -= walkingSpeed * dt;
                }
                if (Input.IsKeyHold(Keys.KEY_S))
                {
                    PlaySoundEffect("walk2");
                    position.y -= walkingSpeed * dt;
                }
                if (Input.IsKeyHold(Keys.KEY_D))
                {
                    PlaySoundEffect("walk2");

                    Vector2 scale = Scale;
                    if (scale.x > 0)
                    {
                        scale.x *= -1;
                    }

                    Scale = scale;
                    position.x += walkingSpeed * dt;
                }

                if ((!Input.IsKeyHold(Keys.KEY_W) && !Input.IsKeyHold(Keys.KEY_A) && !Input.IsKeyHold(Keys.KEY_S) && !Input.IsKeyHold(Keys.KEY_D)))
                {
                    GetComponent<StateMachine>().SetStateCondition(1);
                }

                GetComponent<RigidBody>().Move(position);
            }
            else if (stateName == "Heal")
            {
                timer += dt;
                if (timer > healAnimTime)
                {
                    healTarget.As<BaseNPC>().HealAmmo();
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }
        }

        public override void OnStateExit(string stateName)
        {

        }

        void PlaySoundEffect(string name)
        {
            if (timer >= 0.5f)
            {
                Sound.PlaySFX(name);
                timer = 0.0f;
            }
        }

        public void TakeDamage(int damage, EnemyTypes enemyType)
        {
            // Skip damage if in God Mode
            if (godMode)
                return;

            health -= damage;
            healthBar.As<PrincessHPBar>().percentHP = health;

            Sound.PlaySFX("Princess_DamageWarning", 0.3f);
            damaged = true;
            Entity camera = FindEntityWithName("MainCamera");
            camera.As<Camera>().ShakeCamera();
            if (health <= 0)
            {
                // game end
                if (!isCreated)
                {
                    Entity gameManager = FindEntityWithName("GameManager");
                    gameManager.As<GameManager>().LoseGame();
                    isCreated = true;
                }
            }
        }
    }
}
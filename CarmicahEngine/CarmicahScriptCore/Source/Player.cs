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

        public float walkingSpeed = 2.0f;

        public string HealAnim;
        public string IdleAnim;
        public string WalkAnim;
        public static bool GameLost = false;

        public bool isWalking = true;
        public bool isCreated = false;

        Entity healTarget;
        Entity redBorder;
        float healAnimTime;

        public bool damaged = false;
        public float flashTime = 1.0f;
        public float flashInterval = 0.25f;
        public float flashTimer = 0.0f; // Keep track for interval flashing
        public float elapsedTime = 0.0f; // keep track of total time for flashTime
        bool invisible = true;
        void OnCreate()
        {
            redBorder = FindEntityWithName("RedBorder");
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
            CMConsole.Log("Healing " + id.ToString());
            GetComponent<StateMachine>().SetStateCondition(3);
        }

        void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            if (damaged)
            {
                elapsedTime += dt;

                if (elapsedTime >= flashTime)
                {
                    damaged = false;
                    invisible = true;
                    redBorder.GetComponent<Renderer>().SetAlpha(0.0f);
                    elapsedTime = 0.0f;
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
            

            /*if (Input.IsKeyPressed(Keys.KEY_W))
            {

                GetComponent<StateMachine>().SetStateCondition(1);

                PlaySoundEffect("walk2");
                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_A))
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

                GetComponent<RigidBody>().ApplyForce(new Vector2(1, 0), 2.0f);
            }
            else
            {
                ToggleIdle();
            }*/
        }

        public void OnStateEnter(string stateName)
        {
            if (stateName == "Idle")
            {
                ChangeAnim(IdleAnim);
                CMConsole.Log("MC IS IDLE YAY");
            }

            else if (stateName == "Walking")
            {
                ChangeAnim(WalkAnim);
            }
            else if (stateName == "Heal")
            {
                ChangeAnim(HealAnim);
                healAnimTime = GetComponent<Animation>().GetMaxTime();
                timer = 0.0f;
                CMConsole.Log("MC HEAL");
            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            if(stateName == "Idle")
            {
                if (Input.IsKeyHold(Keys.KEY_W) || Input.IsKeyHold(Keys.KEY_A)|| Input.IsKeyHold(Keys.KEY_S) || Input.IsKeyHold(Keys.KEY_D))
                {
                    //CMConsole.Log("Should be changing to Walking State");
                    isWalking = true;
                    GetComponent<StateMachine>().SetStateCondition(2);

                    //PlaySoundEffect("walk2");
                    //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                    //GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
                }
            }

            else if (stateName == "Walking")
            {
                Vector2 position = Position;
                if (Input.IsKeyHold(Keys.KEY_W))
                {
                    PlaySoundEffect("walk2");
                    //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                    //GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
                    //Position = new Vector2(Position.x, Position.y + (walkingSpeed * dt));
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

                    //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                    // Position = new Vector2(Position.x - (walkingSpeed * dt), Position.y);
                    position.x -= walkingSpeed * dt;
                    // GetComponent<RigidBody>().ApplyForce(new Vector2(-1, 0), 2.0f);
                }
                if (Input.IsKeyHold(Keys.KEY_S))
                {

                    PlaySoundEffect("walk2");

                    //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");
                    // Position = new Vector2(Position.x, Position.y - (walkingSpeed * dt));
                    position.y -= walkingSpeed * dt;
                    // GetComponent<RigidBody>().ApplyForce(new Vector2(0, -1), 2.0f);
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

                    //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");
                    // Position = new Vector2(Position.x + (walkingSpeed * dt), Position.y);
                    position.x += walkingSpeed * dt;
                    //GetComponent<RigidBody>().ApplyForce(new Vector2(1, 0), 2.0f);
                }
                
                if((!Input.IsKeyHold(Keys.KEY_W) && !Input.IsKeyHold(Keys.KEY_A) && !Input.IsKeyHold(Keys.KEY_S) && !Input.IsKeyHold(Keys.KEY_D)))
                {
                    GetComponent<StateMachine>().SetStateCondition(1);
                }

                Position = position;
            }
            else if (stateName == "Heal")
            {
                timer += dt;
                if(timer > healAnimTime)
                {
                    CMConsole.Log("Healing Target!");
                    healTarget.As<HeroAI>().HealAmmo();
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }
        }

        public void OnStateExit(string stateName)
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
            this.AsChild<HealthSystem>().TakeDamage(damage);

            damaged = true;
            //CMConsole.Log($"Health :{this.AsChild<HealthSystem>().mCurHealth}");
            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                // game end
                if (!isCreated)
                {
                    Entity gameManager = FindEntityWithName("GameManager");
                    gameManager.As<GameManager>().LoseGame();
                    //CreateGameObject("LosePrefab");
                    isCreated = true;
                }
            }
        }
    }
}

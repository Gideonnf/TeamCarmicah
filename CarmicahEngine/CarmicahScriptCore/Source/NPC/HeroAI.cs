using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroAI : Entity
    {
        public string idleAnim = "Shooter_Idle";
        public string shootAnim = "Shooter_Shoot";

        public string projectilePrefab = "Bullet";
        public float shootRate = 1.0f;
        public float shootTime = 1.0f;
        public int lane;
        float timer = 0.0f;
        public bool active = false;
        public bool IsLeft = false;

       // float shootTimer = 0.0f;
        bool isShooting = false;
        bool shot = false;
        GameManager gameManager;
        PauseManager pauseManager;
        MouseAI targetMouse;
        float animationTime;

        void OnCreate()
        {
            gameManager = FindEntityWithName("GameManager").As<GameManager>();
            pauseManager = FindEntityWithName("PauseManager").As<PauseManager>();
        }

        void OnUpdate(float dt)
        {
            if (pauseManager.IsPaused) { return; }
           // CMConsole.Log("TESTING IF CMLOG WORKS");

            if (isShooting)
            {
                //shootTimer += dt;
                //if (shootTimer >= shootRate)
                //{
                //    shootTimer = 0.0f;
                //    ShootProjectile();
                //}
            }
        }

        void ShootProjectile()
        {
            // Get the closest enemy to the hero based on the side
            //MouseAI targetMouse = gameManager.GetClosestMouse(this);
            // if it found a target
            if (targetMouse != null)
            {
                // CMConsole.Log($"mouse retrieved : {targetMouse}");
                Entity projectile = CreateGameObject(projectilePrefab);
                if (projectile != null) 
                {
                  //  CMConsole.Log($"Hero Position: {Position.x}, {Position.y}");
                    projectile.Position = Position;
                   // CMConsole.Log($"Projectile Position: {projectile.Position.x}, {projectile.Position.y}");

                    Projectile bullet = projectile.As<Projectile>();
                    Sound.PlaySFX("Shooter_Shooting");
                    if (bullet != null)
                    {
                        bullet.targetMouse = targetMouse;
                        
                        bullet.SetUp(targetMouse);
                    }
                }
            }
        }

        // TODO: This should be done in the update loop when shooting
        // it should call shoot anim when shooting and idle at the end of shooting animation
        // either do it by animation state or get the time of the animation and change when ending
        public void ToggleShooting()
        {
            //check if the npc is already shooting
            isShooting = true;
           // ChangeAnim(shootAnim);
            
        }

        public void ToggleIdle()
        {
            //check if the npc is already idle
            isShooting = false;
           // ChangeAnim(idleAnim);
        }

        public void OnStateEnter(string stateName)
        {
            if (stateName == "Idle")
            {
                ChangeAnim(idleAnim);
                CMConsole.Log("IDLE ANIM");
            }
            else if (stateName == "Attacking")
            {
                //CMConsole.Log("TESTING Enter State");
                ChangeAnim(shootAnim);
                animationTime = GetComponent<Animation>().GetMaxTime();
                timer = 0.0f;
                shot = false;
                CMConsole.Log($"Max Anim Time : {animationTime}");

            }


            //CMConsole.Log($"Enter State Name: {stateName}");
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            if (active == false) return;

            // idk if this will happen but if the mouse dies
            // this script might still hold a refeence to a 0 id mouse
            // which will cause crashes
            if (targetMouse != null && targetMouse.mID == 0)
            {
                CMConsole.Log("I AM HERE");
                targetMouse = null;
                // Change back to idle state
                //if (stateName == "Attacking")
                //    GetComponent<StateMachine>().SetStateCondition(1);
                return;
            }

            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Idle")
            {
                // Get nearest enemy 
                targetMouse = gameManager.GetClosestMouse(this);
                if (targetMouse != null)
                {
                    CMConsole.Log($"Target mouse : {targetMouse.mID}");

                    // change to attacking state
                    GetComponent<StateMachine>().SetStateCondition(2);
                }
            }
            else if (stateName == "Attacking")
            {
                //CMConsole.Log($"Shooting timer : {timer}");

                timer += dt;
                if (timer > shootTime)
                {
                    if (!shot && targetMouse != null)
                    {
                        ShootProjectile();
                        shot = true;

                        // reset the timer
                       // timer = 0.0f;
                    }
                    else
                    {
                        if (timer >= animationTime)
                            GetComponent<StateMachine>().SetStateCondition(1);
                    }

                }
            }

        }

        public void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");

        }
    }
}

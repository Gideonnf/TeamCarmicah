using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HeroAI : Entity
    {
        public string idleAnim = "Shooter_Idle";
        public string shootAnim = "Shooter_Shoot";

        public string projectilePrefab = "Bullet";
        public float shootRate = 1.0f;

        public bool IsLeft = false;

        float shootTimer = 0.0f;
        bool isShooting = false;
        GameManager gameManager;
        PauseManager pauseManager;

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
                shootTimer += dt;
                if (shootTimer >= shootRate)
                {
                    shootTimer = 0.0f;
                    ShootProjectile();
                }
            }
        }

        void ShootProjectile()
        {
            // Get the closest enemy to the hero based on the side
            MouseAI targetMouse = gameManager.GetClosestMouse(this);
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
            ChangeAnim(shootAnim);
            
        }

        public void ToggleIdle()
        {
            //check if the npc is already idle
            isShooting = false;
            ChangeAnim(idleAnim);
        }
        public void OnStateEnter(string stateName)
        {
            if (stateName == "Attacking")
            {
                //CMConsole.Log("TESTING Enter State");

            }
            //CMConsole.Log($"Enter State Name: {stateName}");
        }

        public void OnStateUpdate(string stateName, float dt)
        {
           // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Attacking")
            {
               // CMConsole.Log("TESTING Update State");

            }
            //CMConsole.Log($"mouse retrieved : {targetMouse}");
        }

        public void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");

        }
    }
}

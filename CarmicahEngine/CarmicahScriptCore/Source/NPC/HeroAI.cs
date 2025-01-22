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
                CMConsole.Log($"{targetMouse}");
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

    }
}

using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScriptCore.Source
{
    public class NPCProjectile : Entity
    {
        //set the speed and lifetime of the projectile
        public float Speed = 5.0f;
        public float LifeTime = 2.0f;
        public string ProjectileHitAnim;
        public string ProjectileMoveAnim;
        public float timer = 0.0f;
        public bool isMoving = true;

        void OnCreate()
        {
            //set the projectile to move
            if (!string.IsNullOrEmpty(ProjectileMoveAnim))
            {
                ChangeAnim(ProjectileMoveAnim);
            }
        }

        void OnUpdate(float dt)
        {
            //check if the game is paused
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            timer += dt;
            if (timer >= LifeTime)
            {
                Destroy();
            }
        }

        void OnCollide()
        {
            if (isMoving)
            {
                isMoving = false;
                if (!string.IsNullOrEmpty(ProjectileHitAnim))
                {
                    ChangeAnim(ProjectileHitAnim);
                }
                Destroy();
            }
        }
    }
}

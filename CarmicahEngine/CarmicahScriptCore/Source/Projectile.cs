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
        public float Speed = 5.0f;
        public float LifeTime = 2.0f;
        public string ProjectileHitAnim;
        public string ProjectileMoveAnim;
        public float timer = 0.0f;
        public bool isMoving = true;

        void OnCreate()
        {
            if (!string.IsNullOrEmpty(ProjectileMoveAnim))
            {
                ChangeAnim(ProjectileMoveAnim);
            }
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

using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CarmicahScriptCore.Source;


namespace Carmicah
{
    public class Projectile : Entity
    {
        public string BulletAnim;
        public float Speed = 5.0f;
        public float LifeTime = 10.0f;
        //target enemy
        public MouseAI targetMouse;
        float timer = 0.0f;
        bool facingRight = false;

        void OnCreate()
        {
            // Set initial animation
            if (!string.IsNullOrEmpty(BulletAnim))
            {
                ChangeAnim(BulletAnim);
            }

            // Set scale based on direction
            Vector2 scale = Scale;
            if (!facingRight)
            {
                scale.x *= -1;
                Scale = scale;
            }
        }

        void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null && pauseManager.As<PauseManager>().IsPaused)
                return;

            // Update lifetime
            timer += dt;
            if (timer >= LifeTime)
            {
                Destroy();
                return;
            }

            if (targetMouse == null)
            {
                Destroy();
                return;
            }
            else
            {
                // Move the bullet
                // this is dying for some reason
                if (HasComponent<RigidBody>())
                {
                    Vector2 dir = targetMouse.Position - Position;
                    dir.Normalize();
                    CMConsole.Log($"{dir.x}, {dir.y}");
                    //Vector2 dir = facingRight ? new Vector2(1, 0) : new Vector2(-1, 0);
                    //GetComponent<RigidBody>().ApplyForce(dir, Speed);
                }

            }

        }

        // Set initial direction
        public void SetUp(MouseAI target)
        {
            targetMouse = target;

            if (targetMouse != null)
            {
                // Get target direction
                Vector2 dir = targetMouse.Position - Position;
                dir.Normalize();
                CMConsole.Log($"{dir.x}, {dir.y}");


            }
            else
            {
                CMConsole.Log("Error setting up bullet");
            }
        }

        public void SetDirection(bool right)
        {
            facingRight = right;
        }

        void OnCollide()
        {
            // Optional: Play some effect
            Destroy();
        }
    }
}

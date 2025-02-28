/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TransformSystem.cpp
  
 author:		YANG YUJIE (40%)

 email:		    y.yujie@digien.edu

 brief:			The projectile system is responsible for managing the projectile. It will move the projectile towards the target enemy. 
                The projectile will be destroyed if it hits the target enemy or if it reaches the end of its lifetime. 
                The projectile will also be destroyed if the target enemy is destroyed. 
                The system will also handle the collision detection between the projectile and the target enemy.
                The system will also play the sound effect when the projectile hits the target enemy.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/



using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CarmicahScriptCore.Source;


namespace Carmicah
{
    public enum BulletType
    {
        SHOOTER_BULLET,
        MAGE_BULLET
    }
    public class Projectile : Entity
    {
        public string BulletAnim = "Shooter_Projectile";
        public string BulletImpactAnim;
        public float Speed = 5.0f;
        public float LifeTime = 10.0f;
        public BulletType bulletType;
        //target enemy
        public MouseAI targetMouse;

        float timer = 0.0f;
        float animTimer = 0.0f;
        float maxAnimTime;

        bool facingRight = false;
        bool playDeathAnimation = false;

        void OnCreate()
        {
            // Set initial animation
            if (!string.IsNullOrEmpty(BulletAnim))
            {
                CMConsole.Log("Changing animation to " + BulletAnim);
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
                targetMouse = null;
                Destroy();
                return;
            }

            if (targetMouse == null)
            {
               // CMConsole.Log("TAOPMDOPSA");
                Destroy();
                return;
            }

            else
            {
                if (targetMouse.mID == 0)
                {
                    targetMouse = null;
                    return;
                }
                // Move the bullet
                // this is dying for some reason
                if (HasComponent<RigidBody>())
                {
                    Vector2 mousePos = targetMouse.Position;
                    Vector2 dir = mousePos - Position;
                    dir.Normalize();
                    //CMConsole.Log($"target mouse??? {targetMouse.mID}");
                    //Vector2 dir = facingRight ? new Vector2(1, 0) : new Vector2(-1, 0);
                    GetComponent<RigidBody>().ApplyForce(dir, Speed);
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
                //CMConsole.Log($"{dir.x}, {dir.y}");


            }
            else
            {
                Destroy();
                CMConsole.Log("Error setting up bullet");
            }
        }

        public void SetDirection(bool right)
        {
            facingRight = right;
        }

        void BulletDeathAnimation()
        {

        }
        
        void OnCollide(uint id)
        {
            Entity collidedEntity = FindEntityWithID(id);
            if (collidedEntity != null)
            {
                //CMConsole.Log($"Collided Entity : {collidedEntity.mID} with tag {collidedEntity.GetTag()}");
                //if (collidedEntity.GetTag() == "Enemy")
                //{
                //    collidedEntity.AsChild<HealthSystem>().TakeDamage(50)
                //}
            }

            //Set to dead state
            GetComponent<StateMachine>().SetStateCondition(1);
        }

        void OnStateEnter(string stateName)
        {
            if (stateName == "Dead")
            {
                if (!string.IsNullOrEmpty(BulletImpactAnim))
                {
                    ChangeAnim(BulletImpactAnim);
                    maxAnimTime = GetComponent<Animation>().GetMaxTime();
                    animTimer = 0.0f;
                }
                else
                {
                    Destroy();
                }
            }
        }

        void OnStateUpdate(string stateName, float dt)
        {
            if(stateName == "Dead")
            {
                animTimer += dt;

                if(animTimer > maxAnimTime)
                {
                    Destroy();
                }
            }
        }
    }
}

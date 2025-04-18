﻿/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

namespace Carmicah
{
    public enum BulletType
    {
        SHOOTER_BULLET,
        MAGE_BULLET,
        SPEAR_BULLET
    }

    public enum BulletTarget
    {
        GROUND,
        AIR
    }

    public class Projectile : Entity
    {
        public string BulletAnim = "Shooter_Projectile";
        public string BulletImpactAnim;
        public float Speed = 5.0f;
        public float LifeTime = 10.0f;
        public BulletType bulletType;
        public BulletTarget bulletTarget = BulletTarget.GROUND;
        //target enemy
        public Entity target;

        float timer = 0.0f;
        float animTimer = 0.0f;
        float maxAnimTime;

        public bool facingRight = false;
        //bool playDeathAnimation = false;

        public override void OnCreate()
        {
            // Set initial animation
            if (!string.IsNullOrEmpty(BulletAnim))
            {
                //CMConsole.Log("Changing animation to " + BulletAnim);
                ChangeAnim(BulletAnim);
            }

            // Set scale based on direction
            //Vector2 scale = Scale;
            //if (!facingRight)
            //{
            //    scale.x *= -1;
            //    Scale = scale;
            //}
        }

        public override void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null && pauseManager.As<PauseManager>().IsPaused)
                return;

            // Update lifetime
            timer += dt;
            if (timer >= LifeTime)
            {
                target = null;
                CMConsole.Log("Lifetime over!");
                GetComponent<StateMachine>().SetStateCondition(1);
                return;
            }

            if (target == null)
            {
                if(!GetComponent<Animation>().IsAnimFinished())
                {

                }
                else
                {
                    CMConsole.Log("TAOPMDOPSA");
                    GetComponent<StateMachine>().SetStateCondition(1);
                    return;
                }
            }

            else
            {
                if (target.mID == 0)
                {
                    target = null;
                    return;
                }
                // Move the bullet
                // this is dying for some reason
                if (HasComponent<RigidBody>())
                {
                    Vector2 mousePos = target.Position;
                    if(bulletType == BulletType.MAGE_BULLET)
                    {
                        if(!facingRight)
                        {
                            mousePos.x = -4;

                        }
                        else
                        {
                            mousePos.x = 4;
                        }
                    }

                    Vector2 dir = mousePos - Position;
                    dir.Normalize();
                    //CMConsole.Log($"target mouse??? {targetMouse.mID}");
                    //Vector2 dir = facingRight ? new Vector2(1, 0) : new Vector2(-1, 0);
                    GetComponent<RigidBody>().ApplyForce(dir, Speed);
                }

            }

        }

        // Set initial direction
        public void SetUp(Entity targetEnemy)
        {
            target = targetEnemy;

            if (targetEnemy != null)
            {
                // Get target direction
                Vector2 dir = targetEnemy.Position - Position;
                dir.Normalize();
                //CMConsole.Log($"{dir.x}, {dir.y}");
                if(bulletType == BulletType.SHOOTER_BULLET)
                {
                    if(bulletTarget == BulletTarget.AIR)
                    {
                        if(facingRight)
                        {
                            Rot = 0;
                        }
                        else
                        {
                            Rot = 80;
                        }
                    }
                    if(bulletTarget == BulletTarget.GROUND)
                    {
                        if(facingRight)
                        {
                            Rot = 225;
                        }
                        else
                        {
                            Rot = 205;
                        }
                    }
                }
                else if(bulletType == BulletType.MAGE_BULLET)
                {
                    float rot = Rot;
                    rot = 25;
                    Rot = rot;

                    //if(!facingRight)
                    //{
                    //    Vector2 scale = Scale;
                    //    scale.x *= -1;
                    //    Scale = scale;
                    //}
                }
                else if (bulletType == BulletType.SPEAR_BULLET)
                {
                    if(facingRight)
                    {
                        Rot = 190;
                    }
                    else
                    {
                        Rot = 170;
                    }
                }
            }
            else
            {
                CMConsole.Log("Error setting up bullet");
                Destroy();
            }
        }

        public void SetDirection(bool right)
        {
            facingRight = right;
        }

        void BulletDeathAnimation()
        {

        }

        public override void OnCollide(uint id)
        {
            if (bulletType == BulletType.MAGE_BULLET)
            {
                Sound.PlaySFX("Mage_Hit_Impact", 0.3f);
            }

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

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "Dead")
            {
                if (!string.IsNullOrEmpty(BulletImpactAnim))
                {
                    if(bulletType == BulletType.MAGE_BULLET)
                    {
                        Vector2 scale = Scale;
                        float rot = Rot;
                        if(this.HasComponent<Collider2D>())
                        {
                            this.GetComponent<Collider2D>().SetCustomWidth(0.5f);
                        }
                        

                        Sound.PlaySFX("Mage_Hit_Explosion", 0.3f);
                        if (!facingRight)
                        {
                            rot -= 5;
                            Rot = rot;
                            //scale.x *= -1;
                            //Scale = scale;

                        }
                        else
                        {
                            rot -= 85;
                            Rot = rot;
                        }
                    }

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

        public override void OnStateUpdate(string stateName, float dt)
        {
            if(stateName == "Dead")
            {
                animTimer += dt;

                if(GetComponent<Animation>().IsAnimFinished())
                {
                    Destroy();
                }
            }
        }
    }
}

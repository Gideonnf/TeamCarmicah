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
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    // Adding enum definitions that were missing
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

        //Bullet Variables
        public float Speed = 5.0f; //Speed of bullet
        public float LifeTime = 10.0f; //Time until it automatically disappears
        public float health = 3.0f; //Health of bullet (for spear projectile)
        public BulletType bulletType;
        public BulletTarget bulletTarget = BulletTarget.GROUND;
        public bool facingRight = false; //False == Left face of cake

        //target enemy
        public Entity target;

        // NPC that shot it
        //public Entity parent;

        float timer = 0.0f;
        float animTimer = 0.0f;
        float maxAnimTime;
        bool isDead = false;

        public override void OnCreate()
        {
            // Set initial animation
            if (!string.IsNullOrEmpty(BulletAnim))
            {
                ChangeAnim(BulletAnim);
            }
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
                GetComponent<StateMachine>().SetStateCondition(1);
                isDead = true;
                return;
            }

            if (target == null)
            {
                if (!GetComponent<Animation>().IsAnimFinished())
                {
                    if(bulletType == BulletType.MAGE_BULLET)
                    {
                        isDead = true;
                        return;
                    }
                }
                else
                {
                    GetComponent<StateMachine>().SetStateCondition(1);
                    isDead = true;
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
                if (HasComponent<RigidBody>() && !isDead)
                {
                    Vector2 mousePos = target.Position;
                    if (bulletType == BulletType.MAGE_BULLET)
                    {
                        if (!facingRight)
                        {
                            mousePos.x = -4;

                        }
                        else
                        {
                            mousePos.x = 4;
                        }
                    }

                    // Make sure target has the MouseAI component before accessing it
                    if (bulletType == BulletType.SPEAR_BULLET && target.Has<MouseAI>())
                    {
                        MouseAI mouseAI = target.As<MouseAI>();
                        switch (mouseAI.lane)
                        {
                            case 0:
                                {
                                    mousePos = mouseAI.startPosRight;
                                    break;
                                }
                            case 1:
                                {
                                    mousePos = mouseAI.startPosLeft;
                                    break;
                                }
                            case 2:
                                {
                                    mousePos = mouseAI.startPosRight2;
                                    break;
                                }
                            case 3:
                                {
                                    mousePos = mouseAI.startPosLeft2;
                                    break;
                                }
                        }
                        mousePos.y -= 4.0f;
                    }

                    Vector2 dir = mousePos - Position;
                    dir = dir.Normalize();
                    GetComponent<RigidBody>().ApplyForce(dir, Speed);
                    float dist = Position.Distance(mousePos);

                    if (bulletType == BulletType.SPEAR_BULLET)
                    {
                        if (dist <= 0.5f)
                        {
                            GetComponent<StateMachine>().SetStateCondition(1);
                        }
                    }
                }
            }
        }

        public void SetParent(Entity _parent)
        {
           // parent = _parent;
        }

        // Set initial direction
        public void SetUp(Entity targetEnemy)
        {
            target = targetEnemy;
            isDead = false;

            if (targetEnemy != null)
            {
                // Get target direction
                Vector2 dir = targetEnemy.Position - Position;
                dir = dir.Normalize();

                if (bulletType == BulletType.SHOOTER_BULLET)
                {
                    if (bulletTarget == BulletTarget.AIR)
                    {
                        if (facingRight)
                        {
                            Rot = 180;
                        }
                        else
                        {
                            Rot = 260;
                        }
                    }
                    if (bulletTarget == BulletTarget.GROUND)
                    {
                        if (facingRight)
                        {
                            Rot = 45;
                        }
                        else
                        {
                            Rot = 25;
                        }
                    }
                }
                else if (bulletType == BulletType.MAGE_BULLET)
                {
                    float rot = Rot;
                    rot = 25;
                    Rot = rot;
                }
                else if (bulletType == BulletType.SPEAR_BULLET)
                {
                    if (facingRight)
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
                Destroy();
            }
        }

        public void SetDirection(bool right)
        {
            facingRight = right;
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
                // Apply damage if this is an enemy
                if (collidedEntity.GetTag() == "Enemy")
                {
                    // Check if the entity has a HealthSystem component
                    if (collidedEntity.Has<HealthSystem>())
                    {
                        int damageAmount = 50; // Default damage

                        // Apply double damage in God Mode
                        if (Player.godMode)
                        {
                            damageAmount = 100; // Double damage in God Mode
                        }

                        collidedEntity.AsChild<HealthSystem>().TakeDamage(damageAmount);
                    }
                }
            }

            //Set to dead state
            if (bulletType == BulletType.MAGE_BULLET || bulletType == BulletType.SHOOTER_BULLET)
            {
                GetComponent<StateMachine>().SetStateCondition(1);
            }
        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "Dead")
            {
                isDead = true;
                if (!string.IsNullOrEmpty(BulletImpactAnim))
                {
                    if (bulletType == BulletType.MAGE_BULLET)
                    {
                        Vector2 scale = Scale;
                        float rot = Rot;
                        if (this.HasComponent<Collider2D>())
                        {
                            this.GetComponent<Collider2D>().SetCustomWidth(0.5f);
                        }

                        Sound.PlaySFX("Mage_Hit_Explosion", 0.3f);
                        if (!facingRight)
                        {
                            rot -= 5;
                            Rot = rot;
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
                    //if (parent != null)
                    //{
                    //    parent.As<BaseNPC>().ProjectileDestroyed();
                    //}
                    Destroy();
                }
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (stateName == "Alive")
            {
                if (health == 0)
                {
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }

            if (stateName == "Dead")
            {
                animTimer += dt;

                if (GetComponent<Animation>().IsAnimFinished())
                {
                    //if (parent != null)
                    //{
                    //    parent.As<BaseNPC>().ProjectileDestroyed();
                    //}
                    Destroy();
                }
            }
        }

        public override void OnTriggerEnter(uint collidedEntity)
        {
            Entity entity = FindEntityWithID(collidedEntity);

            if (entity != null)
            {
                if (entity.GetTag() == "Enemy")
                {
                    if (Player.godMode)
                    {
                        health -= 0.5f; // Make spears last longer in God Mode
                    }
                    else
                    {
                        health--;
                    }
                }
            }
        }
    }
}
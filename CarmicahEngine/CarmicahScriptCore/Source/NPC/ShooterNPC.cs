﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ShooterNPC : BaseNPC
    {
        public string airAnim;
        Entity target;
        BulletTarget targetType;
        float timer = 0.0f;
        string voiceOver;
        public float xAirOffset = 0.0f;
        public float yAirOffset = 0.0f;


        public override void OnCreate()
        {
            base.OnCreate();

            

            npcType = AbilityType.SHOOTER;

        }

        public override void OnUpdate(float dt)
        {
            if(pauseManager.IsPaused)
            {
                return;
            }
        }

        public override void ProjectileDestroyed()
        {
           // projectile = null;
        }

        public override void ShootProjectile()
        {
            if (target != null)
            {
                Entity projectile = CreateGameObject(projectilePrefab);
                Vector2 shootOffset;
                if (targetType == BulletTarget.GROUND)
                {
                    shootOffset = new Vector2(xShootOffset, yShootOffset);
                }
                else
                {
                    shootOffset = new Vector2(xAirOffset, yAirOffset);
                }
                if (projectile != null)
                {
                    if (IsLeft)
                    {
                        shootOffset.y *= -1;
                        projectile.Position = Position - shootOffset;
                    }
                    else
                    {
                        projectile.Position = Position + shootOffset;

                    }
                    Projectile bullet = projectile.As<Projectile>();
                    bullet.SetParent(this);
                    bullet.As<Projectile>().bulletType = BulletType.SHOOTER_BULLET;
                    if(targetType == BulletTarget.AIR)
                    {
                        bullet.As<Projectile>().bulletTarget = BulletTarget.AIR;
                    }

                    if(IsLeft)
                    {
                        bullet.As<Projectile>().facingRight = false;
                    }
                    else
                    {
                        bullet.As<Projectile>().facingRight = true;
                    }

                    Random rnd = new Random();
                    int number = rnd.Next(1, 6);
                    string soundFile = "Shooting_v3_0" + number.ToString();
                    
                    Sound.PlaySFX(soundFile, 0.5f);
                    if (bullet != null)
                    {
                        bullet.target = target;

                        bullet.SetUp(target);
                    }

                    if (mana > 0)
                    {
                        mana--; 
                    }
                }
            }
        }

        public override void GetTarget()
        {
            //CMConsole.Log($"Mouse lane one: {gameManager.mouseLaneOne.Count}");
            //CMConsole.Log($"Mouse lane two: {gameManager.mouseLaneTwo.Count}");
            //CMConsole.Log($"Mouse lane three: {gameManager.mouseLaneThree.Count}");
            //CMConsole.Log($"Mouse lane four: {gameManager.mouseLaneFour.Count}");

            float distance = float.MaxValue;

            switch (lane)
            {
                case 0:
                    {
                        foreach (MouseAI mouse in gameManager.mouseLaneOne)
                        {
                            if(mouse.isDead() == true)
                            {
                                continue;
                            }
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");
                            if (dist < distance)
                            {
                                distance = dist;
                                target = mouse;
                                targetType = BulletTarget.GROUND;
                            }
                        }
                    }
                    break;
                case 1:
                    {
                        foreach(MouseAI mouse in gameManager.mouseLaneTwo)
                        {
                            if (mouse.isDead() == true)
                            {
                                continue;
                            }
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                target = mouse;
                                targetType = BulletTarget.GROUND;
                            }
                        }
                    }
                    break;
                case 2:
                    {
                        foreach(MouseAI mouse in gameManager.mouseLaneThree)
                        {
                            if (mouse.isDead() == true)
                            {
                                continue;
                            }
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                target = mouse;
                                targetType = BulletTarget.GROUND;
                            }
                        }
                    }
                    break;
                case 3:
                    {
                        foreach(MouseAI mouse in gameManager.mouseLaneFour)
                        {
                            if (mouse.isDead() == true)
                            {
                                continue;
                            }
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                target = mouse;
                                targetType = BulletTarget.GROUND;
                            }
                        }
                    }
                    break;
            }

            switch(IsLeft)
            {
                case true:
                {    
                    foreach(FlyingEnemyAI bird in gameManager.flyingEnemyLeft)
                    {
                        float dist = bird.Position.Distance(Position);

                        if(dist < distance)
                        {
                            distance = dist;
                            target = bird;
                                targetType = BulletTarget.AIR;
                            }
                    }
                    break;
                }
                case false:
                {
                    foreach (FlyingEnemyAI bird in gameManager.flyingEnemyRight)
                    {
                        float dist = bird.Position.Distance(Position);

                        if (dist < distance)
                        {
                            distance = dist;
                            target = bird;
                            targetType = BulletTarget.AIR;
                        }
                    }
                    break;
                }
            }
            if(distance == float.MaxValue)
            {
                target = null;
            }
        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "Idle")
            {
                timer = 0.0f;

                ChangeAnim(idleAnim);
                //shot = false;
                //CMConsole.Log("IDLE ANIM");
            }
            else if (stateName == "Attacking")
            {
                //CMConsole.Log("TESTING Enter State");
                if (targetType == BulletTarget.AIR)
                {
                    ChangeAnim(airAnim);
                }
                if(targetType == BulletTarget.GROUND)
                { 
                    ChangeAnim(shootAnim); 
                }
                shot = false;
               // CMConsole.Log($"Max Anim Time : {animationTime}");

            }
            else if (stateName == "NoMana")
            {
                ChangeAnim(manaAnim);
                //CMConsole.Log("Out of Ammo!");
            }
            else if (stateName == "Teleport")
            {
                Entity[] children = GetAllChildren();
                foreach (Entity ent in children)
                {
                    if (ent != null)
                    {
                        ent.GetComponent<Renderer>().SetAlpha(0.0f);
                    }
                }
                ChangeAnim(teleportAnim);
                
            }
            else if (stateName == "Dead")
            {
                Sound.PlaySFX("Shooter_Death", 0.8f);
                Sound.PlaySFX("NPC_Death", 0.8f);
                ChangeAnim(dissolveAnim);
                //CMConsole.Log("NPC Dying");
            }


            //CMConsole.Log($"Enter State Name: {stateName}");
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (active == false) return;

            if (pauseManager.IsPaused)
            {
                return;
            }
            

            // idk if this will happen but if the mouse dies
            // this script might still hold a refeence to a 0 id mouse
            // which will cause crashes
            if (target != null && target.mID == 0)
            {
                target = null;
                // Change back to idle state
                //if (stateName == "Attacking")
                //    GetComponent<StateMachine>().SetStateCondition(1);
                return;
            }

            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Idle")
            {
                timer += dt;

                // Get nearest enemy 
                //targetMouse = gameManager.GetClosestMouse(this);
                if (timer > shootTime)
                {
                    GetTarget(); // get targetMouse
                    if (target != null)
                    {
                        //CMConsole.Log($"Target mouse : {target.mID}");
                        if (targetType == BulletTarget.GROUND)
                        {
                            if (target.As<MouseAI>().isDead())
                            {
                                // CMConsole.Log("Target Mouse died already");
                                return;
                            }
                            else
                            {
                                if (mana > 0)
                                {
                                    // CMConsole.Log("Trying to attack!");
                                    GetComponent<StateMachine>().SetStateCondition(2);
                                }
                                else
                                {
                                    GetComponent<StateMachine>().SetStateCondition(3);
                                }
                            }
                        }

                        else if (targetType == BulletTarget.AIR)
                        {
                            if (target.As<FlyingEnemyAI>().isDead())
                            {
                                return;
                            }
                            else
                            {
                                if (mana > 0)
                                {
                                    //CMConsole.Log("Trying to attack!");
                                    GetComponent<StateMachine>().SetStateCondition(2);
                                }
                                else
                                {
                                    GetComponent<StateMachine>().SetStateCondition(3);
                                }
                            }
                        }
                    }
                }
                if (mana == 0)
                {
                    GetComponent<StateMachine>().SetStateCondition(3);
                }
            }
            else if (stateName == "Attacking")
            {
                //CMConsole.Log($"Shooting timer : {timer}");
                if (!shot && target != null)
                {
                    if (targetType == BulletTarget.AIR)
                    {
                        if (GetComponent<Animation>().GetFrameNo() == 7)
                        {
                            ShootProjectile();
                            //CMConsole.Log("Shooting Air");
                            shot = true;
                        }
                    }
                    else if (targetType == BulletTarget.GROUND)
                    {
                        if (GetComponent<Animation>().GetFrameNo() == 7)
                        {
                            ShootProjectile();
                            shot = true;
                        }
                    }
                }
                else
                {
                    if (GetComponent<Animation>().IsAnimFinished())
                    {
                        //CMConsole.Log("Going back to Idle");
                        GetComponent<StateMachine>().SetStateCondition(1);
                    }
                }

            }
            else if (stateName == "NoMana")
            {
                //TODO: Implement Logic with MC
                if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT) && hovering)
                {
                    //CMConsole.Log("MC Should try to heal " + mID.ToString());
                    player.HealAI(mID);

                    
                }
            }
            else if(stateName == "Dead")
            {
                if (GetComponent<Animation>().IsAnimFinished())
                {
                    Destroy();
                }
            }

        }

        public override void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");
            if (stateName == "NoMana")
            {
                if (mana > 0)
                {

                    PlayVoiceOver();

                }
            }

            else if (stateName == "Teleport")
            {
                Entity[] children = GetAllChildren();
                foreach (Entity ent in children)
                {
                    if (ent != null)
                    {
                        ent.GetComponent<Renderer>().SetAlpha(0.3528999984264374f);
                    }
                }
                // ChangeAnim(TeleportAnim);
            }



        }

        public void PlayVoiceOver()
        {
            Random rnd = new Random();
            int number = rnd.Next(1, 11);

            if (number > 9)
            {

                voiceOver = "VO_Shooter_Placement_" + number.ToString();

            }
            else
            {
                voiceOver = "VO_Shooter_Placement_0" + number.ToString();
            }

            Sound.PlaySFX(voiceOver, 1.5f);
        }

        public override void OnMouseEnter()
        {
            base.OnMouseEnter();
        }

        public override void OnMouseHover()
        {
            base.OnMouseHover();
        }

        public override void OnMouseExit()
        {
            base.OnMouseExit();
        }
    }
}

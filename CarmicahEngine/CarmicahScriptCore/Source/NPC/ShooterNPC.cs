using System;
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

        public override void ShootProjectile()
        {
            if (target != null)
            {
                Entity projectile = CreateGameObject(projectilePrefab);
                if (projectile != null)
                {
                    projectile.Position = Position;
                    // CMConsole.Log($"Projectile Position: {projectile.Position.x}, {projectile.Position.y}");

                    Projectile bullet = projectile.As<Projectile>();
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
        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "Idle")
            {
                ChangeAnim(idleAnim);
                CMConsole.Log("IDLE ANIM");
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
                animationTime = GetComponent<Animation>().GetMaxTime();
                timer = 0.0f;
                shot = false;
               // CMConsole.Log($"Max Anim Time : {animationTime}");

            }
            else if (stateName == "NoMana")
            {
                ChangeAnim(manaAnim);
                CMConsole.Log("Out of Ammo!");
            }
            else if (stateName == "Teleport")
            {
                ChangeAnim(teleportAnim);
            }
            else if (stateName == "Dead")
            {
                Sound.PlaySFX("Shooter_Death", 0.8f);
                Sound.PlaySFX("NPC_Death", 0.8f);
                ChangeAnim(dissolveAnim);
                CMConsole.Log("NPC Dying");
            }


            //CMConsole.Log($"Enter State Name: {stateName}");
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (active == false) return;

            // idk if this will happen but if the mouse dies
            // this script might still hold a refeence to a 0 id mouse
            // which will cause crashes
            if (target != null && target.mID == 0)
            {
                CMConsole.Log("I AM HERE");
                target = null;
                // Change back to idle state
                //if (stateName == "Attacking")
                //    GetComponent<StateMachine>().SetStateCondition(1);
                return;
            }

            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Idle")
            {
                // Get nearest enemy 
                //targetMouse = gameManager.GetClosestMouse(this);
                GetTarget(); // get targetMouse
                if (target != null)
                {
                    CMConsole.Log($"Target mouse : {target.mID}");

                    // change to attacking state
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

                if (mana == 0)
                {
                    GetComponent<StateMachine>().SetStateCondition(3);
                }
            }
            else if (stateName == "Attacking")
            {
                //CMConsole.Log($"Shooting timer : {timer}");

                timer += dt;
                if (timer > shootTime)
                {
                    if (!shot && target != null)
                    {
                        if(targetType == BulletTarget.AIR)
                        {
                            if(GetComponent<Animation>().GetFrameNo() == 7)
                            {
                                ShootProjectile();
                                shot = true;
                            }
                        }
                        else if(targetType == BulletTarget.GROUND)
                        {
                            ShootProjectile();
                            shot = true;
                        }
                    }
                    else
                    {
                        if (timer >= animationTime)
                        {
                            GetComponent<StateMachine>().SetStateCondition(1);
                        }
                    }

                }
            }
            else if (stateName == "NoMana")
            {
                //TODO: Implement Logic with MC
                if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT) && hovering)
                {
                    CMConsole.Log("MC Should try to heal " + mID.ToString());
                    player.HealAI(mID);

                    if (player.GetComponent<Animation>().IsAnimFinished())
                    {
                        Random rnd = new Random();
                        int number = rnd.Next(1, 6);
                        string soundFile = "Shooting_v3_0" + number.ToString();

                        Sound.PlaySFX(soundFile, 0.5f);
                    }
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class MageNPC : BaseNPC
    {
        // TODO: Mage NPC shouldn't be using target mouse
        // it just needs a target position then the projectile should have its own mageProjectile class
        // that just reaches the position and explodes compared to shooter npc

        // Ill leave it the exact same as shooter for now
        // some one else do this pls

        MouseAI targetMouse;
        float timer = 0.0f;
        public override void OnCreate()
        {
            base.OnCreate();
            npcType = AbilityType.MAGE;

        }

        public override void OnUpdate(float dt)
        {
            if (pauseManager.IsPaused)
            {
                return;
            }
        }

        public override void ShootProjectile()
        {
            if (targetMouse != null)
            {
                Entity projectile = CreateGameObject(projectilePrefab);
                if (projectile != null)
                {
                    projectile.Position = Position;
                    // CMConsole.Log($"Projectile Position: {projectile.Position.x}, {projectile.Position.y}");

                    Projectile bullet = projectile.As<Projectile>();
                    bullet.As<Projectile>().bulletType = BulletType.MAGE_BULLET;

                    Sound.PlaySFX(shootSound);
                    if (bullet != null)
                    {
                        bullet.targetMouse = targetMouse;

                        bullet.SetUp(targetMouse);
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
                                targetMouse = mouse;
                            }
                        }

                        foreach (MouseAI mouse in gameManager.mouseLaneTwo)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                    }
                    break;
                case 1:
                    {
                        foreach (MouseAI mouse in gameManager.mouseLaneOne)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }

                        foreach (MouseAI mouse in gameManager.mouseLaneTwo)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                    }
                    break;
                case 2:
                    {
                        foreach (MouseAI mouse in gameManager.mouseLaneThree)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }

                        foreach (MouseAI mouse in gameManager.mouseLaneFour)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                    }
                    break;
                case 3:
                    {
                        foreach (MouseAI mouse in gameManager.mouseLaneThree)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }

                        foreach (MouseAI mouse in gameManager.mouseLaneFour)
                        {
                            float dist = mouse.Position.Distance(Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                    }
                    break;
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
                ChangeAnim(shootAnim);
                animationTime = GetComponent<Animation>().GetMaxTime();
                timer = 0.0f;
                shot = false;
                CMConsole.Log($"Max Anim Time : {animationTime}");

            }
            else if (stateName == "NoMana")
            {
                ChangeAnim(manaAnim);
                CMConsole.Log("Out of Ammo!");
            }


            //CMConsole.Log($"Enter State Name: {stateName}");
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (active == false) return;

            // idk if this will happen but if the mouse dies
            // this script might still hold a refeence to a 0 id mouse
            // which will cause crashes
            if (targetMouse != null && targetMouse.mID == 0)
            {
                CMConsole.Log("I AM HERE");
                targetMouse = null;
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
                if (targetMouse != null)
                {
                    CMConsole.Log($"Target mouse : {targetMouse.mID}");

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
                    if (!shot && targetMouse != null)
                    {
                        ShootProjectile();
                        shot = true;

                        // reset the timer
                        // timer = 0.0f;
                    }
                    else
                    {
                        if (timer >= animationTime)
                            GetComponent<StateMachine>().SetStateCondition(1);
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

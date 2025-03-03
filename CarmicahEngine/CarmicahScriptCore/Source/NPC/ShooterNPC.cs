using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ShooterNPC : BaseNPC
    {
        MouseAI targetMouse;
        float timer = 0.0f;
        public override void OnCreate()
        {
            base.OnCreate();

        }

        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_L))
            {
                GetTarget();
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

                    Sound.PlaySFX(shootSound);

                    if (mana > 0)
                        mana--;
                    //if (bullet != null)
                    //{
                    //    bullet.targetMouse = targetMouse;

                    //    bullet.SetUp(targetMouse);

                    //    if (mana > 0)
                    //    {
                    //        mana--;
                    //    }
                    //}
                }
            }
        }

        public override void GetTarget()
        {
            CMConsole.Log($"Mouse lane one: {gameManager.mouseLaneOne.Count}");
            CMConsole.Log($"Mouse lane two: {gameManager.mouseLaneTwo.Count}");
            CMConsole.Log($"Mouse lane three: {gameManager.mouseLaneThree.Count}");
            CMConsole.Log($"Mouse lane four: {gameManager.mouseLaneFour.Count}");

        }

        public void OnStateEnter(string stateName)
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

        public void OnStateUpdate(string stateName, float dt)
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

        public void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");

        }


        public void OnMouseEnter()
        {
            //CMConsole.Log("Hovering!");
            hovering = true;
        }

        public void OnMouseHover()
        {
            //CMConsole.Log("Hovering!");
            hovering = true;
        }

        public void OnMouseExit()
        {
            hovering = false;
        }

    }
}

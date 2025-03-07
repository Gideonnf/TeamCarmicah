using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    //public enum HeroType
    //{
    //    SHOOTER,
    //    MAGE,
    //    SPEAR
    //}
    public class HeroAI : Entity
    {
        public string idleAnim = "Shooter_Idle";
        public string shootAnim = "Shooter_Shoot";
        public string manaAnim = "Shooter_Idle";

        public string projectilePrefab = "Bullet";
        public float shootRate = 1.0f;
        public float shootTime = 1.0f;
        public int mana = 5;
        public int lane;
        float timer = 0.0f;
        public bool active = false;
        public bool IsLeft = false;
        public AbilityType type;

        // float shootTimer = 0.0f;
        bool isShooting = false;
        bool shot = false;
        bool hasAmmo = true;
        bool hovering = false;
        GameManager gameManager;
        PauseManager pauseManager;
        Player player;
        MouseAI targetMouse;
        float animationTime;

        void OnCreate()
        {
            gameManager = FindEntityWithName("GameManager").As<GameManager>();
            pauseManager = FindEntityWithName("PauseManager").As<PauseManager>();
            player = FindEntityWithName("mainCharacter").As<Player>();
        }

        void OnUpdate(float dt)
        {
            if (pauseManager.IsPaused) { return; }
            // CMConsole.Log("TESTING IF CMLOG WORKS");

            if (isShooting)
            {
                //shootTimer += dt;
                //if (shootTimer >= shootRate)
                //{
                //    shootTimer = 0.0f;
                //    ShootProjectile();
                //}
            }
        }

        void ShootProjectile()
        {
            // Get the closest enemy to the hero based on the side
            //MouseAI targetMouse = gameManager.GetClosestMouse(this);
            // if it found a target
            if (targetMouse != null)
            {
                // CMConsole.Log($"mouse retrieved : {targetMouse}");
                Entity projectile = CreateGameObject(projectilePrefab);
                if (projectile != null)
                {
                    //  CMConsole.Log($"Hero Position: {Position.x}, {Position.y}");
                    projectile.Position = Position;
                    // CMConsole.Log($"Projectile Position: {projectile.Position.x}, {projectile.Position.y}");

                    Projectile bullet = projectile.As<Projectile>();
                    switch(type)
                    {
                        case AbilityType.SHOOTER:
                            bullet.bulletType = BulletType.SHOOTER_BULLET;
                            break;
                        case AbilityType.MAGE:
                            bullet.bulletType = BulletType.MAGE_BULLET;
                            break;
                    }
                    Sound.PlaySFX(this.mID, "Shooting_v2");
                    if (bullet != null)
                    {
                        bullet.targetMouse = targetMouse;

                        bullet.SetUp(targetMouse);

                        if (mana > 0)
                        {
                            mana--;
                        }
                    }
                }
            }
        }

        public void HealAmmo()
        {
            mana = 5;
            CMConsole.Log("Restocking Ammo");
            GetComponent<StateMachine>().SetStateCondition(1);
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
            //else if (stateName == "Dead")
            //{
            //    ChangeAnim(dissolve)
            //}


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
                targetMouse = gameManager.GetClosestMouse(this);
                if (targetMouse != null)
                {
                    CMConsole.Log($"Target mouse : {targetMouse.mID}");

                    // change to attacking state
                    if(mana > 0)
                    {
                        //CMConsole.Log("Trying to attack!");
                        GetComponent<StateMachine>().SetStateCondition(2);
                    }
                    else
                    {
                        GetComponent<StateMachine>().SetStateCondition(3);
                    }
                }

                if(mana == 0)
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
                if(Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT) && hovering)
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

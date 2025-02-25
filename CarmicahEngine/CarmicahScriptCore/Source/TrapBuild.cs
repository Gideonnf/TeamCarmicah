using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
   public class TrapBuild : Entity
    {
        public string TrapPrefabName = "Trap_1";


        public string TrapTranslucentPrefab = "TrapTranslucent";
        public bool IsLeft = false;
        public float depthVal = 3.45f;
        public float trapOffset = 1.6f;


        Entity translucentTrap;
        Entity trapEntity;
        Entity TrapIcon;
        bool hovering = false;
        bool built = false;
        bool enemyCollided = false;

        float redCol;
       // bool isActive = false;

        public void OnCreate()
        {
            
            TrapIcon = FindEntityWithName("TrapIcon");



        }

        public void OnUpdate(float dt)
        {

            // incase trap entity gets destroyed, it needs to update to null here
            if (trapEntity != null && trapEntity.mID == 0)
            {
                CMConsole.Log("Trap entity destroyed");
                trapEntity = null;
                built = false;
            }

            if (enemyCollided && translucentTrap != null)
            {
                // change color to red
                translucentTrap.GetComponent<Renderer>().SetColour(1.0f, 0.2f, 0.2f);
               // CMConsole.Log($"Setting red color to new");

            }
            else if (translucentTrap != null)
            {
                translucentTrap.GetComponent<Renderer>().SetColour(1.0f, 1.0f, 1.0f);
                //CMConsole.Log($"Setting red color to original");

            }


            if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity != null)
            {
               // CMConsole.Log("Trying to build a trap");
                if (translucentTrap == null && built == false)
                {
                    //CMConsole.Log("It shouldnt be here atm");
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    CMConsole.Log($"{Position.x} , {Position.y}");
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x + trapOffset, Position.y);
                    translucentTrap.GetComponent<Transform>().Depth = depthVal;
                    if (IsLeft)
                    {
                        Vector2 scale = translucentTrap.GetComponent<Transform>().Scale;
                        translucentTrap.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x - trapOffset, Position.y);

                    }
                    //redCol = translucentTrap.GetComponent<Renderer>().Red;
                    //CMConsole.Log($"Red Color : {redCol}");
                }
                
            }
            else if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity == null)
            {
                if (translucentTrap != null)
                {
                    // dont let them build if an enemy is taking the spot
                    if (enemyCollided)
                    {
                        translucentTrap.Destroy();
                        translucentTrap = null;
                        return;
                    }


                    // if the player let go when its hovering a build spot
                    if (hovering && trapEntity == null)
                    {
                        // build a trap
                        built = true;
                        trapEntity = CreateGameObject(TrapPrefabName);
                        CMConsole.Log($"{Position.x} , {Position.y}");
                        trapEntity.GetComponent<Transform>().Position = new Vector2(translucentTrap.Position.x, translucentTrap.Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;
                        trapEntity.As<TrapAI>().built = true;
                        trapEntity.As<TrapAI>().SetBuildEntity(this);
                        
                        Sound.PlaySFX("trap_placement", 0.5f);

                        if (IsLeft)
                        {
                            Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                            trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);

                            //trapEntity.Position = new Vector2(translucentTrap.Position.x, translucentTrap.Position.y);
                        }

                       // trapEntity.Position = new Vector2(trapEntity.Position.x + trapOffset, trapEntity.Position.y);
                    }

                    translucentTrap.Destroy();
                    translucentTrap = null;

                }
            }
        }


        public void OnMouseEnter()
        {
            hovering = true;
        }

        public void OnMouseHover()
        {
            hovering = true;
        }

        public void OnMouseExit()
        {
            hovering = false;
        }

        public void TrapDead()
        {
            trapEntity = null;
            built = false;
        }

        public override void OnTriggerEnter(uint collidedEntity)
        {
            Entity mouseEntity = FindEntityWithID(collidedEntity);
            if (mouseEntity.GetTag() == "Enemy")
            {
                //CMConsole.Log("Colliding with enemy");
                enemyCollided = true;
            }

            //CMConsole.Log("Colliding with smth");
        }

        public override void OnTriggerStay(uint collidedEntity)
        {
            Entity mouseEntity = FindEntityWithID(collidedEntity);
            if (mouseEntity.GetTag() == "Enemy")
            {
                //CMConsole.Log("Colliding with enemy");
                enemyCollided = true;
            }
        }

        public override void OnTriggerExit()
        {
           // CMConsole.Log("Not colliding with enemy");
            enemyCollided = false;
        }
    }
}

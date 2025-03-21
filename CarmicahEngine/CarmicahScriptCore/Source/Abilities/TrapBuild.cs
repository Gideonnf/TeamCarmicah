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
        string soundFile = "Trap_Placement";

        public bool IsLeft = false;
        public float depthVal = 3.45f;
        float trapxOffset;
        float trapyOffset;
        


        Entity translucentTrap;
        Entity trapEntity;
        //Entity CandyConeTrapIcon;
        //Entity HoneyTrapIcon;
        Entity trapIcon;
        public AbilityType type;

        bool hovering = false;
        bool built = false;
        bool enemyCollided = false;

        //float redCol;
       // bool isActive = false;

        public override void OnCreate()
        {
            //CandyConeTrapIcon = FindEntityWithName("CandyConeTrapIcon");
            //HoneyTrapIcon = FindEntityWithName("HoneyTrapIcon");
        }

        public override void OnUpdate(float dt)
        {

            // incase trap entity gets destroyed, it needs to update to null here
            if (trapEntity != null && trapEntity.mID == 0)
            {
                //CMConsole.Log("Trap entity destroyed");
                trapEntity = null;
                built = false;
            }

            if (trapIcon != null && trapIcon.mID == 0)
            {
                trapIcon = null;
                if (translucentTrap != null)
                {
                    translucentTrap.Destroy();
                    translucentTrap = null;
                }
                return;
            }

            // for some reason destroying it will leave a reference here
            // even tho i set null???
            // so i have to add these
            // same as trapicon above
            if (translucentTrap != null && translucentTrap.mID == 0)
            {
                translucentTrap = null;
                return;
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

            if (trapIcon != null)
            {
                TrapBuildChecks(trapIcon);
            }

            //switch(type)
            //{
            //    case AbilityType.CANDY_CONE:
            //        TrapBuildChecks(CandyConeTrapIcon);
            //        break;

            //    case AbilityType.HONEY:
            //        TrapBuildChecks(HoneyTrapIcon);
            //        break;
            //}
        }


        void TrapBuildChecks(Entity entity)
        {
            trapxOffset = entity.As<TrapIcon>().trapxOffset;
            trapyOffset = entity.As<TrapIcon>().trapyOffset;
            if (entity != null && entity.As<BaseIcon>().trapEntity != null)
            {
                // CMConsole.Log("Trying to build a trap");
                if (translucentTrap == null && built == false)
                {

                    //CMConsole.Log("It shouldnt be here atm");
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    //CMConsole.Log($"{Position.x} , {Position.y}");
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x + trapxOffset, Position.y + trapyOffset);
                    translucentTrap.GetComponent<Transform>().Depth = depthVal;
                    translucentTrap.GetComponent<Renderer>().SetAlpha(0.3f);
                    if (IsLeft)
                    {
                        Vector2 scale = translucentTrap.GetComponent<Transform>().Scale;
                        translucentTrap.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x - trapxOffset, Position.y + trapyOffset);

                    }
                    //redCol = translucentTrap.GetComponent<Renderer>().Red;
                    //CMConsole.Log($"Red Color : {redCol}");
                }

            }
            else if (entity != null && entity.As<BaseIcon>().trapEntity == null)
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
                        // If its wave start, this will trigger cooldown
                        entity.As<TrapIcon>().ActivateCooldown();

                        // build a trap
                        built = true;
                        trapEntity = CreateGameObject(TrapPrefabName);
                       // CMConsole.Log($"{Position.x} , {Position.y}");
                        trapEntity.GetComponent<Transform>().Position = new Vector2(translucentTrap.Position.x, translucentTrap.Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;
                        trapEntity.As<TrapAI>().built = true;
                        trapEntity.As<TrapAI>().SetBuildEntity(this);
                        if(type == AbilityType.CANDY_CONE)
                        {
                            trapEntity.As<TrapAI>().type = AbilityType.CANDY_CONE;
                            soundFile += "_v2";
                        }
                        else if(type == AbilityType.HONEY)
                        {
                            trapEntity.As<TrapAI>().type = AbilityType.HONEY;
                            soundFile += "_Honey";
                        }

                        else if (type == AbilityType.JELLYBEAN)
                        {
                            trapEntity.As<TrapAI>().type = AbilityType.JELLYBEAN;
                            soundFile += "_Honey";
                        }



                        Sound.PlaySFX(soundFile, 0.5f);

                        //To reset it
                        soundFile = "Trap_Placement";

                        if (IsLeft)
                        {
                            Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                            trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                            if(type == AbilityType.CANDY_CONE)
                            {
                            trapEntity.GetComponent<Collider2D>().SetxPivot(1.0f);
                            }

                            //trapEntity.Position = new Vector2(translucentTrap.Position.x, translucentTrap.Position.y);
                        }

                        trapIcon.As<TrapIcon>().TrapBuilt();

                        // trapEntity.Position = new Vector2(trapEntity.Position.x + trapOffset, trapEntity.Position.y);
                    }

                    translucentTrap.Destroy();
                    translucentTrap = null;
                    //
                    trapIcon = null;
                    return;
                }
            }
        }



        public override void OnMouseEnter()
        {
            hovering = true;
        }

        public override void OnMouseHover()
        {
            hovering = true;
        }

        public override void OnMouseExit()
        {
            hovering = false;
        }

        public void TrapDead()
        {
            trapEntity = null;
            built = false;
        }

        public void SetTrapType(AbilityType trapType, string trapPrefabName, string fakeTrapName, Entity icon)
        {
            //CMConsole.Log("Shouldnt Honey be running this too?");
            type = trapType;
            TrapPrefabName = trapPrefabName;
            TrapTranslucentPrefab = fakeTrapName;
            trapIcon = icon;
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

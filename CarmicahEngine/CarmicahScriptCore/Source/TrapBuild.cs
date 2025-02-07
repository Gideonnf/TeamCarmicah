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

        Entity translucentTrap;
        Entity trapEntity;
        Entity TrapIcon;
        bool hovering = false;
        bool built = false;
        bool isActive = false;

        public void OnCreate()
        {
            
            TrapIcon = FindEntityWithName("TrapIcon");



        }

        public void OnUpdate(float dt)
        {
            // incase trap entity gets destroyed, it needs to update to null here
            if (trapEntity != null && trapEntity.mID == 0)
            {
                trapEntity = null;
            }

            if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity != null)
            {
                if (translucentTrap == null && built == false)
                {
                    //CMConsole.Log("It shouldnt be here atm");
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    CMConsole.Log($"{Position.x} , {Position.y}");
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                    translucentTrap.GetComponent<Transform>().Depth = depthVal;
                    if (IsLeft)
                    {
                        Vector2 scale = translucentTrap.GetComponent<Transform>().Scale;
                        translucentTrap.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                    }

                }
            }
            else if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity == null)
            {
                if (translucentTrap != null)
                {
                    translucentTrap.Destroy();
                    translucentTrap = null;

                    // if the player let go when its hovering a build spot
                    if (hovering && trapEntity == null)
                    {
                        // build a trap
                        built = true;
                        trapEntity = CreateGameObject(TrapPrefabName);
                        CMConsole.Log($"{Position.x} , {Position.y}");
                        trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;
                        Sound.PlaySFX("trap_placement", 0.5f);

                        if (IsLeft)
                        {
                            Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                            trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        }

                    }
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
    }
}

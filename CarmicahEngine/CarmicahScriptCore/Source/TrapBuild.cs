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

            if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity != null )
            {
                if (translucentTrap == null && built == false) 
                {
                    //CMConsole.Log("It shouldnt be here atm");
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
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
                        trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;

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
            //if (trapEntity != null) { return; }

            //trapEntity = CreateGameObject(TrapPrefabName);
            ////Vector2 newPos = new Vector2(-5, -5);
            //Console.WriteLine($"Position of build: {Position.x}, {Position.y}");
            ////Vector2 newPos = new Vector2(Position.x, Position.y);
            ////trapEntity.As<Testing>().SetUpPosition(newPos);
            //trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
        }

        public void OnMouseHover()
        {
            hovering = true;

           // Console.WriteLine($"Position of Trap: {Position.x}, {Position.y}");
            //if (translucentTrap != null)
            //{

            //   // Console.WriteLine($"Position of build: {Position.x}, {Position.y}");
            //   // trapEntity.Position = Position;
            //  //  trapEntity.As<Testing>().SetUpPosition(Position);
            //   // trapEntity.Position = new Vector2(Position.x, Position.y);
            //    //Console.WriteLine($"Position of trap: {trapEntity.Position.x}, {trapEntity.Position.y}");
            //}
        }

        public void OnMouseExit()
        {
            //trapEntity.Destroy();
            //trapEntity = null;
            hovering = false;
        }
    }
}

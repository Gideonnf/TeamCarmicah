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

        Entity translucentTrap;
        Entity trapEntity;
        Entity TrapIcon;
        public void OnCreate()
        {
            TrapIcon = FindEntityWithName("TrapIcon");

        }

        public void OnUpdate(float dt)
        {
            if (TrapIcon != null && translucentTrap == null)
            {
                if (TrapIcon.As<TrapIcon>().trapEntity != null)
                {
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                }
            }
            else if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity == null)
            {
                if (translucentTrap != null)
                {
                    translucentTrap.Destroy();
                    translucentTrap = null;

                }
            }
        }


        public void OnMouseEnter()
        {
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
           // Console.WriteLine($"Position of Trap: {Position.x}, {Position.y}");
            if (translucentTrap != null)
            {

               // Console.WriteLine($"Position of build: {Position.x}, {Position.y}");
               // trapEntity.Position = Position;
              //  trapEntity.As<Testing>().SetUpPosition(Position);
               // trapEntity.Position = new Vector2(Position.x, Position.y);
                //Console.WriteLine($"Position of trap: {trapEntity.Position.x}, {trapEntity.Position.y}");
            }
        }

        public void OnMouseExit()
        {
            //trapEntity.Destroy();
            //trapEntity = null;
        }
    }
}

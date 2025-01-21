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
        Entity trapEntity;
        public void OnMouseEnter()
        {
            if (trapEntity != null) { return; }

            trapEntity = CreateGameObject(TrapPrefabName);
            //Vector2 newPos = new Vector2(-5, -5);
            Console.WriteLine($"Position of build: {Position.x}, {Position.y}");
            //Vector2 newPos = new Vector2(Position.x, Position.y);
            //trapEntity.As<Testing>().SetUpPosition(newPos);
            trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
        }

        public void OnMouseHover()
        {
           // Console.WriteLine($"Position of Trap: {Position.x}, {Position.y}");
            if (trapEntity != null)
            {
                Console.WriteLine($"Position of build: {Position.x}, {Position.y}");
                trapEntity.Position = Position;
              //  trapEntity.As<Testing>().SetUpPosition(Position);
               // trapEntity.Position = new Vector2(Position.x, Position.y);
                //Console.WriteLine($"Position of trap: {trapEntity.Position.x}, {trapEntity.Position.y}");
            }
        }

        public void OnMouseExit()
        {
            trapEntity.Destroy();
        }
    }
}

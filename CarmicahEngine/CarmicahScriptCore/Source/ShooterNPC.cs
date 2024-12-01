/* File Documentation ----------------------------------------------------------------------------- 
file:           ShooterNPC.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `ShooterNPC` class, a type of entity that can toggle between 
                shooting and idle animations. It references a `GameManager` entity and includes 
                methods for animation management during runtime.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ShooterNPC : Entity
    {
        public string ShootAnim = "Shooter_Shoot";
        public string IdleAnim = "Shooter_Idle";
        GameManager gmReference;


        void OnCreate()
        {
            // Note: I foresee an issue that if a npc is created in runtime and game manager is not yet initialized this will crash
            // but i dont think that will ever happen
            // but if it does then this is why :pepepray:
            gmReference = FindEntityWithName("GameManager").As<GameManager>();
        }

        void OnUpdate(float dt)
        {
            //timer += dt;
            //if (timer >= shootTimer)
            //{
            //    timer = 0.0f;
            //    ChangeAnim(ShootAnim);
            //    //MouseAI targetMouse = gmReference.GetFirstMouse().As<MouseAI>();
            //    //Console.WriteLine($"Getting Mouse {targetMouse}");
            //    //if (targetMouse != null)
            //    //{
            //    //    Console.WriteLine($"Killing Mouse {targetMouse}");
            //    //    targetMouse.KillMouse();
            //    //}
            //}
            //else
            //{
            //    ChangeAnim(IdleAnim);
            //}
        }

        public void ToggleShooting()
        {
            ChangeAnim(ShootAnim);
        }

        public void ToggleIdle()
        {
            ChangeAnim(IdleAnim);
        }
    }
}

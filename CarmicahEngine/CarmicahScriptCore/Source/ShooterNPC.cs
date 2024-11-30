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

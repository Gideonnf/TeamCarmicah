using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {mID}");

        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_W))
            {

                Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
        }
    }
}

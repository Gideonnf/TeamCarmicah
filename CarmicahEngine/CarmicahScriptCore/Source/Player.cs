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
        List<Vector2> waypointsRight;
        List<Vector2> waypointsLeft;
        bool isLeft;
        int currPoint;

        Vector2 originalPos;
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate Test 54 - {mID}");
            waypointsRight = new List<Vector2>();
            waypointsLeft = new List<Vector2>();
            originalPos = new Vector2();
            //Random rand = new Random();
            isLeft = false;//rand.Next(0, 2) == 1; // 
            currPoint = 0;

            Entity leftPoint0 = FindEntityWithName("Duck2_5");
            Entity leftPoint1 = FindEntityWithName("Duck2_3");
            Entity leftPoint2 = FindEntityWithName("Duck2_4");
            // waypointsLeft.Add(new Vector2(leftPoint0.Position.x, leftPoint0.Position.y));
            Console.WriteLine($"Player.OnCreate Test 54 - {leftPoint0.mID}");

            Entity rightPoint0 = FindEntityWithName("Duck2");
            Entity rightPoint1 = FindEntityWithName("Duck2_2");
            Entity rightPoint2 = FindEntityWithName("Duck2_1");



            //if (isLeft)
            //{
            //    Position = waypointsLeft[currPoint];
            //}
            //else
            //{
            //    Position = waypointsRight[currPoint];
            //}

            //currPoint++;

            //// store original position
            //originalPos = Position;
        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyHold(Keys.KEY_W))
            {

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, 1), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_A))
            {

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(-1, 0), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_S))
            {

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(0, -1), 2.0f);
            }
            if (Input.IsKeyHold(Keys.KEY_D))
            {

                //Console.WriteLine("Thoughts and prayers. It do :b: like that sometimes");

                GetComponent<RigidBody>().ApplyForce(new Vector2(1,0), 2.0f);
            }

        }
    }
}

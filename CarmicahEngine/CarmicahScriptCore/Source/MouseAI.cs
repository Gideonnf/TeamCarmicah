using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class MouseAI : Entity
    {/*
        //   StateMachine mouseSM;
        List<Vector2> waypointsRight;
        List<Vector2> waypointsLeft;
        bool isLeft;
        int currPoint;

        Vector2 originalPos;
        Entity player;

        void OnCreate()
        {
            waypointsRight = new List<Vector2>();
            waypointsLeft = new List<Vector2>();
            originalPos = new Vector2();
            //Random rand = new Random();
            isLeft = false;//rand.Next(0, 2) == 1; // 
            currPoint = 0;
            player = FindEntityWithName("mainChracter");

            Entity leftPoint0 = FindEntityWithName("Duck2_5");
            Entity leftPoint1 = FindEntityWithName("Duck2_3");
            Entity leftPoint2 = FindEntityWithName("Duck2_4");
            waypointsLeft.Add(leftPoint0.Position);
            waypointsLeft.Add(leftPoint1.Position);
            waypointsLeft.Add(leftPoint2.Position);

            Entity rightPoint0 = FindEntityWithName("Duck2");
            Entity rightPoint1 = FindEntityWithName("Duck2_2");
            Entity rightPoint2 = FindEntityWithName("Duck2_1");
            waypointsRight.Add(rightPoint0.Position);
            waypointsRight.Add(rightPoint1.Position);
            waypointsRight.Add(rightPoint2.Position);


            if (isLeft)
            {
                Position = waypointsLeft[currPoint];
            }
            else
            {
                Position = waypointsRight[currPoint];
            }

            currPoint++;

            // store original position
            originalPos = Position;*/

        }

      /*  void OnUpdate(float dt)
        {
            //mouseSM.Update(ref mouseSM);
            //Entity 
            //Console.WriteLine($"Player.OnCreate - {player.mID}");

            // Get the direction to the next waypoint
            if (isLeft)
            {
                // Get direction
                Vector2 dir = (waypointsLeft[currPoint] - Position).Normalize();
                // APply force in that direction
                GetComponent<RigidBody>().ApplyForce(dir, 2.0f);

                // Check distance to waypoint
                float dist = Position.Distance(waypointsLeft[currPoint]);
                if (dist <= 0.1f)
                {
                    if (currPoint < waypointsLeft.Count - 1)
                        currPoint++;
                    else
                    {
                        currPoint = 0;
                        // reset pos
                        Position = originalPos;
                    }
                }
            }
            else
            {
                // Get direction
                Vector2 dir = (waypointsRight[currPoint] - Position).Normalize();
                // APply force in that direction
                GetComponent<RigidBody>().ApplyForce(dir, 2.0f);

                // Check distance to waypoint
                float dist = Position.Distance(waypointsRight[currPoint]);
                if (dist <= 0.1f)
                {
                    if (currPoint < waypointsRight.Count - 1)
                        currPoint++;
                    else
                    {
                        currPoint = 0;
                        // reset pos
                        Position = originalPos;
                    }
                }
            }
*/

            //// If its dead, kill it
            //if (mouseSM.GetCurrentState() == "MouseDead")
            //{
            //    // kill it
            //}
        }

    }
}

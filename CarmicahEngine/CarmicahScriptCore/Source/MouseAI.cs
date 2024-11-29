using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class MouseAI : Entity
    {
        //   StateMachine mouseSM;
        List<Vector2> waypointsRight;
        List<Vector2> waypointsLeft;
        public bool isLeft = false;
        int currPoint;

        Vector2 originalPos;
        Entity player;

        void OnCreate()
        {
            // initialize state machine
            waypointsRight = new List<Vector2>();
            waypointsLeft = new List<Vector2>();
            originalPos = new Vector2();
           // Random rand = new Random();

            // set the initial state
           // isLeft = rand.Next(0, 2) == 1; // 
            currPoint = 0;

            // Get the player
            //Entity rightChar = FindEntityWithName("mainCharacter_2");  // Right character
            //Entity leftChar = FindEntityWithName("mainCharacter_3");   // Left character

            // Determine which path to take based on closest character
            //float distToRightChar = Position.Distance(rightChar.Position);
            //float distToLeftChar = Position.Distance(leftChar.Position);

            //// Set path based on closest character
            //isLeft = distToLeftChar < distToRightChar;

            //// Set the active player to watch out for
            //player = isLeft ? leftChar : rightChar;


            Entity leftPoint0 = FindEntityWithName("wall");
            Entity leftPoint1 = FindEntityWithName("wall_1");
            Entity leftPoint2 = FindEntityWithName("wall_2");
            waypointsRight.Add(leftPoint0.Position);
            waypointsRight.Add(leftPoint1.Position);
            waypointsRight.Add(leftPoint2.Position);

            Entity rightPoint0 = FindEntityWithName("wall_3");
            Entity rightPoint1 = FindEntityWithName("wall_4");
            Entity rightPoint2 = FindEntityWithName("wall_5");
            waypointsLeft.Add(rightPoint0.Position);
            waypointsLeft.Add(rightPoint1.Position);
            waypointsLeft.Add(rightPoint2.Position);


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
            originalPos = Position;

        }

     void OnUpdate(float dt)
        {
            //mouseSM.Update(ref mouseSM);
            //Entity 
            //Console.WriteLine($"Mouse is moving - {mID}, onLeft - {isLeft}");
            //if (player != null)
            //{
            //    float distToPlayer = Position.Distance(player.Position);
            //    if (distToPlayer < 3.0f) // Detection radius
            //    {
            //        // Run away from player
            //        Vector2 awayDir = (Position - player.Position).Normalize();
            //        GetComponent<RigidBody>().ApplyForce(awayDir, 3.0f);
            //        return; // Skip waypoint following while escaping
            //    }
            //}
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


            //// If its dead, kill it
            //if (mouseSM.GetCurrentState() == "MouseDead")
            //{
            //    // kill it
            //}
        }

    void OnCollide()
        {

        }

    }
}



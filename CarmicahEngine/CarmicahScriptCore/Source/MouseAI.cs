using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


/*namespace Carmicah
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

        // variables for respawning
        bool shouldRespawn = false;
        float respawnTimer = 0f;
        const float RESPAWN_DELAY = 0.2f;

        void OnCreate()
        {
            // initialize state machine
            waypointsRight = new List<Vector2>();
            waypointsLeft = new List<Vector2>();
            currPoint = 0;

            // Load right path points
            Entity leftPoint0 = FindEntityWithName("wall");
            Entity leftPoint1 = FindEntityWithName("wall_1");
            Entity leftPoint2 = FindEntityWithName("wall_2");
            waypointsRight.Add(leftPoint0.Position);
            waypointsRight.Add(leftPoint1.Position);
            waypointsRight.Add(leftPoint2.Position);

            // Load left path points
            Entity rightPoint0 = FindEntityWithName("wall_3");
            Entity rightPoint1 = FindEntityWithName("wall_4");
            Entity rightPoint2 = FindEntityWithName("wall_5");
            waypointsLeft.Add(rightPoint0.Position);
            waypointsLeft.Add(rightPoint1.Position);
            waypointsLeft.Add(rightPoint2.Position);

            // Set spawn position based on path
            if (isLeft)
            {
                Position = waypointsLeft[0];
                originalPos = waypointsLeft[0];
            }
            else
            {
                Position = waypointsRight[0];
                originalPos = waypointsRight[0];
            }
            // Start moving to second point
            currPoint = 1;

            //Sound.PlaySFX("spawn", 0.5f);
        }

        void OnUpdate(float dt)
        {
            if (shouldRespawn)
            {
                respawnTimer += dt;
                if (respawnTimer >= RESPAWN_DELAY)
                {
                    // Reset position to original spawn point
                    ResetPosition();

                    //Sound.PlaySFX("spawn", 0.5f);
                }
                return;
            }

            // Get direction
            List<Vector2> currentPath = isLeft ? waypointsLeft : waypointsRight;

            // Get direction to next waypoint
            Vector2 dir = (currentPath[currPoint] - Position).Normalize();
            GetComponent<RigidBody>().ApplyForce(dir, 2.0f);

            // Check if reached waypoint
            float dist = Position.Distance(currentPath[currPoint]);
            if (dist <= 0.2f)
            {
                if (currPoint < currentPath.Count - 1)
                {
                    currPoint++;
                }
                else
                {
                    // Trigger respawn
                    shouldRespawn = true;
                    respawnTimer = 0f;
                   // Position = isLeft ? waypointsLeft[0] : waypointsRight[0];
                    //Sound.PlaySFX("death", 0.5f);
                }
            }
        }
        private void ResetPosition()
        {
            // Apply a zero force first to stop any movement
            GetComponent<RigidBody>().ApplyForce(new Vector2(0, 0), 0f);
            Position = isLeft ? waypointsLeft[0] : waypointsRight[0];
            currPoint = 1;
            shouldRespawn = false;
            //Sound.PlaySFX("spawn", 0.5f);
        }

    }
}
*/



namespace Carmicah
{
    public class MouseAI : Entity
    {
        List<Vector2> waypointsRight;
        List<Vector2> waypointsLeft;
        public bool isLeft = false;
        int currPoint;
        Vector2 originalPos;
        StateMachine stateMachine;

        void OnCreate()
        {
            waypointsRight = new List<Vector2>();
            waypointsLeft = new List<Vector2>();
            currPoint = 0;

            InitWaypoints();
            SetInitialPosition();

            // Initialize state machine
            stateMachine = new StateMachine();
            stateMachine.AddState(new MouseChase("Chase"));
            stateMachine.AddState(new MouseDead("Dead"));
            stateMachine.SetNextState("Chase");
        }

        void OnUpdate(float dt)
        {
            stateMachine.Update(ref stateMachine);

            // Only update movement if in chase state
            if (stateMachine.GetCurrentState() == "Chase")
            {
                UpdateMovement(dt);
            }
            else if (stateMachine.GetCurrentState() == "Dead")
            {
                ResetPosition();
                stateMachine.SetNextState("Chase");
            }
        }

        void InitWaypoints()
        {
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
        }

        void SetInitialPosition()
        {
            if (isLeft)
            {
                Position = waypointsLeft[0];
                originalPos = waypointsLeft[0];
            }
            else
            {
                Position = waypointsRight[0];
                originalPos = waypointsRight[0];
            }
            currPoint = 1;
        }

        void UpdateMovement(float dt)
        {
            List<Vector2> currentPath = isLeft ? waypointsLeft : waypointsRight;
            Vector2 dir = (currentPath[currPoint] - Position).Normalize();
            GetComponent<RigidBody>().ApplyForce(dir, 2.0f);

            float dist = Position.Distance(currentPath[currPoint]);
            if (dist <= 0.2f)
            {
                if (currPoint < currentPath.Count - 1)
                {
                    currPoint++;
                }
                else
                {
                    stateMachine.SetNextState("Dead");
                }
            }
        }

        void ResetPosition()
        {
            GetComponent<RigidBody>().ApplyForce(new Vector2(0, 0), 0f);
            Position = isLeft ? waypointsLeft[0] : waypointsRight[0];
            currPoint = 1;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;


namespace Carmicah
{
    public class MouseAI : Entity
    {
        public string SpawnPointEntityLeft;
        public string SpawnPointEntityRight;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;

        public bool isLeft = false;
        //int currPoint;
        Vector2 startPosLeft;
        Vector2 startPosRight;
        StateMachine stateMachine;
        Entity endEntityLeft;
        Entity endEntityRight;

        void OnCreate()
        {
            startPosLeft = FindEntityWithName(SpawnPointEntityLeft).Position;
            startPosRight = FindEntityWithName(SpawnPointEntityRight).Position;
            endEntityLeft = FindEntityWithName(EndPointEntityLeft);
            endEntityRight = FindEntityWithName(EndPointEntityRight);

            // InitWaypoints();
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
                Destroy();
            }
        }

        //void InitWaypoints()
        //{

        //}

        public void SetInitialPosition()
        {
           // Console.WriteLine($"Position Before : {Position.x} , {Position.y}");
            if (isLeft)
            {
                Position = startPosLeft;
                //originalPos = waypointsLeft[0];
            }
            else
            {
                Position = startPosRight;
            }
           // Console.WriteLine($"Position After : {Position.x} , {Position.y}");

            //currPoint = 1;
        }

        void UpdateMovement(float dt)
        {
            Vector2 endPos = isLeft ? endEntityLeft.Position : endEntityRight.Position;
            Vector2 dir = (endPos - Position).Normalize();
            GetComponent<RigidBody>().ApplyForce(dir, 2.0f);

            float dist = Position.Distance(endPos);
            if (dist <= 0.2f)
            {
                 stateMachine.SetNextState("Dead");
            }
        }

        //void ResetPosition()
        //{
        //    GetComponent<RigidBody>().ApplyForce(new Vector2(0, 0), 0f);
        //    //Position = isLeft ? waypointsLeft[0] : waypointsRight[0];
        //    currPoint = 1;
        //}
    }
}

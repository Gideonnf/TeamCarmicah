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
        public string Animation0;
        public string Animation1;
        public string Animation2;
        public string Animation3;

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
            Random rand = new Random();
            int randomInt = rand.Next(0, 4); // rand between 1 to 3

            switch(randomInt)
            {
                case 0:
                    //Console.WriteLine($"Trying to change Anim {Animation0}");
                    ChangeAnim(Animation0);

                    break;
                case 1:
                    //Console.WriteLine($"Trying to change Anim {Animation1}");

                    ChangeAnim(Animation1);

                    break;
                case 2:
                    //Console.WriteLine($"Trying to change Anim {Animation2}");

                    ChangeAnim(Animation2);

                    break;
                case 3:
                   // Console.WriteLine($"Trying to change Anim {Animation3}");

                    ChangeAnim(Animation3);

                    break;
            }
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
                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                gm.MouseDestroyed(this);
                Destroy();
            }
        }

        void OnCollide()
        {
            Console.WriteLine("HELP ME");
            KillMouse();
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

        public void KillMouse()
        {
            stateMachine.SetNextState("Dead");
        }

        //void ResetPosition()
        //{
        //    GetComponent<RigidBody>().ApplyForce(new Vector2(0, 0), 0f);
        //    //Position = isLeft ? waypointsLeft[0] : waypointsRight[0];
        //    currPoint = 1;
        //}
    }
}

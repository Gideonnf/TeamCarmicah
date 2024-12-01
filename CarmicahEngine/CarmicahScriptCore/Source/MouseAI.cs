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

        public string SpawnPointEntityLeft2;
        public string SpawnPointEntityRight2;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;

        public bool isLeft = false;
        public string Animation0;
        public string Animation1;
        public string Animation2;
        public string Animation3;

        public string AnimationDie0;
        public string AnimationDie1;
        public string AnimationDie2;
        public string AnimationDie3;

        //int currPoint;
        Vector2 startPosLeft;
        Vector2 startPosRight;
        Vector2 startPosLeft2;
        Vector2 startPosRight2;
        StateMachine stateMachine;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;

        public float ChanceToDie = 0.12f;
        public float TimeToDie = 1.5f;
        public float timer;
        public float DeathTime = 1.5f;

        int animType = 0;
        int randLane = 0;

        void OnCreate()
        {
            if (FindEntityWithName(SpawnPointEntityLeft) != null)
                startPosLeft = FindEntityWithName(SpawnPointEntityLeft).Position;
            if (FindEntityWithName(SpawnPointEntityRight) != null)
                startPosRight = FindEntityWithName(SpawnPointEntityRight).Position;
            if (FindEntityWithName(SpawnPointEntityLeft2) != null)
                startPosLeft2 = FindEntityWithName(SpawnPointEntityLeft2).Position;
            if (FindEntityWithName(SpawnPointEntityRight2) != null)
                startPosRight2 = FindEntityWithName(SpawnPointEntityRight2).Position;

            endEntityLeft = FindEntityWithName(EndPointEntityLeft);
            endEntityRight = FindEntityWithName(EndPointEntityRight);
            endEntityLeft2 = FindEntityWithName(EndPointEntityLeft2);
            endEntityRight2 = FindEntityWithName(EndPointEntityRight2);

            // InitWaypoints();
            SetInitialPosition();

            // Initialize state machine
            stateMachine = new StateMachine();
            stateMachine.AddState(new MouseChase("Chase"));
            stateMachine.AddState(new MouseDead("Dead"));
            stateMachine.SetNextState("Chase");
            Random rand = new Random();
            animType = rand.Next(0, 4); // rand between 1 to 3
            randLane = rand.Next(0, 4); // rand between 1 to 3

            switch(animType)
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
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }
            stateMachine.Update(ref stateMachine);

            // Only update movement if in chase state
            if (stateMachine.GetCurrentState() == "Chase")
            {
                UpdateMovement(dt);
                timer += dt;
                if (timer >= TimeToDie)
                {
                    timer = 0.0f;
                    Random rand = new Random();

                    float randFloat = (float)rand.NextDouble();
                    //Console.WriteLine($"Random float : {randFloat}");
                    if (randFloat <= ChanceToDie)
                    {
                        timer = 0.0f;
                        stateMachine.SetNextState("Dead");

                        switch (animType)
                        {
                            case 0:
                                //Console.WriteLine($"Trying to change Anim {Animation0}");
                                ChangeAnim(AnimationDie0);

                                break;
                            case 1:
                                //Console.WriteLine($"Trying to change Anim {Animation1}");

                                ChangeAnim(AnimationDie1);

                                break;
                            case 2:
                                //Console.WriteLine($"Trying to change Anim {Animation2}");

                                ChangeAnim(AnimationDie2);

                                break;
                            case 3:
                                // Console.WriteLine($"Trying to change Anim {Animation3}");

                                ChangeAnim(AnimationDie3);

                                break;
                        }
                    }
                }
            }
            else if (stateMachine.GetCurrentState() == "Dead")
            {
                timer += dt;

                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                if(timer >= DeathTime)
                {
                    if(gm != null)
                        gm.MouseDestroyed(this);

                    timer = 0.0f;
                    Destroy();
                }
            }
        }

        public void SetInitialPosition()
        {
            Vector2 scale = Scale;

            // Console.WriteLine($"Position Before : {Position.x} , {Position.y}");
            switch (randLane)
            {
                case 0:
       
                    Position = startPosLeft;

                    break;
                case 1:
                    Position = startPosLeft2;
                    scale.x *= -1;
                    Scale = scale;
                    break;
                case 2:
                    Position = startPosRight;

                    break;
                case 3:
                    Position = startPosRight2;
                    scale.x *= -1;
                    Scale = scale;
                    break;
            }
           // Console.WriteLine($"Position After : {Position.x} , {Position.y}");

            //currPoint = 1;
        }

        void UpdateMovement(float dt)
        {
            Vector2 endPos = Vector2.Zero;  //endEntityLeft.Position : endEntityRight.Position;
            switch (randLane)
            {
                case 0:
                    if (endEntityLeft != null)
                        endPos = endEntityLeft.Position;

                    break;
                case 1:
                    if (endEntityLeft2 != null)
                        endPos = endEntityLeft2.Position;

                    break;
                case 2:
                    if (endEntityRight != null)
                        endPos = endEntityRight.Position;

                    break;
                case 3:
                    if (endEntityRight2 != null)
                        endPos = endEntityRight2.Position;
                    break;
            }
            Vector2 dir = (endPos - Position).Normalize();
            if (HasComponent<RigidBody>())
            {
                GetComponent<RigidBody>().ApplyForce(dir, 1.0f);

            }

            float dist = Position.Distance(endPos);
            if (dist <= 0.3f)
            {
                timer = 0.0f;
                stateMachine.SetNextState("Dead");
                switch (animType)
                {
                    case 0:
                        //Console.WriteLine($"Trying to change Anim {Animation0}");
                        ChangeAnim(AnimationDie0);

                        break;
                    case 1:
                        //Console.WriteLine($"Trying to change Anim {Animation1}");

                        ChangeAnim(AnimationDie1);

                        break;
                    case 2:
                        //Console.WriteLine($"Trying to change Anim {Animation2}");

                        ChangeAnim(AnimationDie2);

                        break;
                    case 3:
                        // Console.WriteLine($"Trying to change Anim {Animation3}");

                        ChangeAnim(AnimationDie3);

                        break;
                }

            }
        }

        void OnCollide()
        {
            timer = 0.0f;

            stateMachine.SetNextState("Dead");
            switch (animType)
            {
                case 0:
                    //Console.WriteLine($"Trying to change Anim {Animation0}");
                    ChangeAnim(AnimationDie0);

                    break;
                case 1:
                    //Console.WriteLine($"Trying to change Anim {Animation1}");

                    ChangeAnim(AnimationDie1);

                    break;
                case 2:
                    //Console.WriteLine($"Trying to change Anim {Animation2}");

                    ChangeAnim(AnimationDie2);

                    break;
                case 3:
                    // Console.WriteLine($"Trying to change Anim {Animation3}");

                    ChangeAnim(AnimationDie3);

                    break;
            }

            //  Console.WriteLine("Testing OnCollide");
        }

        public void KillMouse()
        {
            timer = 0.0f;
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

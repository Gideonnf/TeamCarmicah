/* File Documentation ----------------------------------------------------------------------------- 
file:           MouseAI.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `MouseAI` class, which manages mouse entity behavior using 
                a state machine. It includes movement logic, animation changes, and interaction 
                with other entities. The class handles states such as "Chase" and "Dead" for 
                dynamic gameplay.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace Carmicah
{
    public enum MouseType
    {
        Regular,
        Fast,
        Heavy
    }
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

        //public bool isLeft = false;
        public string Animation0;
        public string Animation1;
        public string Animation2;
        public string Animation3;

        public string AnimationDie0;
        public string AnimationDie1;
        public string AnimationDie2;
        public string AnimationDie3;

        public string DeathSound = "mouse die btr";
        public string InjuredSound = "mouse die";

        public int lane;
        public EnemyTypes enemyType;

        //int currPoint;
        Vector2 startPosLeft;
        Vector2 startPosRight;
        Vector2 startPosLeft2;
        Vector2 startPosRight2;
        //StateMachine stateMachine;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;

        public float ChanceToDie = 0.12f;


        public MouseType mouseType = MouseType.Regular;

        // Base speeds for each type
        private float baseRegularSpeed = 1.0f;
        private float baseFastSpeed = 1.5f;
        private float baseHeavySpeed = 1.8f;

       


        public float TimeToDie = 1.5f;
        public float timer;
        public float DeathTime = 2.0f;
        public float Speed = 1.0f;



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


            if (FindEntityWithName(SpawnPointEntityLeft) != null)
                startPosLeft = FindEntityWithName(SpawnPointEntityLeft).Position;

            // Initialize state machine
            //stateMachine = new StateMachine();
            //stateMachine.AddState(new MouseChase("Chase"));
            //stateMachine.AddState(new MouseDead("Dead"));
            //stateMachine.SetNextState("Chase");
            Random rand = new Random();
            animType = rand.Next(0, 4); // rand between 0 to 3
            randLane = rand.Next(0, 4); // rand between 0 to 3

            lane = randLane;

            //int mouseTypeRand = rand.Next(0, 3); // Random type

            //switch (mouseTypeRand)
            //{
            //    case 0:
            //        mouseType = MouseType.Regular;
            //        Speed = baseRegularSpeed;
            //        break;
            //    case 1:
            //        mouseType = MouseType.Fast;
            //        Speed = baseFastSpeed;
            //        break;
            //    case 2:
            //        mouseType = MouseType.Heavy;
            //        Speed = baseHeavySpeed;
            //        break;
            //}

            Sound.PlaySFX("Portal_Spawn", 0.3f);

            switch (animType)
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
        }

        public void SetInitialPosition()
        {
            Vector2 scale = Scale;

            // Console.WriteLine($"Position Before : {Position.x} , {Position.y}");
            switch (randLane)
            {
                case 0:
       
                    Position = startPosLeft;
                    lane = 1;
                    break;
                case 1:
                    Position = startPosLeft2;
                    lane = 3;
                    scale.x *= -1;
                    Scale = scale;
                    break;
                case 2:
                    Position = startPosRight;
                    lane = 0;
                    break;
                case 3:
                    Position = startPosRight2;
                    lane = 2;
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
                GetComponent<RigidBody>().ApplyForce(dir, Speed);

            }

            float dist = Position.Distance(endPos);
            //CMConsole.Log($"Distance to end {dist}");

            if (dist <= 0.3f)
            {
               // CMConsole.Log("Dying");

                timer = 0.0f;
                GetComponent<StateMachine>().SetStateCondition(1);

                Entity mainCharacter = FindEntityWithName("mainCharacter");
                mainCharacter.As<Player>().TakeDamage(10);
            }
        }

        void OnCollide(uint id)
        {

            // its alr dead
            if (mID == 0)
            {
                return;
            }

            Entity collidedEntity = FindEntityWithID(id);
            if (collidedEntity != null)
            {
                if (collidedEntity.GetTag() == "Trap")
                {
                    if (!collidedEntity.As<TrapAI>().built) return;

                    this.AsChild<HealthSystem>().TakeDamage(100);
                }
                else if (collidedEntity.GetTag() == "Bullet")
                {
                    //CMConsole.Log($"Collided Entity : {collidedEntity.mID} with tag {collidedEntity.GetTag()}");
                    this.AsChild<HealthSystem>().TakeDamage(50);
                }
            }

            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                GetComponent<StateMachine>().SetStateCondition(1);

            }

            //timer = 0.0f;
            //GetComponent<StateMachine>().SetStateCondition(1);
            //Sound.PlaySFX("mouse die", 0.5f);
            //CMConsole.Log("Dying");
        }

        public void KillMouse()
        {
            timer = 0.0f;
            GetComponent<StateMachine>().SetStateCondition(1);
            //stateMachine.SetNextState("Dead");
        }

        public void OnStateEnter(string stateName)
        {
            //GetComponent<StateMachine>().SetStateCondition(1);
            // CMConsole.Log($"Update State Name: {stateName}");
            
            if (stateName == "Dead")
            {
                timer = 0.0f;
                Sound.PlaySFX(InjuredSound, 0.5f);
                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                if (gm != null)
                    gm.EntityDestroyed(this);

                //CMConsole.Log("TESTING Enter State");
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

        public void OnStateUpdate(string stateName, float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }
            Entity gameManager = FindEntityWithName("GameManager");
            CMConsole.Log($"game manager gameOver :{gameManager.As<GameManager>().GameOver}");
            if (gameManager.As<GameManager>().GameOver)
                return;



            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Running")
            {
                // CMConsole.Log("TESTING Update State");
                UpdateMovement(dt);
            }
            else if (stateName == "Dead")
            {
                
                timer += dt;
                if (timer >= GetComponent<Animation>().GetMaxTime())
                {
                    Sound.PlaySFX(DeathSound, 0.5f);
                    timer = 0.0f;
                    Destroy();
                }
            }
            //CMConsole.Log($"mouse retrieved : {targetMouse}");
        }

        public void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");

        }

        //public void SetTypeAndSpeedWithWaveScaling(int waveNumber)
        //{
        //    // Wave-based speed scaling
        //    float waveMultiplier = 1.0f + (waveNumber * 0.15f); // 15% increase per wave
        //    Speed *= waveMultiplier;

        //    // Optional: You can adjust other properties based on wave number
        //    // For example, higher health for later waves
        //    if (this.AsChild<HealthSystem>() != null)
        //    {
        //        int baseHealth = 100;
        //        int bonusHealth = waveNumber * 10; // 10 extra health per wave
        //        this.AsChild<HealthSystem>().SetMaxHealth(baseHealth + bonusHealth);
        //    }

        //    CMConsole.Log($"Mouse type: {mouseType}, Wave: {waveNumber}, Final Speed: {Speed}");
        //}

    }
}

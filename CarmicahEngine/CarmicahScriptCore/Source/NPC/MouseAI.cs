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
        // lane switching
        public float switchCooldown = 0.0f;     // cooldown for switching lanes
        public const float switchDelay = 5.0f;  // mice can only move again after 5 seconds after switching 
        public bool isSwitching = false;        // bool to check if mouse is currently switching lanes
        public Vector2 switchTargetPos;         // final x target after lane switch
        public const float xMoveSpeed = 2.5f;   // speed of x movement during lane switch
        public const float xSnapThreshold = 0.05f; // how long to snap to the target x position


        public string SpawnPointEntityLeft;
        public string SpawnPointEntityRight;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;

        public string SpawnPointEntityLeft2;
        public string SpawnPointEntityRight2;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;

        //public bool isLeft = false;
        public string baseAnimation;
        public string baseAnimationDie;
        public string baseAnimationDissolve;

        string soundFile;

        public string DeathSound = "mouse die btr";
        public string InjuredSound = "mouse die";

        public int lane;
        public EnemyTypes enemyType;

        //int currPoint;
        public Vector2 startPosLeft;
        public Vector2 startPosRight;
        public Vector2 startPosLeft2;
        public Vector2 startPosRight2;
        //StateMachine stateMachine;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;
        Entity mainCamera;

        public float ChanceToDie = 0.12f;


        public MouseType mouseType = MouseType.Regular;

        // Base speeds for each type
        //private float baseRegularSpeed = 1.0f;
        //private float baseFastSpeed = 2.0f;
        //private float baseHeavySpeed = 1.8f;

       
        bool isRunning = false;

        public float TimeToDie = 1.5f;
        public float timer;
        public float DeathTime = 2.0f;
        public float Speed;
        public float speedDebuff = 0.7f; // 60% slower
        public float cameraHeight = 10.0f;
        public float debuff = 1.0f;
        bool dead = false;
        //int animType = 0;
        int randLane = 0;
        bool move = false;

        public override void OnCreate()
        {
            mainCamera = FindEntityWithName("MainCamera");
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
            //


            if (FindEntityWithName(SpawnPointEntityLeft) != null)
                startPosLeft = FindEntityWithName(SpawnPointEntityLeft).Position;

            // Initialize state machine
            //stateMachine = new StateMachine();
            //stateMachine.AddState(new MouseChase("Chase"));
            //stateMachine.AddState(new MouseDead("Dead"));
            //stateMachine.SetNextState("Chase");
            //Random rand = new Random();
            //animType = CMRand.Range(0, 3); // rand between 0 to 3
            randLane = CMRand.Range(0, 4); // rand between 0 to 3

            //randLane = 3; //For Testing

            lane = randLane;
            //SetInitialPosition();
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

            ChangeAnim(baseAnimation);
        }

        public override void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            if (Input.IsKeyPressed(Keys.KEY_T)) // test to see if switching works
            {
                StartLaneSwitch();
                isSwitching = true; // this does nothing for now because there's no delay timer
                CMConsole.Log($"Switching lane");
            }

            // //test with key first, don't use chance variables yet
            //if (!move || isSwitching) return; // skip if not moving or already switching

            //if (switchCooldown > 0.0f)
            //{
            //    switchCooldown -= dt;
            //    return;
            //}

            //// Roll for chance to switch
            //if (CMRand.Range(0, 100) < 15) // 15% chance
            //{
            //    StartLaneSwitch();
            //    isSwitching = true;
            //    switchCooldown = switchDelay;
            //}
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            Entity gameManager = FindEntityWithName("GameManager");
            if (gameManager != null)
            {
                if (gameManager.As<GameManager>().GameOver)
                {
                    if (HasComponent<RigidBody>())
                    {
                        //CMConsole.Log("Stop Da mouse!");
                        GetComponent<RigidBody>().StopObject();
                    }
                    return;
                }
            }

            if (move)
            {
                if (isSwitching)
                {
                    Vector2 pos = Position; // get current position
                    float dx = switchTargetPos.x - pos.x; // get x distance to target

                    // if x distance is greater than threshold, move towards target
                    if (Math.Abs(dx) > xSnapThreshold)
                    {
                        // move towards target
                        pos.x += Math.Sign(dx) * xMoveSpeed * fixedDt;
                    }
                    else
                    {
                        pos.x = switchTargetPos.x; // this is to snap to the target x position
                        pos.x = (float)Math.Round(switchTargetPos.x, 2); // for 2 decimal precision
                        isSwitching = false; // reset immediately since movement comes later, no more moving allowed

                        // stop residual horizontal velocity (important!)
                        if (HasComponent<RigidBody>())
                            GetComponent<RigidBody>().StopObject();

                        CMConsole.Log($"[MouseAI] Mouse {mID} finished moving to lane {lane}"); // test
                    }

                    Position = pos; // set new position
                }

                // only move if not switching lanes
                UpdateMovement(fixedDt);
            }
        }

        public void SetInitialPosition()
        {
            Vector2 scale = Scale;

            //Console.WriteLine($"Position Before : {Position.x} , {Position.y}");
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
           //Console.WriteLine($"Position After : {Position.x} , {Position.y}");

            //currPoint = 1;
        }

        void UpdateMovement(float dt)
        {
            Vector2 endPos = Vector2.Zero;  //endEntityLeft.Position : endEntityRight.Position;
            //switch (randLane)
            switch(lane) // by changing from randLane to lane, 
            {
                case 0:
                    if (endEntityRight != null)
                        endPos = endEntityRight.Position;
                    break;
                case 1:
                    if (endEntityLeft != null)
                        endPos = endEntityLeft.Position;
                    break;
                case 2:
                    if (endEntityRight2 != null)
                        endPos = endEntityRight2.Position;
                    break;
                case 3:
                    if (endEntityLeft2 != null)
                        endPos = endEntityLeft2.Position;
                    break;
            }
            //Vector2 dir = (endPos - Position).Normalize();

            // fullDir is taking end position - current position, then normalize it -> normalize is to make it 1 unit long
            Vector2 fullDir = (endPos - Position).Normalize(); // this is the full direction, used when not switching lanes
            Vector2 dir = new Vector2(0.0f, fullDir.y); // only move upward (Y-axis)
            // something here is fucking up the hitboxes at the top

            //Vector2 nextPos = dir * (Speed * debuff) * dt;

            if (HasComponent<RigidBody>())
            {
                GetComponent<RigidBody>().ApplyForce(fullDir, Speed * debuff);
              // GetComponent<RigidBody>().Move(nextPos);
            }

          
        }

        public override void OnCollide(uint id)
        {

            // its alr dead
            if (mID == 0)
            {
                return;
            }

            if (dead) return;


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

        public override void OnTriggerEnter(uint collidedEntity)
        {
            if (mID == 0)
            {
                return;
            }

            Entity entity = FindEntityWithID(collidedEntity);
            if (entity != null)
            {
                if (entity.GetTag() == "Trap")
                {
                    if (!entity.As<TrapAI>().built) return;

                   // CMConsole.Log("COLLIDING WITH HONEY TRAP");
                    //this.AsChild<HealthSystem>().TakeDamage(100);
                }

                if(entity.GetTag() == "Bullet")
                {
                    this.AsChild<HealthSystem>().TakeDamage(50);
                }

            }

            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                GetComponent<StateMachine>().SetStateCondition(1);

            }
        }

        public override void OnTriggerStay(uint collidedEntity)
        {
            Entity entity = FindEntityWithID(collidedEntity);
            if (entity != null)
            {
                if (entity.GetTag() == "Trap")
                {
                    if (!entity.As<TrapAI>().built) return;

                    //CMConsole.Log("COLLIDING WITH HONEY TRAP");
                    debuff = speedDebuff;
                    //this.AsChild<HealthSystem>().TakeDamage(100);
                }
            }
        }

        public override void OnTriggerExit(uint collidedEntity)
        {
            // reset
            //CMConsole.Log("Exiting some kinematic other thing");
            debuff = 1.0f;
        }
        public void KillMouse()
        {
            timer = 0.0f;
            GetComponent<StateMachine>().SetStateCondition(1);
            //stateMachine.SetNextState("Dead");
        }

        public override void OnStateEnter(string stateName)
        {
            //GetComponent<StateMachine>().SetStateCondition(1);
            // CMConsole.Log($"Update State Name: {stateName}");
            
            if (stateName == "Dead")
            {
                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                if (gm != null)
                    gm.EntityDestroyed(this);

                //CMConsole.Log("Dying here");
                dead = true;
                move = false;
                timer = 0.0f;
                Sound.PlaySFX(InjuredSound, 0.5f);
                Entity[] children = GetAllChildren();

                foreach(Entity entity in children)
                {
                    entity.GetComponent<Renderer>().SetAlpha(0);
                }


                //CMConsole.Log("TESTING Enter State");
                if (this.AsChild<HealthSystem>().mCurHealth <= 0)
                {
                    ChangeAnim(baseAnimationDie);
                }
                else
                {
                    ChangeAnim(baseAnimationDissolve);

                }
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }
            Entity gameManager = FindEntityWithName("GameManager");
          //  CMConsole.Log($"game manager gameOver :{gameManager.As<GameManager>().GameOver}");



            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Running")
            {
                if (gameManager.As<GameManager>().GameOver)
                {
                    //GetComponent<RigidBody>().StopForces();
                    //GetComponent<RigidBody>().StopObject();
                    
                    return;

                }
                move = true;
                // CMConsole.Log("TESTING Update State");
                if (enemyType != EnemyTypes.BEAR)
                {

                    if(!isRunning)
                    {
                        Random rnd = new Random();
                        int number = rnd.Next(1, 8);
                        soundFile = "Mice_Running_0" + number.ToString();
                        //CMConsole.Log(soundFile);
                        Sound.PlaySFX(soundFile, 0.2f, true, this.mID);

                        //Below the camera
                        

                        isRunning = true;
                    }

                    if (GetComponent<Transform>().Position.y < (mainCamera.Position.y - cameraHeight))
                    {
                        //CMConsole.Log("Below Camera");
                        
                        //float distance = Math.Abs(GetComponent<Transform>().Position.y - (mainCamera.Position.y - cameraHeight));
                        Sound.ToggleMuffleSFX(false, this.mID);
                        

                    }
                    else if (GetComponent<Transform>().Position.y > (mainCamera.Position.y - cameraHeight))
                    {
                        Sound.ToggleMuffleSFX(true, this.mID);
                    }

                }

                Vector2 endPos = Vector2.Zero;  //endEntityLeft.Position : endEntityRight.Position;
                //switch (randLane)
                switch(lane)
                {
                    case 0:
                        if (endEntityRight != null)
                            endPos = endEntityRight.Position;
                        //CMConsole.Log($"End Pos: {endPos.x} , {endPos.y}, Cur Pos: {Position.x}, {Position.y}, Distance: {Position.Distance(endPos)}");
                        break;
                    case 1:
                        if (endEntityLeft != null)
                            endPos = endEntityLeft.Position;
                        //CMConsole.Log($"End Pos: {endPos.x} , {endPos.y}, Cur Pos: {Position.x}, {Position.y}, Distance: {Position.Distance(endPos)}");
                        break;
                    case 2:
                        if (endEntityRight2 != null)
                            endPos = endEntityRight2.Position;
                        //CMConsole.Log($"End Pos: {endPos.x} , {endPos.y}, Cur Pos: {Position.x}, {Position.y}, Distance: {Position.Distance(endPos)}");
                        break;
                    case 3:
                        if (endEntityLeft2 != null)
                            endPos = endEntityLeft2.Position;
                        //CMConsole.Log($"End Pos: {endPos.x} , {endPos.y}, Cur Pos: {Position.x}, {Position.y}, Distance: {Position.Distance(endPos)}");
                        break;
                }


                float dist = Position.Distance(endPos);
                //CMConsole.Log($"Distance to end {dist}");

                if (dist <= 0.5f)
                {
                    // CMConsole.Log("Dying");

                   // Entity gameManager = FindEntityWithName("GameManager");

                    // if thers no NPC to kill then deal damage to the player
                    if (!gameManager.As<GameManager>().KillNPC(this))
                    {
                        Entity mainCharacter = FindEntityWithName("mainCharacter");
                        mainCharacter.As<Player>().TakeDamage(10, enemyType);

                    }

                    timer = 0.0f;
                    GetComponent<RigidBody>().StopObject();
                    Sound.StopSoundSFX(soundFile);
                    GetComponent<StateMachine>().SetStateCondition(1);


                }
                //UpdateMovement(dt);
            }
            else if (stateName == "Dead")
            {

                //timer += dt;
                
                if (GetComponent<Animation>().IsAnimFinished())
                {

                    isRunning = false;
                    Sound.StopSoundSFX(soundFile);
                    

                    Sound.PlaySFX(DeathSound, 0.5f);
                    Sound.PlaySFX("NPC_Death", 0.8f);
                    //timer = 0.0f;
                    
                    Destroy();
                }
            }
            //CMConsole.Log($"mouse retrieved : {targetMouse}");
        }

        public override void OnStateExit(string stateName)
        {
            //CMConsole.Log("TESTING Exit State");
            //CMConsole.Log($"Exit State Name: {stateName}");

        }

        public bool isDead()
        {
            return dead;
        }

        // this will be called by chance on by key_t for now to test, starts the lane switching of mice
        public void StartLaneSwitch()
        {
            int newLane = GetPairedLane(lane); // get the lane on the opposite side
            GameManager gm = FindEntityWithName("GameManager").As<GameManager>(); // get the game manager

            // remove from current lane list
            switch (lane)
            {
                case 0: 
                    gm.mouseLaneOne.RemoveAll(mouse => mouse.mID == this.mID); 
                    break;
                case 1: 
                    gm.mouseLaneTwo.RemoveAll(mouse => mouse.mID == this.mID); 
                    break;
                case 2: 
                    gm.mouseLaneThree.RemoveAll(mouse => mouse.mID == this.mID); 
                    break;
                case 3: 
                    gm.mouseLaneFour.RemoveAll(mouse => mouse.mID == this.mID); 
                    break;
            }

            // add to new lane list
            switch (newLane)
            {
                case 0: 
                    gm.mouseLaneOne.Add(this); 
                    break;
                case 1: 
                    gm.mouseLaneTwo.Add(this); 
                    break;
                case 2: 
                    gm.mouseLaneThree.Add(this); 
                    break;
                case 3: 
                    gm.mouseLaneFour.Add(this); 
                    break;
            }

            // get x-position of the new lane's start point
            Vector2 newLaneStartPos = GetLaneStartPos(newLane);
            switchTargetPos = new Vector2(newLaneStartPos.x, Position.y); // set the target x position

            CMConsole.Log($"[MouseAI] Mouse {mID} switched from lane {lane} to lane {newLane}"); // test

            lane = newLane; // update the lane
            isSwitching = true; // now do the x movement
        }

        // returns the lane on the opposite side of the current lane
        public int GetPairedLane(int currentLane)
        {
            // if even, add 1, if odd, subtract 1
            // this is assuming left lane is 0 and 2, right lane is 1 and 3
            //return currentLane % 2 == 0 ? currentLane + 1 : currentLane - 1;

            // 
            // right lane is 0 and 1, left lane is 2 and 3
            //// right right is 0, right left is 1, left right is 2, left left is 3
            //if(currentLane == 0 || currentLane == 1)
            //    return currentLane % 2 == 0 ? currentLane + 2 : currentLane - 2;
            //else
            //    return currentLane % 2 == 0 ? currentLane + 1 : currentLane - 1;
            //vbsivebvbubrub wtf do i bother with this complicated method ffs, just do a if else

            // right right is 0, left right is 1 or 2, left left is 3, right left is 2?
            if (currentLane == 0)
                return 1;
            else if (currentLane == 1)
                return 0;
            else if (currentLane == 2)
                return 3;
            else
                return 2;
        }

        // returns the start position of the lane
        private Vector2 GetLaneStartPos(int lane)
        {
            switch (lane)
            {
                case 0: 
                    return startPosRight;   // right right
                case 1: 
                    return startPosLeft;  // right left
                case 2: 
                    return startPosRight2;    // left right
                case 3: 
                    return startPosLeft2;   // left left
                default: 
                    return Position;
            }
        }

    }
}

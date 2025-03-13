using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    //ps
    //create flying enemy afrom the top left and right 
    //then the enemy will move 
    //then try to dive and kill the npc
    public enum FlyingStage
    {
        HORIZONTAL,  // Moving horizontally
        STATIONARY,  // Stationary for 3 seconds
        DIAGONAL     // 45 degree dive
    }

    public class FlyingEnemyAI : Entity
    {
        // Updated entity names for spawn and end points
        public string SpawnPointEntityLeft = "StartTopLeft";
        public string SpawnPointEntityRight = "StartTopRight";

        public string HorizontalAnim = "Pigeon_Fly";
        public string StationaryAnim = "Flying_Stationary";
        public string DiagonalAnim = "Pigeon_Dive";
        public string DeathAnim = "Pigeon_Death";

        public string DeathSound = "flying_die";
        public string InjuredSound = "flying_hurt";

        // set movement
        // 3 stage, constanrt, at rest, suicide bomb
        public float horizontalSpeed = 1.0f;
        public float diagonalSpeed = 1.8f;  // Increased speed when gg down 45 degrees

        public int lane;
        bool dead = false;
        public EnemyTypes enemyType;

        public FlyingStage currentStage = FlyingStage.HORIZONTAL;

        // Paths and positions
        Vector2 startPosition;
        Vector2 horizontalTarget;  // Target for horizontal 
        Vector2 diveTarget;        // Target diagonal 
        Vector2 targetPos;
        float debuff = 1.0f;
        public float speedDebuff = 0.4f;

        // Death properties
        public float timer = 0.0f;

        public bool isLeft = false;

        // Movement tracking - updated with new names
        Vector2 startPosLeft;
        Vector2 startPosRight;


        Entity targetEntity;

        int randLane = 0;
        bool move = false;

        public override void OnCreate()
        {
            if (FindEntityWithName(SpawnPointEntityLeft) != null)
                startPosLeft = FindEntityWithName(SpawnPointEntityLeft).Position;
            if (FindEntityWithName(SpawnPointEntityRight) != null)
                startPosRight = FindEntityWithName(SpawnPointEntityRight).Position;

            // InitWaypoints();
            //SetInitialPosition(); this is being done in game manager.cs

            //Sound.PlaySFX("Portal_Spawn", 0.3f);

            ChangeAnim(HorizontalAnim);

            //GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
            //if (gm != null)
            //{
            //    gm.activeEnemies++;
            //}
        }

        //this one idk is correct or not 
        public void SetInitialPosition()
        {
            Vector2 scale = Scale;

            // Random lane selection (similar to MouseAI)
            randLane = CMRand.Range(0, 2); // rand between 0 to 3
            lane = randLane;

            switch (randLane)
            {
                case 0:
                    Position = startPosLeft;
                    lane = 0;
                    startPosition = Position;
                    horizontalTarget = new Vector2(startPosLeft.x + 6.0f, startPosLeft.y);
                    //CMConsole.Log($"Target Position {horizontalTarget.x}, {horizontalTarget.y}");
                    scale.x *= -1;

                   // CMConsole.Log($"scale {scale.x}, {scale.y}");
                    Scale = scale;
                    //CMConsole.Log($"actual scale {Scale.x}, {Scale.y}");

                    isLeft = true;
                    break;
                case 1:
                    Position = startPosRight;
                    lane = 1;
                    startPosition = Position;
                    horizontalTarget = new Vector2(startPosRight.x - 6.0f, startPosRight.y);
                    break;
            }

            // Calculate the diagonal dive target (will be set when entering diagonal stage)
            //CalculateDiveTarget();
        }


        public override void OnUpdate(float dt)
        {

            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            if(move)
            {
                UpdateMovement(fixedDt, targetPos);
            }
        }

        void UpdateMovement(float dt, Vector2 targetPos)
        {
            Vector2 dir = (targetPos - Position).Normalize();
            if (HasComponent<RigidBody>())
            {
                if (currentStage == FlyingStage.DIAGONAL)
                {
                    GetComponent<RigidBody>().ApplyForce(dir, diagonalSpeed);

                }
                else
                {

                    GetComponent<RigidBody>().ApplyForce(dir, horizontalSpeed);
                }
            }

            float dist = Position.Distance(targetPos);
            if (dist <= 1.0f)
            {
                if (targetEntity != null && targetEntity.mID != 0)
                {
                    // the target is the player
                    if (targetEntity.GetTag() == "Player")
                    {
                        Entity mainCharacter = FindEntityWithName("mainCharacter");
                        mainCharacter.As<Player>().TakeDamage(10, enemyType);
                    }
                    else
                    {
                        // its probably an NPC
                        GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                        gm.KillNPC(targetEntity);
                    }
                    // change to dead state
                    GetComponent<StateMachine>().SetStateCondition(2);
                }
            }
        }

        public void UpdateTarget(Entity entity)
        {
            if (currentStage == FlyingStage.DIAGONAL)
            {
                if (targetEntity != null && entity == targetEntity)
                {
                    GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                    targetEntity = gm.GetTargetNPC(this);

                    // check if the new target is in the other direction
                    // if it is then rotate it
                    if (isLeft)
                    {
                        // is on the left of the bird
                        if (targetEntity.Position.x < Position.x)
                        {
                            // flip the scale
                            Vector2 scale = Scale;
                            scale.x *= -1;
                            // CMConsole.Log($"scale {scale.x}, {scale.y}");
                            Scale = scale;
                            // probably need adjust rotation or some shit later
                            Rot = 40.0f;
                        }
                    }
                    else
                    {
                        // is on the right of the bird
                        if (targetEntity.Position.x > Position.x)
                        {
                            // flip the scale
                            Vector2 scale = Scale;
                            scale.x *= -1;
                            // CMConsole.Log($"scale {scale.x}, {scale.y}");
                            Scale = scale;

                            // probably need adjust rotation or some shit later
                            Rot = -20.0f;
                        }
                    }

                    // idk if this is a good idea
                    // cause it looks a bit funky turning atm
                    ChangeAnim(HorizontalAnim);
                }
            }
        }

        public override void OnCollide(uint id)
        {
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

                    //this.AsChild<HealthSystem>().TakeDamage(100);
                }
                else if (collidedEntity.GetTag() == "Bullet")
                {
                     this.AsChild<HealthSystem>().TakeDamage(50);
                    //GetComponent<StateMachine>().SetStateCondition(2);
                }
            }

            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                GetComponent<StateMachine>().SetStateCondition(2);
            }
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
                }
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

                    //CMConsole.Log("FLYING ENEMY SLOWED BY TRAP");
                    debuff = speedDebuff;
                }
            }
        }

        public override void OnTriggerExit()
        {
            debuff = 1.0f;
        }

        public void KillEnemy()
        {
            timer = 0.0f;
            GetComponent<StateMachine>().SetStateCondition(1);
        }

        public override void OnStateEnter(string stateName)
        {
            //CMConsole.Log($"Entering State {stateName}");

            if (stateName == "Diving")
            {
                // get diving target
                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                targetEntity = gm.GetTargetNPC(this);
                currentStage = FlyingStage.DIAGONAL;
                ChangeAnim(DiagonalAnim);
                 
                if(isLeft)
                { 
                    Rot = -60.0f; 
                }
                else
                {
                    Rot = 60.0f;
                }
            }

            if (stateName == "Dead")
            {
                dead = true;
                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                if (gm != null)
                    gm.EntityDestroyed(this);
                timer = 0.0f;

                GetComponent<RigidBody>().StopForces();
                GetComponent<RigidBody>().StopObject();
                //Sound.PlaySFX(InjuredSound, 0.5f);

                //GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                //if (gm != null)
                //{
                //    gm.activeEnemies--;
                //}

                ChangeAnim(DeathAnim);
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            Entity gameManager = FindEntityWithName("GameManager");
            //  CMConsole.Log($"game manager gameOver :{gameManager.As<GameManager>().GameOver}");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            if  (stateName == "Flying")
            {
                if (gameManager.As<GameManager>().GameOver)
                {
                    GetComponent<RigidBody>().StopForces();
                    return;
                }
                move = true;

                targetPos = horizontalTarget;

                // reaching diving point
                if (Position.Distance(horizontalTarget) < 0.5f)
                {
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }
            else if (stateName == "Diving")
            {
                if (gameManager.As<GameManager>().GameOver)
                {
                    GetComponent<RigidBody>().StopForces();
                    return;
                }
                if (targetEntity == null) return;

                //UpdateMovement(dt, targetEntity.Position);

                targetPos = targetEntity.Position;
            }
            else if (stateName == "Dead")
            {
                timer += dt;
                if (GetComponent<Animation>().IsAnimFinished())
                {
                    // Sound.PlaySFX(DeathSound, 0.5f);
                    Destroy();
                }
                //if (timer >= GetComponent<Animation>().GetMaxTime())
                //{
                //    Sound.PlaySFX(DeathSound, 0.5f);
                //    timer = 0.0f;
                //    Destroy();
                //}
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
    }
}
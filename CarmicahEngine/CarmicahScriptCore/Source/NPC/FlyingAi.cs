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
        public string SpawnPointEntityLeft;
        public string SpawnPointEntityRight;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;
        public string SpawnPointEntityLeft2;
        public string SpawnPointEntityRight2;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;

        public string HorizontalAnim = "Flying_Horizontal";
        public string StationaryAnim = "Flying_Stationary";
        public string DiagonalAnim = "Flying_Diagonal";
        public string DeathAnim = "Flying_Death";

        public string DeathSound = "flying_die";
        public string InjuredSound = "flying_hurt";

        // set movement
        // 3 stage, constanrt, at rest, suicide bomb
        public float horizontalSpeed = 1.0f;
        public float diagonalSpeed = 1.8f;  // Increased speed when gg down 45 degrees
        public float stationaryTime = 3.0f;  // Time to stay station
        public float stationaryTimer = 0.0f;  

        public int lane;
        public EnemyTypes enemyType;
        public float ChanceToDie = 0.12f;


        public FlyingStage currentStage = FlyingStage.HORIZONTAL;

        // Paths and positions
        Vector2 startPosition;
        Vector2 horizontalTarget;  // Target for horizontal 
        Vector2 diveTarget;        // Target diagonal 
        float debuff = 1.0f;
        public float speedDebuff = 0.4f;

        // Death properties
        public float timer = 0.0f;
        public bool isDead = false;

        // Movement tracking
        Vector2 startPosLeft;
        Vector2 startPosRight;
        Vector2 startPosLeft2;
        Vector2 startPosRight2;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;

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

            SetInitialPosition();

            Sound.PlaySFX("Portal_Spawn", 0.3f);

            ChangeAnim(HorizontalAnim);

            GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
            if (gm != null)
            {
                gm.activeEnemies++; 
            }
        }

        //this one idk is correct or not 
        public void SetInitialPosition()
        {
            Vector2 scale = Scale;

            // Random lane selection (similar to MouseAI)
            randLane = CMRand.Range(0, 3); // rand between 0 to 3
            lane = randLane;

            switch (randLane)
            {
                case 0:
                    Position = startPosLeft;
                    lane = 1;
                    startPosition = Position;
                    horizontalTarget = new Vector2(Position.x + 6.0f, Position.y);
                    break;
                case 1:
                    Position = startPosLeft2;
                    lane = 3;
                    startPosition = Position;
                    horizontalTarget = new Vector2(Position.x + 6.0f, Position.y);
                    scale.x *= -1;
                    Scale = scale;
                    break;
                case 2:
                    Position = startPosRight;
                    lane = 0;
                    startPosition = Position;
                    horizontalTarget = new Vector2(Position.x - 6.0f, Position.y);
                    break;
                case 3:
                    Position = startPosRight2;
                    lane = 2;
                    startPosition = Position;
                    horizontalTarget = new Vector2(Position.x - 6.0f, Position.y);
                    scale.x *= -1;
                    Scale = scale;
                    break;
            }

            // Calculate the diagonal dive target (will be set when entering diagonal stage)
            CalculateDiveTarget();
        }

        void CalculateDiveTarget()
        {
            // Calculate the 45-degree downward vector
            Vector2 endPos = Vector2.Zero;

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

            // Create a diagonal vector (45 degrees downward)
            Vector2 direction = (endPos - horizontalTarget).Normalize();
            diveTarget = endPos;
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

        void UpdateMovement(float dt)
        {
            switch (currentStage)
            {
                case FlyingStage.HORIZONTAL:

                    Vector2 horizontalDir = (horizontalTarget - Position).Normalize();
                    if (HasComponent<RigidBody>())
                    {
                        GetComponent<RigidBody>().ApplyForce(horizontalDir, horizontalSpeed * debuff);
                    }

                    Vector2 scale = Scale;
                    if (horizontalDir.x < 0 && scale.x > 0)
                    {
                        scale.x = -Math.Abs(scale.x);
                        Scale = scale;
                    }
                    else if (horizontalDir.x > 0 && scale.x < 0)
                    {
                        scale.x = Math.Abs(scale.x);
                        Scale = scale;
                    }

                    float distToTarget = Position.Distance(horizontalTarget);
                    if (distToTarget <= 0.3f)
                    {
                        currentStage = FlyingStage.STATIONARY;
                        stationaryTimer = 0.0f;

                        ChangeAnim(StationaryAnim);

                        if (HasComponent<RigidBody>())
                        {
                            GetComponent<RigidBody>().StopForces();
                        }
                    }
                    break;

                case FlyingStage.STATIONARY:
                    stationaryTimer += dt;
                    if (stationaryTimer >= stationaryTime)
                    {
                        currentStage = FlyingStage.DIAGONAL;

                        ChangeAnim(DiagonalAnim);
                    }
                    break;

                case FlyingStage.DIAGONAL:
                    Vector2 diagonalDir = (diveTarget - Position).Normalize();
                    //45 degree
                    if (HasComponent<RigidBody>())
                    {
                        GetComponent<RigidBody>().ApplyForce(diagonalDir, diagonalSpeed * debuff);
                    }

                    //check end pt
                    float distToEnd = Position.Distance(diveTarget);
                    if (distToEnd <= 0.3f)
                    {
                        Entity mainCharacter = FindEntityWithName("mainCharacter");
                        mainCharacter.As<Player>().TakeDamage(10, enemyType);

                        timer = 0.0f;
                        GetComponent<StateMachine>().SetStateCondition(1);
                    }
                    break;
            }
        }

        void OnCollide(uint id)
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

                    this.AsChild<HealthSystem>().TakeDamage(100);
                }
                else if (collidedEntity.GetTag() == "Bullet")
                {
                    this.AsChild<HealthSystem>().TakeDamage(50);
                }
            }

            if (this.AsChild<HealthSystem>().mCurHealth <= 0)
            {
                GetComponent<StateMachine>().SetStateCondition(1);
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

                    CMConsole.Log("FLYING ENEMY SLOWED BY TRAP");
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

        public void OnStateEnter(string stateName)
        {
            if (stateName == "Dead")
            {
                timer = 0.0f;
                Sound.PlaySFX(InjuredSound, 0.5f);

                GameManager gm = FindEntityWithName("GameManager").As<GameManager>();
                if (gm != null)
                {
                    gm.activeEnemies--; 
                }

                ChangeAnim(DeathAnim);
            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            //  CMConsole.Log($"game manager gameOver :{gameManager.As<GameManager>().GameOver}");

            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            Entity gameManager = FindEntityWithName("GameManager");

            // CMConsole.Log($"Update State Name: {stateName}");
            if (stateName == "Running")
            {
                if (gameManager.As<GameManager>().GameOver)
                {
                    GetComponent<RigidBody>().StopForces();
                    return;
                }

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
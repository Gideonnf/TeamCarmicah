using Carmicah;
using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class GameManager : Entity
    {
        public float spawnTimer = 0.5f;
        public string MousePrefabName = "MouseGO";
        public string CakePrefabName = "Cake";
        public string PlayerName = "mainCharacter";
        public float timer = 0.0f;
        public bool LeftOrRight = false;
        public float CakeHeightOffset;
        public string StartingCake;
        public List<Entity> mouseEntities = new List<Entity>(); // Doing this doesn't work idk why
        public float WaveStartTime = 25.0f;
        public float waveTimer = 0.0f;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;
        public string ShooterNPC = "ShooterNPC";
        public string ShooterNPC2 = "ShooterNPC_1";

        public bool GameStart = false;
        public float MobCounter = 0;
        public float NumOfMobs = 10;
        

        Entity startingCakeEntity;
        Entity playerEntity;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;
        Entity shooterNPC;
        Entity shooterNPC2;

        Entity wall;
        Entity wall1;
        Entity wall2;
        Entity wall3;
        int cakeCounter = 1;
        int waveCount = 0;
        void OnCreate()
        {
            endEntityLeft = FindEntityWithName(EndPointEntityLeft);
            endEntityRight = FindEntityWithName(EndPointEntityRight);
            endEntityLeft2 = FindEntityWithName(EndPointEntityLeft2);
            endEntityRight2 = FindEntityWithName(EndPointEntityRight2);

            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
            shooterNPC = FindEntityWithName(ShooterNPC);
            shooterNPC2 = FindEntityWithName(ShooterNPC2);

            wall = FindEntityWithName("Wall");
            wall1 = FindEntityWithName("Wall_1");
            wall2 = FindEntityWithName("Wall_2");
            wall3 = FindEntityWithName("Wall_3");
        }

        void OnUpdate(float dt)
        {
            waveTimer += dt;

            if (MobCounter > 0)
            {
                timer += dt;

                if (timer > spawnTimer)
                {
                    timer = 0.0f;
                    Entity mouseEntity = CreateGameObject(MousePrefabName);
                    MouseAI mouseAI = mouseEntity.As<MouseAI>();
                    mouseAI.isLeft = LeftOrRight;
                    LeftOrRight = !LeftOrRight;
                    mouseAI.SetInitialPosition(); // Reset initial position
                    MobCounter--;
                    //Console.WriteLine($"Mouse Added {mouseEntity}");

                   // mouseEntities.Add(mouseEntity);
                }
            }
            else
            {
                EndWave();
            }


            if (waveTimer > WaveStartTime)
            {
                waveTimer = 0.0f;
                timer = spawnTimer;
                StartNextWave();
            }

            if (Input.IsKeyPressed(Keys.KEY_1))
            {
                StartNextWave();
                waveTimer = 0;
            }

            if (Input.IsKeyPressed(Keys.KEY_SPACEBAR))
            {
                if (cakeCounter < 3)
                {
                    Entity cakeEntity = CreateGameObject(CakePrefabName);
                    // Set the cake position
                    cakeEntity.Position = startingCakeEntity.Position;
                    cakeEntity.Scale = startingCakeEntity.Scale;
                    cakeEntity.Depth = startingCakeEntity.Depth;

                    cakeEntity.Depth = cakeEntity.Depth + (0.1f * cakeCounter);
                    Vector2 pos = cakeEntity.Position;
                    pos.y += (CakeHeightOffset * cakeCounter);
                    cakeEntity.Position = pos;
                    cakeCounter++;

                    if (playerEntity != null)
                    {
                        pos = playerEntity.Position;
                        pos.y += CakeHeightOffset;
                        playerEntity.Position = pos;
                    }

                    if (endEntityLeft != null)
                    {
                        pos = endEntityLeft.Position;
                        pos.y += CakeHeightOffset;
                        endEntityLeft.Position = pos;
                    }

                    if (endEntityLeft2 != null)
                    {
                        pos = endEntityLeft2.Position;
                        pos.y += CakeHeightOffset;
                        endEntityLeft2.Position = pos;
                    }

                    if (endEntityRight != null)
                    {
                        pos = endEntityRight.Position;
                        pos.y += CakeHeightOffset;
                        endEntityRight.Position = pos;
                    }

                    if (endEntityRight2 != null)
                    {
                        pos = endEntityRight2.Position;
                        pos.y += CakeHeightOffset;
                        endEntityRight2.Position = pos;
                    }

                    if (shooterNPC != null)
                    {
                        pos = shooterNPC.Position;
                        pos.y += CakeHeightOffset;
                        shooterNPC.Position = pos;
                    }

                    if (shooterNPC2 != null)
                    {
                        pos = shooterNPC2.Position;
                        pos.y += CakeHeightOffset;
                        shooterNPC2.Position = pos;
                    }

                    if (wall != null)
                    {
                        pos = wall.Position;
                        pos.y += CakeHeightOffset;
                        wall.Position = pos;
                    }
                    if (wall1 != null)
                    {
                        pos = wall1.Position;
                        pos.y += CakeHeightOffset;
                        wall1.Position = pos;
                    }
                    if (wall2 != null)
                    {
                        pos = wall2.Position;
                        pos.y += CakeHeightOffset;
                        wall2.Position = pos;
                    }
                    if (wall3 != null)
                    {
                        pos = wall3.Position;
                        pos.y += CakeHeightOffset;
                        wall3.Position = pos;
                    }
                }
            }
        }

        public void StartNextWave()
        {
            GameStart = true;
            MobCounter += NumOfMobs;
            waveCount++;
            if (shooterNPC != null)
            {
                ShooterNPC npc = shooterNPC.As<ShooterNPC>();
                npc.ToggleShooting();

            }

            if (shooterNPC2 != null)
            {
                ShooterNPC npc = shooterNPC2.As<ShooterNPC>();
                npc.ToggleShooting();
            }

            GameStart = true;
        }

        public void EndWave()
        {
            if (GameStart)
            {
                GameStart = false;
                if (shooterNPC != null)
                {
                    ShooterNPC npc = shooterNPC.As<ShooterNPC>();
                    npc.ToggleIdle();

                }

                if (shooterNPC2 != null)
                {
                    ShooterNPC npc = shooterNPC2.As<ShooterNPC>();
                    npc.ToggleIdle();
                }
            }
        }

        public void MouseDestroyed(Entity mouse)
        {
           //
           // ($"Mouse Destroyed {mouse}");
            //mouseEntities.Remove(mouse);
        }

        //public Entity GetFirstMouse()
        //{
        //    return mouseEntities.First();
        //}

    }
}
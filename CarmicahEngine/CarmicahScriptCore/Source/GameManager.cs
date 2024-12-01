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

        public bool GameStart = false;

        Entity startingCakeEntity;
        Entity playerEntity;
        int cakeCounter = 1;
        int waveCount = 0;
        void OnCreate()
        {
            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
        }

        void OnUpdate(float dt)
        {
            waveTimer += dt;

            if (GameStart)
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

                    Console.WriteLine($"Mouse Added {mouseEntity}");

                    mouseEntities.Add(mouseEntity);
                }
            }


            if (waveTimer > WaveStartTime)
            {
                waveTimer = 0.0f;
                timer = spawnTimer;
                GameStart = !GameStart;
                if (GameStart)
                {
                    waveCount++;
                    Entity shooter = FindEntityWithName("ShooterNPC");
                     ShooterNPC npc = shooter.As<ShooterNPC>();
                    npc.ToggleShooting();
                }
                else
                {
                    Entity shooter = FindEntityWithName("ShooterNPC");
                    ShooterNPC npc = shooter.As<ShooterNPC>();
                    npc.ToggleIdle();

                }
            }

            // TODO: When I wake up
            // Make a function in c++ side to find game object with name that searches for anything with the substring
            // if that doesnt work then i just make a timer and a random in the mouse AI to kill itself lmao
            // idk why this doesnt work
            //if (shootTimer > shootTime)
            //{
            //    Entity targetMouse = mouseEntities[0];
            //    Console.WriteLine("Testing");
            //    if (targetMouse != null)
            //    {
            //        Console.WriteLine("KILLING A MOUSE");
            //        MouseAI mouseAI = targetMouse.As<MouseAI>();
            //        mouseAI.KillMouse();
            //        shootTimer = 0.0f;
            //    }
            //}
            if (Input.IsKeyPressed(Keys.KEY_1))
            {
                waveTimer = WaveStartTime;
            }

            if (Input.IsKeyPressed(Keys.KEY_SPACEBAR))
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

                pos = playerEntity.Position;
                pos.y += CakeHeightOffset;
                playerEntity.Position = pos;
                cakeCounter++;
            }
        }

        public void MouseDestroyed(Entity mouse)
        {
            Console.WriteLine($"Mouse Destroyed {mouse}");
            mouseEntities.Remove(mouse);
        }

        //public Entity GetFirstMouse()
        //{
        //    return mouseEntities.First();
        //}

    }
}
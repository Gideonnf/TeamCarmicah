using Carmicah;
using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
        Entity startingCakeEntity;
        Entity playerEntity;
        int cakeCounter = 1;
        void OnCreate()
        {
            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
        }

        void OnUpdate(float dt)
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

    }
}
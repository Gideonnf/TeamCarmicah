using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class WaveSystem : Entity
    {
        public float waveStartTime = 20.0f;
        public float waveTimer = 0.0f;
        public bool startNewWave = true;
        
        public int mobWave0 = 5;
        public int mobWave1 = 10;
        public int mobWave2 = 15;
        public int mobWave3 = 20;
        public int mobWave4 = 25;

        //public int[] mobWaves = { 5, 10, 15, 20, 25 };
        int[] mobWaves = new int[5];
        public int mobCounter = 0;
        public int waveCounter = 0;

        Entity gameManager;

        void OnCreate()
        {
            gameManager = FindEntityWithName("GameManager");
            mobWaves[0] = mobWave0;
            mobWaves[1] = mobWave1;
            mobWaves[2] = mobWave2;
            mobWaves[3] = mobWave3;
            mobWaves[4] = mobWave4;
        }

        void OnUpdate(float dt)
        {
            waveTimer += dt;

            if (waveCounter >= 5)
            {
                //CreateGameObject("WinScreen");
                
                waveCounter--;
            }

            if (waveTimer > waveStartTime && startNewWave)
            {
                // start next wave
                gameManager.As<GameManager>().StartNextWave(mobWaves[waveCounter]);
                CMConsole.Log("Starting New Wave");
                
                waveCounter++;
                waveTimer = 0.0f;
                startNewWave = false;
            }

            if (Input.IsKeyPressed(Keys.KEY_1))
            {
                // start next wave
                gameManager.As<GameManager>().StartNextWave(mobWaves[waveCounter]);

                waveCounter++;
                waveTimer = 0.0f;
            }
        }

        public void EndOfWave()
        {
            waveTimer = 0.0f;
            startNewWave = true;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class WaveSystem : Entity
    {
        public float waveSetupTime = 20.0f;
        public float waveStartTime = 20.0f;
        public float waveTimer = 0.0f;

        public bool waveStart = false;

        public LevelManager levelManager;

        //public int mobCounter = 0;
        //public int waveCounter = 0;

        Entity gameManager;

        void OnCreate()
        {
            levelManager = new LevelManager();
            gameManager = FindEntityWithName("GameManager");
            Player.GameLost = false;

        }

        void OnUpdate(float dt)
        {
            if (!Player.GameLost )
            {
                // Only increment time for as long as theres a wave coming
                waveTimer += dt;
                // Initial wave start
                if (waveTimer > waveSetupTime && waveStart == false)
                {
                    waveStart = true;
                    //Level level = levelManager.GetLevel();
                    Wave nextWave = levelManager.GetWave();
                    //nextWave.PrintWaveData();
                    if (nextWave != null)
                        gameManager.As<GameManager>().StartNextWave(nextWave);
                    //CMConsole.Log("Starting New Wave");

                   // waveCounter++;
                    waveTimer = 0.0f;
                    Sound.StopSoundBGM("BGM_SetupPhase_Mix1");
                    Sound.PlayBGM("BGM_LevelMusic_FullTrack_Vers1", 0.4f);
                }

                // Subsequent wave starts or skip the waves
                if (waveTimer > waveStartTime || Input.IsKeyPressed(Keys.KEY_2))
                {
                    CMConsole.Log("Starting next wave!");
                    // start next wave
                    Wave nextWave = levelManager.GetWave();
                    if (nextWave != null)
                        gameManager.As<GameManager>().StartNextWave(nextWave);
                    //CMConsole.Log("Starting New Wave");

                    //waveCounter++;
                    waveTimer = 0.0f;
                    // waveStart = true;
                }

                // end of level only when all enemies are dead
                if (levelManager.EndOfLevel() && gameManager.As<GameManager>().activeEnemies == 0)
                {
                    //CreateGameObject("WinScreen");

                    // NOTE FOR NOW COMMENTING OUT SO I CAN DO WIN SCREEN HERE
                    // THEN WIN SCREEN WILL TRANSITION TO NEXT LEVEL DEPENDING
                    //gameManager.As<GameManager>().GetComponent<StateMachine>().SetStateCondition(2);

                    //waveCounter = 0;
                    waveTimer = 0.0f;
                    //startNewWave = true;
                    // waveCounter--;
                }



                // if (Input.IsKeyPressed(Keys.KEY_1))
                // {
                //     // start next wave
                //     gameManager.As<GameManager>().StartNextWave(mobWaves[waveCounter], bearWaves[waveCounter]);

                //     waveCounter++;
                //     waveTimer = 0.0f;
                // }
                //if (Input.IsKeyPressed(Keys.KEY_2))
                //{
                //    //waveTimer = 0.0f;
                //   // startNewWave = true;

                //}

                // Skip to end of wave
                //if (Input.IsKeyPressed(Keys.KEY_1))
                //{
                //    // start next wave
                //    gameManager.As<GameManager>().StartNextWave(mobWaves[waveCounter], bearWaves[waveCounter]);

                //    waveCounter = 5;
                //    waveTimer = 0.0f;
                //    waveStart = true;
                //}
            }

        }
        public void EndOfWave()
        {
            waveTimer = 0.0f;
            waveStart = false;
        }
    }
}

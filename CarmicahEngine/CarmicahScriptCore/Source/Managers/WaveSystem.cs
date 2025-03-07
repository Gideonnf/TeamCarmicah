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
        public float countdownStartTime = 4.0f;

        public bool waveStart = false;

        public LevelManager levelManager;

        //public int mobCounter = 0;
        //public int waveCounter = 0;

        public string winPrefab = "Win_Screen";
        public string losePrefab = "LoseScreen";
        public string countdownPrefab = "Countdown_1";

        Entity gameManager;
        Entity winScreen;
        Entity countdown;



        void OnCreate()
        {
            levelManager = new LevelManager();
            gameManager = FindEntityWithName("GameManager");
            Player.GameLost = false;

        }

        void OnUpdate(float dt)
        {
            // Only increment time for as long as theres a wave coming
            if (!gameManager.As<GameManager>().GameOver)
            {
                waveTimer += dt;

            }

            if (waveStart == false && (waveSetupTime - waveTimer) <= countdownStartTime)
            {
                if (countdown == null)
                {
                    countdown = CreateGameObject(countdownPrefab);
                }
            }
            // Initial wave start
            //CMConsole.Log("Test");

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
                Sound.StopSoundBGM(this.mID,"BGM_SetupPhase_Mix1");
                Sound.PlayBGM(this.mID, "BGM_LevelMusic_FullTrack_Vers1", 0.4f);

                if (countdown != null)
                {
                    countdown.Destroy();

                }

            }
            // CMConsole.Log("Test1");

            // Subsequent wave starts or skip the waves
            if (waveTimer > waveStartTime || Input.IsKeyPressed(Keys.KEY_2))
            {
                Wave nextWave = levelManager.GetWave();
                if (nextWave != null)
                    gameManager.As<GameManager>().StartNextWave(nextWave);

                waveTimer = 0.0f;
            }
            //CMConsole.Log($"{gameManager.As<GameManager>().activeEnemies} and {levelManager.EndOfLevel()}");

            // end of level only when all enemies are dead
            if (levelManager.EndOfLevel() && gameManager.As<GameManager>().activeEnemies == 0)
            {
                // NOTE FOR NOW COMMENTING OUT SO I CAN DO WIN SCREEN HERE
                // THEN WIN SCREEN WILL TRANSITION TO NEXT LEVEL DEPENDING
                //gameManager.As<GameManager>().GetComponent<StateMachine>().SetStateCondition(2);
                if (winScreen == null)
                {
                    gameManager.As<GameManager>().GameOver = true;
                    winScreen = CreateGameObject(winPrefab);
                }
                //CreateGameObject(winPrefab);
                waveStart = false;
                //waveCounter = 0;
                waveTimer = 0.0f;
                //startNewWave = true;
                // waveCounter--;
            }


           //CMConsole.Log("PLEASE GO TO A NEW LEVEL PLS PLS PLS");

          //  if (winScreen != null && winScreen.mID == 0)
          //      winScreen = null;
          ////  CMConsole.Log("ASDASASDASDA");
          //  //CMConsole.Log($"{gameManager.As<GameManager>().GameOver} and {winScreen == null}");
          //  if (winScreen != null)
          //      // CMConsole.Log($"{winScreen.mID}");
          //      // win screen was deleted
          //      if (gameManager.As<GameManager>().GameOver && winScreen == null)
          //      {
          //          CMConsole.Log("PLEASE GO TO A NEW LEVEL PLS PLS PLS");
          //      }
        }

        public void EndOfLevel()
        {
            gameManager.As<GameManager>().GameOver = false;
            gameManager.As<GameManager>().GetComponent<StateMachine>().SetStateCondition(2);
            levelManager.NextLevel();


            Entity powerController = FindEntityWithName("PowerUpControl");
            powerController.As<PowerUpControl>().WipePowerups();
        }

        //public void EndOfWave()
        //{
        //    waveTimer = 0.0f;
        //    waveStart = false;
        //}
    }
}

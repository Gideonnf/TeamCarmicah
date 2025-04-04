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

        //public bool EndGame;

        Entity gameManager;
        Entity pauseManager;
        Entity UIManager;

        Entity winScreen;
        Entity countdown;
        bool winScreenCreated = false;



        public override void OnCreate()
        {
            levelManager = new LevelManager();
            gameManager = FindEntityWithName("GameManager");
            pauseManager = FindEntityWithName("PauseManager");
            UIManager = FindEntityWithName("UIManager");

            Player.GameLost = false;
            
        }

        public override void OnUpdate(float dt)
        {
            if (pauseManager.As<PauseManager>().IsPaused)
            {
                return;

            }

            // Only increment time for as long as theres a wave coming
            if (gameManager.As<GameManager>().GameOver)
            {
                return;
            }

            waveTimer += dt;
            if (waveStart == false && (waveSetupTime - waveTimer) <= countdownStartTime)
            {
                if (countdown == null)
                {
                    countdown = CreateGameObject(countdownPrefab);
                    Sound.SwitchBGM("BGM_LevelMusic_FullTrack_Vers1", 1.5f, 1.0f, false, true);
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
                {
                    //nextWave.ShuffleEnemies();
                    gameManager.As<GameManager>().StartNextWave(nextWave);
                }
                //CMConsole.Log("Starting New Wave");
                //Sound.StopSoundBGM("BGM_SetupPhase_Mix1");
                
                // waveCounter++;
                waveTimer = 0.0f;
                

                if (countdown != null)
                {
                    countdown.Destroy();
                    countdown = null;

                }

            }
            // CMConsole.Log("Test1");

            // Subsequent wave starts or skip the waves
            if (waveTimer > waveStartTime || Input.IsKeyPressed(Keys.KEY_2))
            {
                Wave nextWave = levelManager.GetWave();
                if (nextWave != null)
                {
                    //nextWave.ShuffleEnemies();
                    gameManager.As<GameManager>().StartNextWave(nextWave);
                }

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
                    Sound.SwitchBGM("WinScreen", 0.5f, 0.5f, false);
                    Sound.StopAllSFX();
                    gameManager.As<GameManager>().GameOver = true;
                    if (levelManager.EndOfGame())
                    {
                        // CMConsole.Log("Does this runNnnnnnnnnnnnnn");
                        winScreen = CreateGameObject("GameEnd_Screen");

                    }
                    else
                    {
                        //  CMConsole.Log("Does this run");
                        winScreen = CreateGameObject(winPrefab);
                        winScreenCreated = true;
                    }
                }
                //CreateGameObject(winPrefab);
                waveStart = false;
                //waveCounter = 0;
                waveTimer = 0.0f;
                //startNewWave = true;
                // waveCounter--;
            }

            if (winScreenCreated)
            {
                List<bool> listEnemiesActive = new List<bool>();
                listEnemiesActive = levelManager.PreviewNextLevelEnemies();
                int count = 0;
                for (int i = 0; i < listEnemiesActive.Count; i++)
                {
                    if (listEnemiesActive[i])
                        count++;
                }
                List<Entity> listEnemies = new List<Entity>();
                Entity wMouse0 = FindEntityWithName("NWMouse0");
                if (wMouse0 != null)
                    listEnemies.Add(wMouse0);
                Entity wMouse1 = FindEntityWithName("NWMouse1");
                if (wMouse1 != null)
                    listEnemies.Add(wMouse1);
                Entity wMouse2 = FindEntityWithName("NWMouse2");
                if (wMouse2 != null)
                    listEnemies.Add(wMouse2);
                Entity wBear = FindEntityWithName("NWBear");
                if (wBear != null)
                    listEnemies.Add(wBear);
                Entity wBird = FindEntityWithName("NWBird");
                if (wBird != null)
                    listEnemies.Add(wBird);

                CMConsole.Log($"A:{listEnemies.Count} B:{listEnemiesActive.Count}");

                if (listEnemies.Count == listEnemiesActive.Count)
                {
                    Vector2 pos = new Vector2(-365.0f, -80.0f);
                    if (count < 5)
                        pos.x += 460.0f / ((float)Math.Pow(2,count));
                    for (int i = 0; i < listEnemiesActive.Count; i++)
                    {
                        if (listEnemiesActive[i])
                        {
                            listEnemies[i].GetComponent<Renderer>().SetAlpha(1.0f);
                            listEnemies[i].LocalPosition = pos;
                            // -365 ~ 95 == 460
                            pos.x += 460.0f / Math.Min(count, 4.0f);
                        }
                    }
                    winScreenCreated = false;
                }
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
            winScreen = null;
        }

        //public void EndOfWave()
        //{
        //    waveTimer = 0.0f;
        //    waveStart = false;
        //}
    }
}

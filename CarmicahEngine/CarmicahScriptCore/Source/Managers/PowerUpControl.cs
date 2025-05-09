﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PowerUpControl : Entity
    {
        public string WaveSystemObject = "Something";
        public string TrapPrefab = "TrapBall";
        public string ShooterPrefab = "ShooterBall";
        public string MagePrefab = "MageBall";
        public string HoneyPrefab = "HoneyBall";
        public string SpearPrefab = "SpearBall";
        public string JellybeanPrefab = "JellybeanBall";

        public float xOffset = 2.0f;
        public float heightOffset = 12.0f;

        public float SpawnTimer = 5.0f;
        public float timer = 0.0f;
        Entity camera;
        Entity mainCharacter;
        Entity gameManager;
        Entity pauseManager;
        Entity waveSystem;

        List<TrapBall> listOfPowerups = new List<TrapBall>();

        //public Dictionary<IconType, float> weights = new Dictionary<IconType, float>();

        List<float> trapWeights = new List<float>()
        {
            0.3f, // Candy
            0.05f, // Shoot
            0.25f, // Honey
            0.05f, // Mage
            0.3f, // Explosion
            0.05f  // Spear
        };

        public override void OnCreate()
        {
            /*
             *   {
                        {IconType.CANDY_ICON, 0.4f },
                        {IconType.SHOOTER_ICON, 0.1f },
                        {IconType.HONEY_ICON, 0.4f },
                        {IconType.MAGE_ICON, 0.1f }
                    };
             */
            mainCharacter = FindEntityWithName("mainCharacter");
            camera = FindEntityWithName("MainCamera");
            gameManager = FindEntityWithName("GameManager");
            pauseManager = FindEntityWithName("PauseManager");
            waveSystem = FindEntityWithName(WaveSystemObject);
            //weights.Add(IconType.CANDY_ICON, 0.4f);
            //weights.Add(IconType.SHOOTER_ICON, 0.1f);
            //weights.Add(IconType.HONEY_ICON, 0.4f);
            //weights.Add(IconType.MAGE_ICON, 0.1f);

        }

        public IconType GetRandomTrap()
        {
            // CMConsole.Log("hellodsdsds");
            float totalWeight = 0.0f;
            foreach (var val in trapWeights)
            {
                totalWeight += val;
            }
            // CMConsole.Log($"Total weight sum {totalWeight}");
            float random = CMRand.Range(0.0f, totalWeight);
            //CMConsole.Log("hello 2");

            float cumulative = 0.0f;

            for (int i = 0; i < trapWeights.Count; i++)
            {
                cumulative += trapWeights[i];
                //CMConsole.Log($"Cumulative : {cumulative} vs {random}");
                if (random <= cumulative)
                {
                    //  CMConsole.Log("does this work");
                    return (IconType)i;
                }
            }

            //CMConsole.Log("does this work");
            return IconType.CANDY_ICON;
        }

        public void PowerUpDestroyed(TrapBall entity)
        {
            listOfPowerups.Remove(entity);
         //   CMConsole.Log($"List of power ups: {listOfPowerups.Count}");
        }

        // to use when going next level
        public void WipePowerups()
        {
            foreach(TrapBall trap in listOfPowerups)
            {
                trap.Destroy();
            }
        }

        public override void OnUpdate(float dt)
        {
            if (pauseManager.As<PauseManager>().IsPaused)
                return;
            if (gameManager.As<GameManager>().GameOver)
                return;
            if (!waveSystem.As<WaveSystem>().waveStart)
                return;

            timer += dt;
           // CMConsole.Log($"{timer}");
            if (timer >= SpawnTimer)
            {
                ///CMConsole.Log("sdasiodioasndiosan");
                timer = 0.0f;
                IconType iconType = GetRandomTrap();
                // Create a prefab
                Entity newTrap = null;
               // CMConsole.Log("does this work 2");
                switch (iconType)
                {
                    case IconType.CANDY_ICON:
                        {
                            newTrap = CreateGameObject(TrapPrefab);
                            break;
                        }
                    case IconType.SHOOTER_ICON:
                        {
                            newTrap = CreateGameObject(ShooterPrefab);

                            break;
                        }
                    case IconType.HONEY_ICON:
                        {
                            newTrap = CreateGameObject(HoneyPrefab);

                            break;
                        }
                    case IconType.MAGE_ICON:
                        {
                            newTrap = CreateGameObject(MagePrefab);

                            break;
                        }
                    case IconType.JELLYBEAN_ICON:
                        {
                            newTrap = CreateGameObject(JellybeanPrefab);

                            break;
                        }
                    case IconType.SPEAR_ICON:
                        {
                            newTrap = CreateGameObject(SpearPrefab);

                            break;
                        }
                }

                // successfuly made a trap
                if (newTrap != null)
                {
                    //CMConsole.Log($"Creating a trap");
                   listOfPowerups.Add(newTrap.As<TrapBall>());
                   // CMConsole.Log($"List of power ups: {listOfPowerups.Count}");

                    // random a range from - and + and x offset from camera's position.x
                    float randXPos = CMRand.Range(camera.Position.x - xOffset, camera.Position.x + xOffset);
                    newTrap.Position = new Vector2(randXPos, mainCharacter.Position.y + heightOffset);
                 //   CMConsole.Log($"trap position {randXPos}, {mainCharacter.Position.y + heightOffset}");
                }
                //CMConsole.Log($"trap is null???");

            }
        }
    }
}

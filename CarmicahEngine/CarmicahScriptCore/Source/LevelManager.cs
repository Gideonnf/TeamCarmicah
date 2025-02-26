using System;
using System.IO;
using System.Collections.Generic;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;

namespace Carmicah
{
    public enum EnemyTypes
    {
        MOUSE1,
        MOUSE2,
        MOUSE3,
        BEAR,
        FLYING,
        TOTAL_ENEMIES
    }
    public class Wave
    {
        public int[] enemySpawns = new int[(int)EnemyTypes.TOTAL_ENEMIES];
        public Wave()
        {
            // init to 0
            for(int i = 0; i < enemySpawns.Length; i++)
            {
                enemySpawns[i] = 0;
            }
        }

        public static Wave operator +(Wave w1, Wave w2 )
        {
            Wave newWave = new Wave();
            for(int i = 0; i < w1.enemySpawns.Length; i++)
            {
                newWave.enemySpawns[i] = w1.enemySpawns[i] + w2.enemySpawns[i];
            }

            return newWave;
        }

        public int GetWaveNumbers()
        {
            int total = 0;
            for( int i = 0; i < enemySpawns.Length; ++i)
            {
                total += enemySpawns[i];
            }

            return total;
        }

        public EnemyTypes GetNextEnemy()
        {
            for(int i = 0; i < enemySpawns.Length;++i)
            {
                if (enemySpawns[i] != 0)
                {
                    enemySpawns[i]--;
                    CMConsole.Log($"Getting enemy of type {i}");
                    return (EnemyTypes)i;
                }
            }

            // if no enemy left in the mob counter
            return EnemyTypes.TOTAL_ENEMIES;
        }

        public void PrintWaveData()
        {
            string printStr = "";
            for(int i = 0; i < enemySpawns.Length; ++i)
            {
                printStr += enemySpawns[i] + " ";

            }
            CMConsole.Log(printStr);
        }
    }

    public class Level
    {
        public int currWave = 0;
        public List<Wave> waves = new List<Wave>();
    }
    public class LevelManager
    {
        public int currentLevel = 1; // level starts at 1
       // public int currentWave = 0; // waves start at 0 cause access list at [0]
        private Dictionary<int, Level> levelMap = new Dictionary<int, Level>();
       // string basePath;
        string fileName = "LevelData.txt";
        int NumOfLevels = 0;
        
        public LevelManager() 
        {
            CMConsole.Log($"Testing Level Manager!");
            string filePath = CMFileReader.GetFilePath() + "\\" + fileName;
           // basePath = AppDomain.CurrentDomain.BaseDirectory;
            //filePath = Path.Combine(basePath, "Assets", "levelData.txt");
            CMConsole.Log($"Testing Level Manager! file path : {filePath}");
            LoadLevels(filePath);

            DebugPrintLevelMap();

            NumOfLevels = levelMap.Count;
            // Load from text file
        }

        public void LoadLevels(string filePath)
        {
            if (File.Exists(filePath))
            {
                string[] lines = File.ReadAllLines(filePath);

                //for(int i = 0; i < lines.Length; ++i)
                //{
                //    CMConsole.Log($"Line:{lines[i]}");
                //}

                Level currLevel = null;
                int currentLevel = 0;

                //start at 3 because first 3 lines is format example
                for (int i = 3; i < lines.Length; i++)
                {
                    CMConsole.Log($"Line:{lines[i]} and i {i}");

                    // skip any empty lines
                    if (string.IsNullOrWhiteSpace(lines[i]))
                    {
                        // end of a level chunk
                        // next line will be a new level
                        i++;

                        if (i >= lines.Length) break;

                        if (int.TryParse(lines[i], out int levelNumber))
                        {
                            //CMConsole.Log($"Level:{levelNumber}");
                            currentLevel = levelNumber;
                            currLevel = new Level();
                            levelMap[currentLevel] = currLevel;
                        }

                        continue;
                    }

                    if (currLevel == null) continue;

                    Wave wave = new Wave();

                    string[] enemyCounts = lines[i].Split();
                    // CMConsole.Log($"Line:{lines[i]}");
                    for (int j = 0; j < enemyCounts.Length && j < (int)EnemyTypes.TOTAL_ENEMIES; j++)
                    {
                        if (int.TryParse(enemyCounts[j], out int count))
                        {
                            //DebugPrint(j, count);
                            wave.enemySpawns[j] = count;
                        }
                    }

                    currLevel.waves.Add(wave);
                }




//                for(int i = 0; i < levelMap[1].waves.Count; ++i)
//                {
//                    for(int j = 0; j < levelMap[1].waves[i].enemySpawns.Length; ++j)
//                    {
//                        DebugPrint(j, levelMap[1].waves[i].enemySpawns[j]);
////CMConsole.Log($"{levelMap[1].waves[i].enemySpawns[j]}");
//                    }
//                }
            }
            else
            {
                CMConsole.Log($"Files does not exist {filePath}");
            }
        }
    
        //public Level GetLevel(int level)
        //{
        //    return levelMap.ContainsKey(level) ? levelMap[level] : null;
        //}

        //public Level GetLevel()
        //{
        //    return levelMap.ContainsKey(currentLevel) ? levelMap[currentLevel] : null;
        //}

        public void SetLevel(int level)
        {
            currentLevel = level;
        }

        public Wave GetWave()
        {
            CMConsole.Log("Get Next Wave");
            DebugPrintLevelMap();
            CMConsole.Log($"current level: {currentLevel} and wave {levelMap[currentLevel].currWave}");
            Wave currWave = levelMap[currentLevel].waves[levelMap[currentLevel].currWave];
            if (levelMap[currentLevel].currWave < levelMap[currentLevel].waves.Count)
            {
                levelMap[currentLevel].currWave++;
            }
            CMConsole.Log("Get Next Wave 2");

            currWave.PrintWaveData();
            return currWave;
        }

        public Wave GetWave(int wave)
        {
            levelMap[currentLevel].currWave = wave;
            Wave currWave = levelMap[currentLevel].waves[levelMap[currentLevel].currWave];
            return currWave;
        }

        public bool EndOfLevel()
        {
            Level currLevel = levelMap[currentLevel];
            // end of level
            if (currLevel.currWave >= currLevel.waves.Count)
            {
                return true;
            }
            return false;
        }

        public int GetLevel()
        {
            return currentLevel;
        }

        public void NextLevel()
        {
            if (currentLevel <= levelMap.Count)
            {
                currentLevel++;
            }
        }

        public void DebugPrint(int j, int count)
        {
            CMConsole.Log($"J : {j}, {count}");
        }

        public void DebugPrintLevelMap()
        {
            foreach (KeyValuePair<int, Level> entry in levelMap)
            {
                CMConsole.Log($"Level: {entry.Key}");

                // loop through each wave in that level
                for(int i = 0; i < entry.Value.waves.Count; i++)
                {
                    //CMConsole.Log($"Wave {i}");
                    string debugStr = "";
                    for(int j = 0; j < entry.Value.waves[i].enemySpawns.Length; j++)
                    {
                        debugStr += entry.Value.waves[i].enemySpawns[j] + " ";
                    }
                    CMConsole.Log(debugStr);
                }
            }
        }
    }
}

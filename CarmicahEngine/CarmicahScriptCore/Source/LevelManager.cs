using System;
using System.IO;
using System.Collections.Generic;


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
    }

    public class Level
    {
        public List<Wave> waves = new List<Wave>();
    }
    public class LevelManager
    {
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
                    //CMConsole.Log($"Line:{lines[i]}");

                    // skip any empty lines
                    if (string.IsNullOrWhiteSpace(lines[i]))
                    {
                        // end of a level chunk
                        continue;
                    }

                    // new level
                    if (i % 4 == 0)
                    {
                        if (int.TryParse(lines[i], out int levelNumber))
                        {
                            CMConsole.Log($"Level:{levelNumber}");
                            currentLevel = levelNumber;
                            currLevel = new Level();
                            levelMap[currentLevel] = currLevel;
                        }
                    }
                    else
                    {
                        if (currLevel == null) continue;

                        Wave wave = new Wave();

                        string[] enemyCounts = lines[i].Split();
                        CMConsole.Log($"Line:{lines[i]}");
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
                }

            }
            else
            {
                CMConsole.Log($"Files does not exist {filePath}");
            }
        }
    
        public Level GetLevel(int level)
        {
            return levelMap.ContainsKey(level) ? levelMap[level] : null;
        }

        public void DebugPrint(int j, int count)
        {
            CMConsole.Log($"J : {j}, {count}");
        }
    }
}

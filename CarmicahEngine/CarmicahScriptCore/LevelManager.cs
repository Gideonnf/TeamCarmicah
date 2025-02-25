using System;

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
    public class Level
    {
        int[] enemySpawns = new int[(int)EnemyTypes.TOTAL_ENEMIES];
    }
    public class LevelManager
    {
        int NumOfLevels = 0;
        public LevelManager() 
        {
            //CMConsole.Log("Test");
            // Load from text file
        }
    }
}

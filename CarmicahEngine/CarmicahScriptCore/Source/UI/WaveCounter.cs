using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScript
{

    public class WaveCounter : Entity
    {
        public string WaveSystemObject = "Something";
        Entity waveSystem;
        public bool IsTimer = false;

        public override void OnCreate()
        {
            waveSystem = FindEntityWithName(WaveSystemObject);
        }

        public override void OnUpdate(float dt)
        {
            if (!IsTimer)
            {
                string wave = waveSystem.As<WaveSystem>().levelManager.currentLevel.ToString();
                string dash = "/";
                string totalWaves = waveSystem.As<WaveSystem>().levelManager.NumOfLevels.ToString();

                GetComponent<TextRenderer>().SetText(wave + dash + totalWaves);
            }
            else
            {
                string text = ((int)(waveSystem.As<WaveSystem>().waveStartTime - waveSystem.As<WaveSystem>().waveTimer)).ToString() + "s";
                GetComponent<TextRenderer>().SetText(text);

            }
        }


    }
}

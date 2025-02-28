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

        void OnCreate()
        {
            waveSystem = FindEntityWithName(WaveSystemObject);
        }

        void OnUpdate(float dt)
        {
            if (!IsTimer)
            {
                string text = "/ 5";
                GetComponent<TextRenderer>().SetText(text);
            }
            else
            {
                string text = ((int)(waveSystem.As<WaveSystem>().waveStartTime - waveSystem.As<WaveSystem>().waveTimer)).ToString() + "s";
                GetComponent<TextRenderer>().SetText(text);

            }
        }


    }
}

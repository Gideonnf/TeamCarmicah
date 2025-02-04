using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Cutscene
    {
        public float timer = 0.0f;
        public float timeToChange = 2.0f;
        public string panelName = "panel";
        public int numOfPanels = 2;
        int currentPanel = 1;

        void OnUpdate(float deltaTime)
        {
            timer += deltaTime;
            if (timer >= timeToChange)
            {

            }
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TowerController : Entity
    {
        public int towerHealth = 10;
        public string TowerPrefabStr;
        Entity gm;
        Entity towerPrefab;
        int cakeCounter = 0;

        void onCreate()
        {
            gm = FindEntityWithName("GameManager");
        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_2))
            {
                cakeCounter++;
                TriggerNewTower();
            }
        }

        public void TriggerNewTower()
        {
            CMConsole.Log("TESTING THIS");
            GetComponent<StateMachine>().SetStateCondition(2);
        }



    }
}

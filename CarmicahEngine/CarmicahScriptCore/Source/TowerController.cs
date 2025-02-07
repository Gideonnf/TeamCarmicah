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
        public float ySpawnPos;
        public float yTargetPos;
        public float yVFXSpawn;
        Entity gameManager;
        Entity towerPrefab;
        int cakeCounter = 0;

        void onCreate()
        {
            gameManager = FindEntityWithName("GameManager");
            
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

        public void OnStateEnter(string stateName)
        {
            if (stateName == "TowerIdle")
            {
                //CMConsole.Log("TESTING TOWER IDLE");
            }
            else if (stateName == "TowerCreate")
            {
                //CMConsole.Log("TESTING TOWER CREATE ");

                towerPrefab = CreateGameObject(TowerPrefabStr);
                towerPrefab.Position = new Vector2(Position.x, ySpawnPos);
                

                if (gameManager != null)
                {
                    gameManager.As<GameManager>().SavePositions();
                    gameManager.As<GameManager>().HideEntities();

                }
                //CMConsole.Log("TESTING TOWER CREATE 2");

                GetComponent<StateMachine>().SetStateCondition(3);
            }
            else if (stateName == "TowerLand")
            {
                gameManager.As<GameManager>().UpdatePositions();
                ySpawnPos += gameManager.As<GameManager>().CakeHeightOffset;
                yTargetPos += gameManager.As<GameManager>().CakeHeightOffset;
                GetComponent<StateMachine>().SetStateCondition(1);

            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            if (stateName == "TowerIdle")
            {
                CMConsole.Log("TESTING TOWER IDLE");
            }

            if (stateName == "TowerCreate")
            {
                CMConsole.Log("Testing tower create");
                GetComponent<StateMachine>().SetStateCondition(3);

            }

            if (stateName == "TowerDrop")
            {
                if (towerPrefab.Position.y <= yVFXSpawn)
                {
                    // create the vfx prefab
                }
                CMConsole.Log($"IN TOWER DROP UPDATE {towerPrefab.Position.x}, {towerPrefab.Position.y}");
                if (towerPrefab.Position.y > yTargetPos)
                {
                    Vector2 pos = towerPrefab.Position;
                    pos.y -= 1.5f * dt;
                    towerPrefab.Position = pos;
                }
                else if (towerPrefab.Position.y <= yTargetPos)
                {
                    // tower landed
                    towerPrefab.Position = new Vector2(towerPrefab.Position.x, yTargetPos);
                    GetComponent<StateMachine>().SetStateCondition(4);
                }
            }
        }

        public void OnStateExit(string stateName)
        {

        }


    }
}

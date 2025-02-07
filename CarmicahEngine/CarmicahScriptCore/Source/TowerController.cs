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

        void onCreate()
        {
            gameManager = FindEntityWithName("GameManager");
            
        }

        public void TriggerNewTower()
        {
            GetComponent<StateMachine>().SetStateCondition(2);
        }

        public void OnStateEnter(string stateName)
        {
            if (stateName == "TowerIdle")
            {

            }
            else if (stateName == "TowerCreate")
            {
                towerPrefab = CreateGameObject(TowerPrefabStr);
                towerPrefab.Position = new Vector2(towerPrefab.Position.x, ySpawnPos);
                gameManager.As<GameManager>().SavePositions();
                gameManager.As<GameManager>().HideEntities();

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
            if (stateName == "TowerDrop")
            {
                if (towerPrefab.Position.y <= yVFXSpawn)
                {
                    // create the vfx prefab
                }

                if (towerPrefab.Position.y > yTargetPos)
                {
                    Vector2 pos = towerPrefab.Position;
                    pos.y -= 0.2f * dt;
                    towerPrefab.Position = pos;
                }
                else if (towerPrefab.Position.y <= yTargetPos)
                {
                    // tower landed
                    towerPrefab.Position = new Vector2(towerPrefab.Position.x, yTargetPos);
                    GetComponent<StateMachine>().SetStateCondition(3);
                }
            }
        }

        public void OnStateExit(string stateName)
        {

        }


    }
}

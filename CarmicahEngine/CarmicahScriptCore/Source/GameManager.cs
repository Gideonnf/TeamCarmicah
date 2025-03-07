/* File Documentation ----------------------------------------------------------------------------- 
file:           GameManager.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `GameManager` class, responsible for managing game logic, 
                such as wave spawning, handling entities like mice and cakes, and updating game 
                states. It provides methods to start and end waves and manage interactions 
                with game objects.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using Carmicah;
using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class GameManager : Entity
    {
        public float spawnTimer = 0.5f;
        public string MousePrefabName = "MouseGONew";
        public string BearPrefabName = "BearGO";
        public string CakePrefabName = "StartingCake";
        public string PlayerName = "mainCharacter";
        public string PlayerHealthBar = "Healthbar";
        public string WaveSystemObject = "Something";
        public string CakeVFXPrefab = "CakeVFX";
        public float timer = 0.0f;
        public bool LeftOrRight = false;
        public float CakeHeightOffset;
        public string StartingCake;
        public float dropSpeed = 12.0f;
        //public List<MouseAI> mouseEntitiesLeft; 
        // public List<MouseAI> mouseEntitiesRight;

        public List<MouseAI> mouseLaneOne;
        public List<MouseAI> mouseLaneTwo;
        public List<MouseAI> mouseLaneThree;
        public List<MouseAI> mouseLaneFour;

        //public float WaveStartTime = 25.0f;
        //public float waveTimer = 0.0f;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;
        public string HeroBuild = "HeroBuild";
        public string HeroBuild1 = "HeroBuild_1";
        public string HeroBuild2 = "HeroBuild_2";
        public string HeroBuild3 = "HeroBuild_3";
        public bool GameOver = false;

        // keep track of active enemies
        public int activeEnemies = 0;
        // Keep track of enemies to spawn
        Wave mobCounter;

        Entity startingCakeEntity;
        Entity playerEntity;
        Entity playerHealth;
        Entity playerHealthCover;

        Entity[] endEntities = new Entity[4];

        List<Entity> npcList;

        Entity[] heroBuildEntities = new Entity[4];

        Entity[] walls = new Entity[4];

        int cakeCounter = 1;

        public float ySpawnPos;
        public float yTargetPos;
        public float yVFXSpawn;
        public float yVFXLocation = 1.5f;

        Entity towerPrefab;
        Entity VFXPrefab;
        Entity waveSystem;

        void OnCreate()
        {
            mobCounter = new Wave();
            mouseLaneOne = new List<MouseAI>();
            mouseLaneTwo = new List<MouseAI>();
            mouseLaneThree = new List<MouseAI>();
            mouseLaneFour = new List<MouseAI>();
            npcList = new List<Entity>();

            endEntities[0] = FindEntityWithName(EndPointEntityLeft);
            endEntities[1] = FindEntityWithName(EndPointEntityRight);
            endEntities[2] = FindEntityWithName(EndPointEntityLeft2);
            endEntities[3] = FindEntityWithName(EndPointEntityRight2);

            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
            playerHealth = FindEntityWithName(PlayerHealthBar);
            playerHealthCover = FindEntityWithName("Healthbar_Cover");
            heroBuildEntities[0] = FindEntityWithName(HeroBuild);
            heroBuildEntities[1] = FindEntityWithName(HeroBuild1);
            heroBuildEntities[2] = FindEntityWithName(HeroBuild2);
            heroBuildEntities[3] = FindEntityWithName(HeroBuild3);

            walls[0] = FindEntityWithName("Wall");
            walls[1] = FindEntityWithName("Wall_1");
            walls[2] = FindEntityWithName("Wall_2");
            walls[3] = FindEntityWithName("Wall_3");

            waveSystem = FindEntityWithName(WaveSystemObject);

            Sound.PlayBGM(waveSystem.mID,"BGM_SetupPhase_Mix1", 0.4f);
        }

        void OnUpdate(float dt)
        {

            // if soundPlay
            

            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            // get rid of any npcs that got destroyed?? maybe??
            // I havent gotten to killing NPCs yet so idk if this works
            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0)
                {
                    npcList.Remove(npc);
                    break;
                }
            }

            if (mobCounter.GetWaveNumbers() > 0)
            {
                timer += dt;

                if (timer > spawnTimer)
                {
                    timer = 0.0f;
                    // Get the next enemy type to spawn
                    EnemyTypes type = mobCounter.GetNextEnemy();
                    
                    switch (type)
                    {
                        case EnemyTypes.MOUSE1:
                        {
                            CreateEnemy(MousePrefabName, type);
                            break;
                        }
                        case EnemyTypes.MOUSE2:
                        {
                            CreateEnemy(MousePrefabName, type);

                            break;
                        }
                        case EnemyTypes.MOUSE3:
                        {
                            CreateEnemy(MousePrefabName, type);

                            break;
                        }
                        case EnemyTypes.BEAR:
                        {
                            CreateEnemy(BearPrefabName, type);

                            break;
                        }
                        default:
                        {
                          //  CMConsole.Log("Shouldn't be here tbh");
                            break;
                        }
                    }
                }
            }

            if (Input.IsKeyPressed(Keys.KEY_SPACEBAR))
            {
                GetComponent<StateMachine>().SetStateCondition(2);
            }
        }

        public void StartNextWave(Wave level)
        {
            //CMConsole.Log("Starting next wave");
            // Add it onto wave data
            mobCounter = mobCounter + level;
            for (int i = 0; i < level.enemySpawns.Length; ++i)
            {
                // add how many enemies are coming this wave
                activeEnemies += level.enemySpawns[i];
                //CMConsole.Log("Adding Enemy");
            }
        }

        public void CreateEnemy(string prefabName, EnemyTypes type)
        {
            Entity mouseEntity = CreateGameObject(prefabName);
            MouseAI mouseAI = mouseEntity.As<MouseAI>();

            mouseAI.SetInitialPosition(); // Reset initial position
            mouseEntity.As<MouseAI>().enemyType = type;
            
            //CMConsole.Log($"Adding mouse entity {mouseAI.mID}");

            switch (mouseAI.lane)
            {
                case 0:
                    mouseLaneOne.Add(mouseAI);
                    break;
                case 1:
                    mouseLaneTwo.Add(mouseAI);
                    break;
                case 2:
                    mouseLaneThree.Add(mouseAI);
                    break;
                case 3:
                    mouseLaneFour.Add(mouseAI);
                    break;
            }

            // any special behavour can add here
            switch (type)
            {
                case EnemyTypes.MOUSE1:
                    {
                        mouseEntity.As<MouseAI>().Speed = 1.0f;
                        break;
                    }
                case EnemyTypes.MOUSE2:
                    {
                        mouseEntity.As<MouseAI>().Speed = 1.5f;
                        break;
                    }
                case EnemyTypes.MOUSE3:
                    {
                        mouseEntity.As<MouseAI>().Speed = 1.85f;
                        break;
                    }
                case EnemyTypes.BEAR:
                    {

                        break;
                    }
                default:
                    {
                        CMConsole.Log("Shouldn't be here tbh");
                        break;
                    }
            }

        }

        //public bool IsLanesEmpty()
        //{
        //   if (MobCounter == 0 && BearCounter == 0)
        //    {
        //        if (mouseLaneOne.Count() == 0 && mouseLaneTwo.Count() == 0 && mouseLaneThree.Count() == 0 && mouseLaneFour.Count() == 0)
        //        {
        //            return true;
        //        }
        //    }

        //    return false;
        //}

        public void EntityDestroyed(MouseAI entity)
        {
            //CMConsole.Log("Enemy died");
            activeEnemies--;

            switch (entity.As<MouseAI>().lane)
            {
                case 0:
                    mouseLaneOne.Remove(entity);
                    break;
                case 1:
                    mouseLaneTwo.Remove(entity);
                    break;
                case 2:
                    mouseLaneThree.Remove(entity);
                    break;
                case 3:
                    mouseLaneFour.Remove(entity);
                    break;
            }

            // CMConsole.Log($"Removing mouse entity {entity}");
            //
            // ($"Mouse Destroyed {mouse}");
            //mouseEntities.Remove(mouse);
        }

        public MouseAI GetClosestMouse(HeroAI entity)
        {

            MouseAI targetMouse = null;
            float distance = float.MaxValue;
            if(entity.type == HeroType.SHOOTER)
            {
                switch (entity.lane)
                {
                    case 0:
                        foreach (MouseAI mouse in mouseLaneOne)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                    case 1:
                        foreach (MouseAI mouse in mouseLaneTwo)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                    case 2:
                        foreach (MouseAI mouse in mouseLaneThree)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                    case 3:
                        foreach (MouseAI mouse in mouseLaneFour)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                }
            }
            else if(entity.type == HeroType.MAGE)
            {
                switch (entity.IsLeft)
                {
                    case true:
                        foreach (MouseAI mouse in mouseLaneThree)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }

                        foreach (MouseAI mouse in mouseLaneFour)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                    case false:
                        foreach (MouseAI mouse in mouseLaneOne)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        foreach (MouseAI mouse in mouseLaneTwo)
                        {
                            float dist = mouse.Position.Distance(entity.Position);
                            //CMConsole.Log($"left {dist}");

                            if (dist < distance)
                            {
                                distance = dist;
                                targetMouse = mouse;
                            }
                        }
                        break;
                }
            }

            return targetMouse;
        }

        public void NewNPC(Entity entity)
        {
            npcList.Add(entity);
            CMConsole.Log($"New NPC Entity{entity.mID}");
        }

        public void UpdatePositions()
        {
            Vector2 pos = new Vector2();

            // return them back to the original position
            //for (int i = 0; i < npcSavedPos.Count; i++)
            //{
            //    CMConsole.Log($"Old Pos {npcList[i].Position.y}, vs new pos {npcSavedPos[i].y}");
            //    //npcList[i].Position = npcSavedPos[i];
            //}

            //CMConsole.Log($"Old Pos {playerEntity.Position.y}, vs new pos {playerPos.y}");

            //playerEntity.Position = playerPos;

            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0) continue;

                npc.GetComponent<Renderer>().SetAlpha(1);
                //npc.Position = new Vector2(200, 200);
            }

            playerEntity.GetComponent<Renderer>().SetAlpha(1);
            playerHealth.GetComponent<Renderer>().SetAlpha(1);
            playerHealthCover.GetComponent<Renderer>().SetAlpha(1);
            //playerPos = playerEntity.Position;

            if (playerEntity != null)
            {
                pos = playerEntity.Position;
                pos.y += CakeHeightOffset;
                playerEntity.Position = pos;
            }

            for (int i = 0; i < 4; ++i)
            {
                if (endEntities[i] == null) continue;

                Vector2 position = endEntities[i].Position;
                position.y += CakeHeightOffset;
                endEntities[i].Position = position;
            }

            for (int i = 0; i < 4; ++i)
            {
                if (heroBuildEntities[i] == null) continue;

                Vector2 position = heroBuildEntities[i].Position;
                position.y += CakeHeightOffset;
                heroBuildEntities[i].Position = position;
            }

            for (int i = 0; i < 4; ++i)
            {
                if (walls[i] == null) continue;

                Vector2 position = walls[i].Position;
                position.y += CakeHeightOffset;
                walls[i].Position = position;

            }

            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0) continue;

                pos = npc.Position;
                pos.y += CakeHeightOffset;
                npc.Position = pos;
            }
        }

        public void SavePositions()
        {
            //foreach (Entity npc in npcList)
            //{
            //    if (npc.mID == 0) continue;

            //    CMConsole.Log("Adding to npcList");
            //    npcSavedPos.Add(npc.Position);
            //}

            //playerPos = playerEntity.Position;
        }

        

        public void HideEntities()
        {
            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0) continue;

                npc.GetComponent<Renderer>().SetAlpha(0);
                //npc.Position = new Vector2(200, 200);
            }

            playerEntity.GetComponent<Renderer>().SetAlpha(0);
            playerHealth.GetComponent<Renderer>().SetAlpha(0);
            playerHealthCover.GetComponent<Renderer>().SetAlpha(0);
            //playerPos = new Vector2(200, 200);

        }

        public void LoseGame()
        {
            GameOver = true;
            CreateGameObject("LoseScreen");
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

                if (cakeCounter >= 2) return;

                towerPrefab = CreateGameObject(CakePrefabName);
                towerPrefab.Position = new Vector2(Position.x, ySpawnPos);

                towerPrefab.Depth = startingCakeEntity.Depth;
                towerPrefab.Depth = towerPrefab.Depth + (0.1f * cakeCounter);
                cakeCounter++;
                // if (gameManager != null)
                // {
                //Entity gm = FindEntityWithName("GameManager");

                SavePositions();
                HideEntities();

                // }
                //CMConsole.Log("TESTING TOWER CREATE 2");

                GetComponent<StateMachine>().SetStateCondition(3);
            }
            else if (stateName == "TowerLand")
            {
                // gm = FindEntityWithName("GameManager");
                UpdatePositions();
                ySpawnPos += CakeHeightOffset;
                yTargetPos += CakeHeightOffset;
                yVFXLocation += CakeHeightOffset;
                GetComponent<StateMachine>().SetStateCondition(1);

            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            //gameManager = FindEntityWithName("GameManager");

            if (stateName == "TowerIdle")
            {
                //CMConsole.Log("TESTING TOWER IDLE");
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
                    if (VFXPrefab == null)
                    {
                        VFXPrefab = CreateGameObject(CakeVFXPrefab);
                        VFXPrefab.Position = new Vector2(VFXPrefab.Position.x, yVFXLocation);
                    }
                }
                CMConsole.Log($"IN TOWER DROP UPDATE {towerPrefab.Position.x}, {towerPrefab.Position.y}");
                if (towerPrefab.Position.y > yTargetPos)
                {
                    Vector2 pos = towerPrefab.Position;
                    pos.y -= dropSpeed * dt;
                    towerPrefab.Position = pos;
                }
                else if (towerPrefab.Position.y <= yTargetPos)
                {
                    // tower landed
                    towerPrefab.Position = new Vector2(towerPrefab.Position.x, yTargetPos);
                    GetComponent<StateMachine>().SetStateCondition(4);
                    VFXPrefab.Destroy();
                }
            }
        }

        public void OnStateExit(string stateName)
        {


        }

    }
}
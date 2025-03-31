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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
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
        public string FlyingEnemyPrefabName = "FlyGO"; //fly enemy
        //public string CakePrefabName = "StartingCake";
        public string PlayerName = "mainCharacter";
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
        public List<FlyingEnemyAI> flyingEnemyLeft;
        public List<FlyingEnemyAI> flyingEnemyRight;

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

        Entity[] topTowerBoxes = new Entity[3];

        Entity[] endEntities = new Entity[4];

        Entity[] flyingSpawns = new Entity[2]; 

        List<Entity> npcList;

        Entity[] heroBuildEntities = new Entity[4];

        Entity[] laneIndicators = new Entity[6];

        Entity[] walls = new Entity[4];

        int cakeCounter = 1;

        public float ySpawnPos;
        public float yTargetPos;
        public float yVFXSpawn;
        public float yVFXLocation = 1.5f;
        public float cameraHeight = 10.0f;
        int cakeType = 0; // 0 - 3
        Entity towerPrefab;
        Entity VFXPrefab;
        Entity waveSystem;
        Entity mainCamera;
        Entity UIManager;

        public string[] CakePrefabNames = new string[4];
        public string[] CakeFallAnimations = new string[4];
        public string[] CakeSquishAnimations = new string[4];


        public override void OnCreate()
        {
            mainCamera = FindEntityWithName("MainCamera");
            UIManager = FindEntityWithName("UIManager");
            mobCounter = new Wave();
            mouseLaneOne = new List<MouseAI>();
            mouseLaneTwo = new List<MouseAI>();
            mouseLaneThree = new List<MouseAI>();
            mouseLaneFour = new List<MouseAI>();
            flyingEnemyLeft = new List<FlyingEnemyAI>();
            flyingEnemyRight = new List<FlyingEnemyAI>();
            npcList = new List<Entity>();

            endEntities[0] = FindEntityWithName(EndPointEntityLeft);
            endEntities[1] = FindEntityWithName(EndPointEntityRight);
            endEntities[2] = FindEntityWithName(EndPointEntityLeft2);
            endEntities[3] = FindEntityWithName(EndPointEntityRight2);

            // instead of end point, this will hodl the start point for the bird spawns
            //endEntities[4] = FindEntityWithName("StartTopLeft");
            //endEntities[5] = FindEntityWithName("StartTopRight");

            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
            heroBuildEntities[0] = FindEntityWithName(HeroBuild1);
            heroBuildEntities[1] = FindEntityWithName(HeroBuild3);
            heroBuildEntities[2] = FindEntityWithName(HeroBuild2);
            heroBuildEntities[3] = FindEntityWithName(HeroBuild);

            laneIndicators[0] = FindEntityWithName("Bubble_1");
            laneIndicators[1] = FindEntityWithName("Bubble");
            laneIndicators[2] = FindEntityWithName("Bubble_2");
            laneIndicators[3] = FindEntityWithName("Bubble_3");
            laneIndicators[4] = FindEntityWithName("Bubble_5");
            laneIndicators[5] = FindEntityWithName("Bubble_4");

            walls[0] = FindEntityWithName("Wall");
            walls[1] = FindEntityWithName("Wall_1");
            walls[2] = FindEntityWithName("Wall_2");
            walls[3] = FindEntityWithName("Wall_3");

            topTowerBoxes[0] = FindEntityWithName("TopTowerBox");
            topTowerBoxes[1] = FindEntityWithName("TopTowerBox_1");
            topTowerBoxes[2] = FindEntityWithName("TopTowerBox_2");

            waveSystem = FindEntityWithName(WaveSystemObject);

            CakeFallAnimations[0] = "CakeStrawberry_Fall";
            CakeFallAnimations[1] = "CakeRainbow_Fall";
            CakeFallAnimations[2] = "CakeMatcha_Fall";
            CakeFallAnimations[3] = "CakeFruit_Fall";

            CakeSquishAnimations[0] = "CakeStrawberry_Squish";
            CakeSquishAnimations[1] = "CakeRainbow_Squish";
            CakeSquishAnimations[2] = "CakeMatcha_Squish";
            CakeSquishAnimations[3] = "CakeFruit_Squish";

            // theres 4 but i realy only need 2 tbh cause we cut the first 2 alr
            CakePrefabNames[0] = "Level3_Cake";
            CakePrefabNames[1] = "Level3_Cake";
            CakePrefabNames[2] = "Level3_Cake";
            CakePrefabNames[3] = "Level4_Cake";

            flyingSpawns[0] = FindEntityWithName("StartTopLeft");
            flyingSpawns[1] = FindEntityWithName("StartTopRight");


            Sound.PlayBGM("BGM_SetupPhase_Mix1", 0.4f);
        }

        public override void OnUpdate(float dt)
        {

            // if soundPlay
            

            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                if (pauseManager.As<PauseManager>().IsPaused)
                    return;
            }

            // Cheat code: Kill all enemies (0)
            if (Input.IsKeyPressed(Keys.KEY_0))
            {
                CheatKillAllEnemies();
               //CMConsole.Log("Cheat activated: Killed em'all");

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
                    //EnemyTypes type = mobCounter.GetNextEnemy();
                    EnemyTypes type = mobCounter.GetNextEnemyRandom();

                    switch (type)
                    {
                        case EnemyTypes.MOUSE1:
                        {
                            CreateEnemy("MouseGONew", type);
                            break;
                        }
                        case EnemyTypes.MOUSE2:
                        {
                            CreateEnemy("MouseGONew2", type);

                            break;
                        }
                        case EnemyTypes.MOUSE3:
                        {
                            CreateEnemy("MouseGONew3", type);

                            break;
                        }
                        case EnemyTypes.BEAR:
                        {
                            CreateEnemy(BearPrefabName, type);

                            break;
                        }
                        case EnemyTypes.FLYING:
                            {
                                //CMConsole.Log($"Creating flying enemy");
                                CreateEnemy(FlyingEnemyPrefabName, type);
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

            //if (Input.IsKeyPressed(Keys.KEY_SPACEBAR))
            //{
            //    GetComponent<StateMachine>().SetStateCondition(2);
            //}

            CheckLaneIndicators();
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
            mobCounter.ShuffleEnemies();
        }

        public void CreateEnemy(string prefabName, EnemyTypes type)
        {
            if (type == EnemyTypes.FLYING)
            {
                Entity birdEntity = CreateGameObject(prefabName);
                FlyingEnemyAI flyingAI = birdEntity.As<FlyingEnemyAI>();

                flyingAI.SetInitialPosition();
                flyingAI.enemyType = type;

                if (flyingAI.isLeft)
                {
                    flyingEnemyLeft.Add(flyingAI);
                }
                else
                {
                    flyingEnemyRight.Add(flyingAI);
                }

            }
            else
            {
                Entity mouseEntity = CreateGameObject(prefabName);
                MouseAI mouseAI = mouseEntity.As<MouseAI>();

                mouseAI.SetInitialPosition(); // Reset initial position
                mouseEntity.As<MouseAI>().enemyType = type;

                //CMConsole.Log($"Adding mouse entity {mouseAI.mID}");
                //CMConsole.Log($"Lane : {mouseAI.lane}");
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
                            mouseEntity.As<MouseAI>().Speed = 1.0f;
                            break;
                        }
                }
            }

           
        }

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

        public void EntityDestroyed(FlyingEnemyAI entity)
        {
            activeEnemies--;
            if (entity.isLeft)
            {
                flyingEnemyLeft.Remove(entity);
            }
            else
            {
                flyingEnemyRight.Remove(entity);
            }
        }

        public MouseAI GetClosestMouse(HeroAI entity)
        {

            MouseAI targetMouse = null;
            float distance = float.MaxValue;
            if(entity.type == AbilityType.SHOOTER)
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
            else if(entity.type == AbilityType.MAGE)
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

        public Entity GetTargetNPC(FlyingEnemyAI enemy)
        {
            // check whether left or right
            foreach(Entity entity in npcList)
            {
                // just find the first one that is either left or right
                // i dont wana get into whether to go for first or second
                if (entity.As<BaseNPC>().IsLeft == enemy.isLeft)
                {
                    return entity;
                }
            }

            // if no NPCs, return player
            return playerEntity;
        }

        public void NewNPC(Entity entity)
        {
            npcList.Add(entity);
            //CMConsole.Log($"New NPC Entity{entity.mID}");
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

                //npc.GetComponent<Renderer>().SetAlpha(1);
                if(npc.As<BaseNPC>().mana <= 0)
                {
                    // change to no mana
                    npc.GetComponent<StateMachine>().SetStateCondition(3);
                }
                else
                {
                    // change to Idle
                    npc.GetComponent<StateMachine>().SetStateCondition(1);
                }
                //npc.Position = new Vector2(200, 200);
            }

            // change to Idle state
            playerEntity.GetComponent<StateMachine>().SetStateCondition(1);
            //playerEntity.GetComponent<Renderer>().SetAlpha(1);
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

            foreach(Entity towerBox in topTowerBoxes)
            {
                if (towerBox.mID == 0) continue;

                pos = towerBox.Position;
                pos.y += CakeHeightOffset;
                towerBox.Position = pos;
            }

            foreach(Entity flyingSpawn in flyingSpawns)
            {
                if (flyingSpawn.mID == 0) continue;

                pos = flyingSpawn.Position;
                pos.y += CakeHeightOffset;
                flyingSpawn.Position = pos;
            }
        }

        // for mouse enemies
        public bool KillNPC(MouseAI mouse)
        {
            // if the mouse's lane has an NPC in it when it dies at the top
            // kil the npc
            // NOTE: I dont know if checking for mID == 0 in the list of hero npcs is enough to delete from here
            // if it crashes, check that
            if (heroBuildEntities[mouse.lane].As<HeroBuild>().heroEntity != null && heroBuildEntities[mouse.lane].As<HeroBuild>().heroEntity.mID != 0)
            {
                foreach(Entity entity in npcList)
                {
                    if (entity == heroBuildEntities[mouse.lane].As<HeroBuild>().heroEntity)
                    {
                        npcList.Remove(entity);
                        
                        // let the flying enemies know that they have to change target
                        foreach(FlyingEnemyAI flyingEnemy in flyingEnemyLeft)
                        {
                            flyingEnemy.UpdateTarget(entity);
                        }

                        foreach(FlyingEnemyAI flyingEnemy in flyingEnemyRight)
                        {
                            flyingEnemy.UpdateTarget(entity);
                        }

                        break;
                    }
                }

                heroBuildEntities[mouse.lane].As<HeroBuild>().KillNPC();
                return true;
            }

            // no npc
            return false;
        }

        // for flying enemies
        public void KillNPC(Entity npcEntity)
        {
            foreach(Entity buildEntity in heroBuildEntities)
            {
                if (buildEntity.As<HeroBuild>().heroEntity == npcEntity)
                {
                    if (npcList.Contains(npcEntity))
                    {
                        //CMConsole.Log($"num of npcList : {npcList.Count}");
                        npcList.Remove(npcEntity);

                        // let the flying enemies know that they have to change target
                        foreach (FlyingEnemyAI flyingEnemy in flyingEnemyLeft)
                        {
                            flyingEnemy.UpdateTarget(npcEntity);
                        }

                        foreach (FlyingEnemyAI flyingEnemy in flyingEnemyRight)
                        {
                            flyingEnemy.UpdateTarget(npcEntity);
                        }

                        //CMConsole.Log($"Killing NPC : {npcList.Count}");
                        buildEntity.As<HeroBuild>().KillNPC();
                    }
                }
            }
        }

        public void CheatKillAllEnemies()
        {
            CMConsole.Log("Cheat activated: Killing all enemies");

            // Create temporary lists to avoid modification during iteration
            List<MouseAI> allMouseToKill = new List<MouseAI>();

            // Add all mice from each lane to our kill list
            foreach (MouseAI mouse in mouseLaneOne)
            {
                if (mouse != null && mouse.mID != 0 && !mouse.isDead())
                {
                    allMouseToKill.Add(mouse);
                }
            }

            foreach (MouseAI mouse in mouseLaneTwo)
            {
                if (mouse != null && mouse.mID != 0 && !mouse.isDead())
                {
                    allMouseToKill.Add(mouse);
                }
            }

            foreach (MouseAI mouse in mouseLaneThree)
            {
                if (mouse != null && mouse.mID != 0 && !mouse.isDead())
                {
                    allMouseToKill.Add(mouse);
                }
            }

            foreach (MouseAI mouse in mouseLaneFour)
            {
                if (mouse != null && mouse.mID != 0 && !mouse.isDead())
                {
                    allMouseToKill.Add(mouse);
                }
            }

            // Kill all mice in our list
            foreach (MouseAI mouse in allMouseToKill)
            {
                mouse.KillMouse();
            }

            // Handle flying enemies
            List<FlyingEnemyAI> allBirdsToKill = new List<FlyingEnemyAI>();

            foreach (FlyingEnemyAI bird in flyingEnemyLeft)
            {
                if (bird != null && bird.mID != 0 && !bird.isDead())
                {
                    allBirdsToKill.Add(bird);
                }
            }

            foreach (FlyingEnemyAI bird in flyingEnemyRight)
            {
                if (bird != null && bird.mID != 0 && !bird.isDead())
                {
                    allBirdsToKill.Add(bird);
                }
            }

            // Kill all birds in our list
            foreach (FlyingEnemyAI bird in allBirdsToKill)
            {
                bird.KillEnemy();
            }

            activeEnemies = 0;
        }




        public void HideEntities()
        {
            // set them to teleport state


            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0) continue;

                npc.GetComponent<StateMachine>().SetStateCondition(5);
                
            }

            playerEntity.GetComponent<StateMachine>().SetStateCondition(5);
            //playerEntity.GetComponent<Renderer>().SetAlpha(0);
            //playerPos = new Vector2(200, 200);

        }

        public void LoseGame()
        {
            GameOver = true;
            Entity pauseManager = FindEntityWithName("PauseManager");
            pauseManager.As<PauseManager>().IsPaused = true;
            Sound.SwitchBGM("LoseScreen", 0.5f, 0.5f);
            
            Sound.StopAllSFX();
            //UIManager.As<UIManager>().CreateMenu("LoseScreen");
            CreateGameObject("LoseScreen");
        }

        public void CheckLaneIndicators()
        {
            bool visible = false;
            if (mouseLaneOne.Count > 0)
            {
                foreach (MouseAI mouse in mouseLaneOne) // left lane of right side
                {
                    // if it is belo the camera
                    if (mouse.Position.y < (mainCamera.Position.y - cameraHeight))
                    {
                        visible = false;
                    }
                    else if (mouse.Position.y > (mainCamera.Position.y - cameraHeight))
                    {
                        visible = true;
                        // CMConsole.Log($"Visible Lane 1 {visible}");
                        break;
                    }
                }
                if (!visible)
                {
                    laneIndicators[0].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[0].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[0].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[0].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }
            }
            else
            {
                laneIndicators[0].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[0].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }

            // reset visible flag
            visible = false;
            if (mouseLaneTwo.Count > 0)
            {
                foreach (MouseAI mouse in mouseLaneTwo) // right lane of right side
                {
                    if (mouse.Position.y < (mainCamera.Position.y - cameraHeight))
                    {
                        visible = false;
                    }
                    else if (mouse.Position.y > (mainCamera.Position.y - cameraHeight))
                    {
                        visible = true;
                        //CMConsole.Log($"Visible Lane 2 {visible}");
                        break;
                    }
                }

                if (!visible)
                {
                    laneIndicators[1].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[1].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[1].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[1].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }
            }
            else
            {
                laneIndicators[1].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[1].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }

            visible = false;
            if (mouseLaneThree.Count > 0)
            {
                foreach (MouseAI mouse in mouseLaneThree) //right lane of left side
                {
                    if (mouse.Position.y < (mainCamera.Position.y - cameraHeight))
                    {
                        visible = false;
                    }
                    else if (mouse.Position.y > (mainCamera.Position.y - cameraHeight))
                    {
                        visible = true;
                        //CMConsole.Log($"Visible Lane 3 {visible}");
                        break;
                    }
                }


                if (!visible)
                {
                    laneIndicators[2].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[2].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[2].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[2].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }

            }
            else
            {
                laneIndicators[2].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[2].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }

            visible = false;
            if (mouseLaneFour.Count > 0)
            {
                foreach (MouseAI mouse in mouseLaneFour) // left side of left side
                {
                    if (mouse.Position.y < (mainCamera.Position.y - cameraHeight))
                    {
                        visible = false;
                    }
                    else if (mouse.Position.y > (mainCamera.Position.y - cameraHeight))
                    {
                        visible = true;
                        //CMConsole.Log($"Visible Lane 4 {visible}");
                        break;
                    }
                }

                if (!visible)
                {
                    laneIndicators[3].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[3].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[3].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[3].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }
            }
            else
            {
                laneIndicators[3].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[3].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }

            visible = false;
            if (flyingEnemyLeft.Count > 0)
            {
                foreach(FlyingEnemyAI bird in flyingEnemyLeft)
                {
                    if (bird.Position.y > (mainCamera.Position.y + cameraHeight))
                    {
                        visible = false;
                    }
                    else if (bird.Position.y < (mainCamera.Position.y + cameraHeight))
                    {
                        visible = true;
                        break;
                    }
                }

                if (!visible)
                {
                    laneIndicators[4].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[4].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[4].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[4].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }
            }
            else
            {
                laneIndicators[4].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[4].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }


            visible = false;
            if (flyingEnemyRight.Count > 0)
            {
                foreach (FlyingEnemyAI bird in flyingEnemyRight)
                {
                    if (bird.Position.y > (mainCamera.Position.y + cameraHeight))
                    {
                        visible = false;
                    }
                    else if (bird.Position.y < (mainCamera.Position.y + cameraHeight))
                    {
                        visible = true;
                        break;
                    }
                }

                if (!visible)
                {
                    laneIndicators[5].GetComponent<Renderer>().SetAlpha(1.0f);
                    laneIndicators[5].GetComponentInChildren<Renderer>().SetAlpha(1.0f);
                }
                else
                {
                    laneIndicators[5].GetComponent<Renderer>().SetAlpha(0.0f);
                    laneIndicators[5].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
                }
            }
            else
            {
                laneIndicators[5].GetComponent<Renderer>().SetAlpha(0.0f);
                laneIndicators[5].GetComponentInChildren<Renderer>().SetAlpha(0.0f);
            }

        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "TowerIdle")
            {
                //CMConsole.Log("TESTING TOWER IDLE");
            }
            else if (stateName == "TowerCreate")
            {
                //CMConsole.Log("TESTING TOWER CREATE ");

                if (cakeCounter >= 2) return;

                Sound.PlayBGM("BGM_SetupPhase_Mix1", 0.4f);

                cakeType = 2;//CMRand.Range(0, 3);
                //CMConsole.Log($"cake type {cakeType}");
                Sound.PlaySFX("TowerStack", 1.0f);
                towerPrefab = CreateGameObject(CakePrefabNames[cakeType]);
                towerPrefab.Position = new Vector2(Position.x, ySpawnPos);

                towerPrefab.GetComponent<Animation>().ChangeAnim(CakeFallAnimations[cakeType]);
                towerPrefab.Depth = startingCakeEntity.Depth;
                towerPrefab.Depth = towerPrefab.Depth + (0.1f * cakeCounter);
                cakeCounter++;
                // if (gameManager != null)
                // {
                //Entity gm = FindEntityWithName("GameManager");

                //SavePositions();
                HideEntities();

                // }
                //CMConsole.Log("TESTING TOWER CREATE 2");

                GetComponent<StateMachine>().SetStateCondition(3);
            }
            else if (stateName == "TowerLand")
            {
                // gm = FindEntityWithName("GameManager");
               

            }
        }
        public override void OnStateUpdate(string stateName, float dt)
        {
            //gameManager = FindEntityWithName("GameManager");

            if (stateName == "TowerIdle")
            {
                //CMConsole.Log("TESTING TOWER IDLE");
            }

            if (stateName == "TowerCreate")
            {
                //CMConsole.Log("Testing tower create");
                
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
                        
                        VFXPrefab.Position = new Vector2(-0.75f, yVFXLocation);
                    }
                }
               // CMConsole.Log($"IN TOWER DROP UPDATE {towerPrefab.Position.x}, {towerPrefab.Position.y}");
                if (towerPrefab.Position.y > yTargetPos)
                {
                    Vector2 pos = towerPrefab.Position;
                    pos.y -= dropSpeed * dt;
                    towerPrefab.Position = pos;
                }
                else if (towerPrefab.Position.y <= yTargetPos && (VFXPrefab != null && VFXPrefab.GetComponent<Animation>().IsAnimFinished()))
                {
                    towerPrefab.GetComponent<Animation>().ChangeAnim(CakeSquishAnimations[cakeType]);
                    // tower landed
                    towerPrefab.Position = new Vector2(towerPrefab.Position.x, yTargetPos);
                    GetComponent<StateMachine>().SetStateCondition(4);
                    //CMConsole.Log("Changing VFX prefab animation");
                    VFXPrefab.ChangeAnim("CakeFallVFxEnd");
                    
                    Sound.PlaySFX("SFX__Magic", 0.4f);

                }

            }

            if (stateName == "TowerLand")
            {
                if (VFXPrefab.GetComponent<Animation>().GetFrameNo() == 0)
                {
                    VFXPrefab.Depth = towerPrefab.Depth + 1.0f;
                }
                if (VFXPrefab.GetComponent<Animation>().IsAnimFinished() && towerPrefab.GetComponent<Animation>().IsAnimFinished())
                {
                    VFXPrefab.Destroy();
                    UpdatePositions();
                    ySpawnPos += CakeHeightOffset;
                    yTargetPos += CakeHeightOffset;
                    yVFXLocation += CakeHeightOffset;
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }
        }

        public override void OnStateExit(string stateName)
        {


        }

    }
}
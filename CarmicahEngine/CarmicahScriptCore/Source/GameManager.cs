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
        public string MousePrefabName = "MouseGO";
        public string CakePrefabName = "StartingCake";
        public string PlayerName = "mainCharacter";
        public float timer = 0.0f;
        public bool LeftOrRight = false;
        public float CakeHeightOffset;
        public string StartingCake;
        public List<MouseAI> mouseEntitiesLeft; 
        public List<MouseAI> mouseEntitiesRight;
        //public float WaveStartTime = 25.0f;
        //public float waveTimer = 0.0f;
        public string EndPointEntityLeft;
        public string EndPointEntityRight;
        public string EndPointEntityLeft2;
        public string EndPointEntityRight2;
        public string HeroBuild = "HeroBuild";
        public string HeroBuild1 = "HeroBuild_1";

        public bool GameStart = false;
        public int MobCounter = 0;
        //public float NumOfMobs = 10;
        
        bool Musicplay = false;

        Entity startingCakeEntity;
        Entity playerEntity;
        Entity endEntityLeft;
        Entity endEntityRight;
        Entity endEntityLeft2;
        Entity endEntityRight2;
        List<Entity> npcList;
        Entity heroBuildEntity;
        Entity heroBuildEntity1;

        Entity wall;
        Entity wall1;
        Entity wall2;
        Entity wall3;
        int cakeCounter = 1;

        void OnCreate()
        {
            mouseEntitiesLeft = new List<MouseAI>();
            mouseEntitiesRight = new List<MouseAI>();
            npcList = new List<Entity>();

            endEntityLeft = FindEntityWithName(EndPointEntityLeft);
            endEntityRight = FindEntityWithName(EndPointEntityRight);
            endEntityLeft2 = FindEntityWithName(EndPointEntityLeft2);
            endEntityRight2 = FindEntityWithName(EndPointEntityRight2);

            startingCakeEntity = FindEntityWithName(StartingCake);
            playerEntity = FindEntityWithName(PlayerName);
            heroBuildEntity = FindEntityWithName(HeroBuild);
            heroBuildEntity1 = FindEntityWithName(HeroBuild1);

            wall = FindEntityWithName("Wall");
            wall1 = FindEntityWithName("Wall_1");
            wall2 = FindEntityWithName("Wall_2");
            wall3 = FindEntityWithName("Wall_3");
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

            if (MobCounter > 0)
            {
                timer += dt;

                if (timer > spawnTimer)
                {
                    timer = 0.0f;
                    Entity mouseEntity = CreateGameObject(MousePrefabName);
                    MouseAI mouseAI = mouseEntity.As<MouseAI>();
                    mouseAI.isLeft = LeftOrRight;
                    LeftOrRight = !LeftOrRight;
                    mouseAI.SetInitialPosition(); // Reset initial position
                    MobCounter--;

                    CMConsole.Log($"Adding mouse entity {mouseAI.mID}");

                    if (mouseAI.isLeft)
                    {
                        mouseEntitiesLeft.Add(mouseAI);
                        //CMConsole.Log($"Mouse List left {mouseEntitiesLeft.Count}");

                    }
                    else
                    {
                        mouseEntitiesRight.Add(mouseAI);
                       // CMConsole.Log($"Mouse List right {mouseEntitiesRight.Count}");
                    }
                }
            }


            if (Input.IsKeyPressed(Keys.KEY_SPACEBAR))
            {
                if (cakeCounter < 3)
                {
                    Entity cakeEntity = CreateGameObject(CakePrefabName);
                    // Set the cake position
                    cakeEntity.Position = startingCakeEntity.Position;
                    cakeEntity.Scale = startingCakeEntity.Scale;
                    cakeEntity.Depth = startingCakeEntity.Depth;

                    cakeEntity.Depth = cakeEntity.Depth + (0.1f * cakeCounter);
                    Vector2 pos = cakeEntity.Position;
                    pos.y += (CakeHeightOffset * cakeCounter);
                    cakeEntity.Position = pos;
                    cakeCounter++;

                    UpdatePositions();
                }
            }
        }

        public void StartNextWave(int mobCount)
        {
            GameStart = true;
            MobCounter += mobCount;
        }

        public void EntityDestroyed(MouseAI entity)
        {
           // CMConsole.Log($"Removing mouse entity {entity}");
            if (entity.As<MouseAI>().isLeft)
            {
                mouseEntitiesLeft.Remove(entity);
                //if ()
                //    CMConsole.Log($"Mouse List left Removal {mouseEntitiesLeft.Count}");

            }
            else
            {
                mouseEntitiesRight.Remove(entity);
                //if ()
                //    CMConsole.Log($"Mouse List right Removal {mouseEntitiesRight.Count}");

            }
            //
            // ($"Mouse Destroyed {mouse}");
            //mouseEntities.Remove(mouse);
        }

        public MouseAI GetClosestMouse(HeroAI entity)
        {

            MouseAI targetMouse = null;
            float distance = float.MaxValue;
            // if its on left side
            if (entity.IsLeft)
            {
                foreach (MouseAI mouse in mouseEntitiesLeft)
                {
                    // Get distance to 
                    float dist = mouse.Position.Distance(entity.Position);
                    //CMConsole.Log($"left {dist}");

                    if (dist < distance)
                    {
                        distance = dist;
                        targetMouse = mouse;
                    }
                }
            }
            // if its on right side
            else
            {
                foreach (MouseAI mouse in mouseEntitiesRight)
                {
                    // Get distance to 
                    float dist = mouse.Position.Distance(entity.Position);
                   // CMConsole.Log($"right {dist}");
                    if (dist < distance)
                    {
                        distance = dist;
                        targetMouse = mouse;
                    }
                }

            }

            //if (targetMouse != null)
            //    CMConsole.Log($"Target mouse : {targetMouse.mID}");

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

            if (playerEntity != null)
            {
                pos = playerEntity.Position;
                pos.y += CakeHeightOffset;
                playerEntity.Position = pos;
            }

            if (endEntityLeft != null)
            {
                pos = endEntityLeft.Position;
                pos.y += CakeHeightOffset;
                endEntityLeft.Position = pos;
            }

            if (endEntityLeft2 != null)
            {
                pos = endEntityLeft2.Position;
                pos.y += CakeHeightOffset;
                endEntityLeft2.Position = pos;
            }

            if (endEntityRight != null)
            {
                pos = endEntityRight.Position;
                pos.y += CakeHeightOffset;
                endEntityRight.Position = pos;
            }

            if (endEntityRight2 != null)
            {
                pos = endEntityRight2.Position;
                pos.y += CakeHeightOffset;
                endEntityRight2.Position = pos;
            }

            if (heroBuildEntity != null)
            {
                pos = heroBuildEntity.Position;
                pos.y += CakeHeightOffset;
                heroBuildEntity.Position = pos;
            }

            if (heroBuildEntity1 != null)
            {
                pos = heroBuildEntity1.Position;
                pos.y += CakeHeightOffset;
                heroBuildEntity1.Position = pos;
            }

            if (wall != null)
            {
                pos = wall.Position;
                pos.y += CakeHeightOffset;
                wall.Position = pos;
            }
            if (wall1 != null)
            {
                pos = wall1.Position;
                pos.y += CakeHeightOffset;
                wall1.Position = pos;
            }
            if (wall2 != null)
            {
                pos = wall2.Position;
                pos.y += CakeHeightOffset;
                wall2.Position = pos;
            }
            if (wall3 != null)
            {
                pos = wall3.Position;
                pos.y += CakeHeightOffset;
                wall3.Position = pos;
            }

            foreach (Entity npc in npcList)
            {
                if (npc.mID == 0) continue;

                pos = npc.Position;
                pos.y += CakeHeightOffset;
                npc.Position = pos;
            }
        }

    }
}
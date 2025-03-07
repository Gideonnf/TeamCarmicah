/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			HowToPlay.cs
  
 author:		

 email:		    

 brief:			

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HowToPlay : Entity
    {
        public int currentPanel = 0;       // panel 0 ~ 5 (inclusive)
        Entity nextBtn = null;
        Entity backBtn = null;
        Entity playBtn = null;
        bool isPlayerFacingLeft = true;
        int frameChanged = 2;

        Entity playerWalk   = null;
        Entity cursor       = null;
        Entity enemyMouse1  = null;
        Entity enemyMouse2  = null;
        Entity enemyBear    = null;
        bool mouse1Climbing = true;
        bool mouse2Climbing = true;
        bool bearClimbing   = true;
        Entity power1Ico    = null;
        Entity power2Ico    = null;

        void OnCreate()
        {
            nextBtn = FindEntityWithName("HowToRightBtn");
            backBtn = FindEntityWithName("HowToLeftBtn");
            playBtn = FindEntityWithName("HowToPlayBtn");
        }

        private string GetPanelName(int pan)
        {
            if (pan < 6)
                return "Tutorial_Spritesheet_How " + pan;
            return "Tutorial_Spritesheet_How 0";
        }

        public void ProgressScene(int num)
        {
            // Delete The How To Prefab
            if(currentPanel != 4)
            {
                Entity toDestroy = FindEntityWithName("HowToStep" + currentPanel);
                if (toDestroy != null)
                {
                    toDestroy.Destroy();
                }
            }
            // Set buttons visibility if any
            if(currentPanel == 0)
            {
                backBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            }
            else if (currentPanel == 5)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
                playBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            }

            // Advance Panel
            currentPanel += num;
            GetComponent<Renderer>().ChangeTexture(GetPanelName(currentPanel));

            // If Overshot, end
            if(currentPanel >= 6)
            {
                Destroy();
                return;
            }
            // Return to first panel
            if(currentPanel <= 0)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
                backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                currentPanel = 0;
                isPlayerFacingLeft = true;
            }
            // Last Panel
            else if(currentPanel == 5)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                playBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            }
            // Create Objs for every panel
            if(currentPanel != 4)
            {
                CreateGameObject("HowToStep" + currentPanel);
            }
            frameChanged = 2;
        }

        void OnUpdate(float dt)
        {
            // Whole purpose to allow for creation of things before getting entites
            if(frameChanged > 0)
            {
                if(frameChanged == 1)
                {
                    switch(currentPanel)
                    {
                        case 1:
                            enemyMouse1 = FindEntityWithName("HowToMice1");
                            enemyMouse2 = FindEntityWithName("HowToMice2");
                            enemyBear = FindEntityWithName("HowToBear");
                            mouse1Climbing = mouse2Climbing = bearClimbing = true;
                            break;
                    }

                }
                --frameChanged;
                return;
            }
            switch (currentPanel)
            {
                case 0:
                    if (Input.IsKeyPressed(Keys.KEY_A))
                    {
                        if (!isPlayerFacingLeft)
                        {
                            Entity et = FindEntityWithName("HowToPlayerWalk");
                            Vector2 scale = et.Scale;
                            scale.x = 1.0f;
                            et.Scale = scale;
                        }

                        isPlayerFacingLeft = true;
                    }
                    if (Input.IsKeyPressed(Keys.KEY_D))
                    {
                        if (isPlayerFacingLeft)
                        {
                            Entity et = FindEntityWithName("HowToPlayerWalk");
                            Vector2 scale = et.Scale;
                            scale.x = -1.0f;
                            et.Scale = scale;
                        }

                        isPlayerFacingLeft = false;
                    }
                break;
                case 1:
                    Vector2 pos;
                    if (mouse1Climbing)
                    {
                        pos = enemyMouse1.LocalPosition;
                        pos.y += 150 * dt;
                        enemyMouse1.LocalPosition = pos;
                    }
                    if (mouse2Climbing)
                    {
                        pos = enemyMouse2.LocalPosition;
                        pos.y += 200 * dt;
                        enemyMouse2.LocalPosition = pos;
                    }
                    if (bearClimbing)
                    {
                        pos = enemyBear.LocalPosition;
                        pos.y += 100 * dt;
                        enemyBear.LocalPosition = pos;
                    }

                    if (mouse1Climbing && enemyMouse1.LocalPosition.y > 20)
                    {
                        enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Death_blue");
                        mouse1Climbing = false;
                    }
                    if (mouse2Climbing && enemyMouse2.LocalPosition.y > 20)
                    {
                        enemyMouse2.GetComponent<Animation>().ChangeAnim("Mouse_Death_brown");
                        mouse2Climbing = false;
                    }
                    if (bearClimbing && enemyBear.LocalPosition.y > 20)
                    {
                        enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Death");
                        bearClimbing = false;
                    }
                break;
                case 2:

                break;
                case 3:

                break;
            }
        }
    }
}

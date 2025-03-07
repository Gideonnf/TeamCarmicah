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
        int frameChanged = 2;// For Making SURE that things are loaded before being used
        Entity nextBtn = null;
        Entity backBtn = null;
        Entity playBtn = null;
        bool isPlayerFacingLeft = true;

        Entity playerWalk = null;
        Entity cursor = null;
        Entity enemyMouse1 = null;
        Entity enemyMouse2 = null;
        Entity enemyBear = null;
        bool mouse1Climbing = true;
        bool mouse2Climbing = true;
        bool bearClimbing = true;
        Entity power1Ico = null;
        Entity power2Ico = null;
        int aniProgress = 0;
        Vector2 someDir;
        Vector2 pos;

        bool isClicked = false;
        float clickTime = 2.0f;

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
            if (currentPanel != 4)
            {
                Entity toDestroy = FindEntityWithName("HowToStep" + currentPanel);
                if (toDestroy != null)
                {
                    toDestroy.Destroy();
                }
            }
            // Set buttons visibility if any
            if (currentPanel == 0)
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
            if (currentPanel >= 6)
            {
                Destroy();
                return;
            }
            // Return to first panel
            if (currentPanel <= 0)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
                backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                currentPanel = 0;
            }
            // Last Panel
            else if (currentPanel == 5)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                playBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            }
            // Create Objs for every panel
            if (currentPanel != 4)
            {
                CreateGameObject("HowToStep" + currentPanel);
            }

            // Set uniforms
            isPlayerFacingLeft = true;
            frameChanged = 2;
            aniProgress = 0;
        }

        void OnUpdate(float dt)
        {
            // Whole purpose to allow for creation of things before getting entites
            if (frameChanged > 0)
            {
                if (frameChanged == 1)
                {
                    switch (currentPanel)
                    {
                        case 0:
                            playerWalk = FindEntityWithName("HowToPlayerWalk");
                            break;
                        case 1:
                            enemyMouse1 = FindEntityWithName("HowToMice1");
                            enemyMouse2 = FindEntityWithName("HowToMice2");
                            enemyBear = FindEntityWithName("HowToBear");
                            mouse1Climbing = mouse2Climbing = bearClimbing = true;
                            break;
                        case 3:
                            playerWalk = FindEntityWithName("HowToPrincess");
                            cursor = FindEntityWithName("HowToCursor");
                            power1Ico = FindEntityWithName("HowToDrop");
                            power2Ico = FindEntityWithName("HowToShooter");
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
                            Vector2 scale = playerWalk.Scale;
                            scale.x = 1.0f;
                            playerWalk.Scale = scale;
                        }

                        isPlayerFacingLeft = true;
                    }
                    if (Input.IsKeyPressed(Keys.KEY_D))
                    {
                        if (isPlayerFacingLeft)
                        {
                            Vector2 scale = playerWalk.Scale;
                            scale.x = -1.0f;
                            playerWalk.Scale = scale;
                        }

                        isPlayerFacingLeft = false;
                    }
                    break;
                case 1:
                    if (mouse1Climbing)
                    {
                        pos = enemyMouse1.LocalPosition;
                        pos.y += 150.0f * dt;
                        enemyMouse1.LocalPosition = pos;
                    }
                    if (mouse2Climbing)
                    {
                        pos = enemyMouse2.LocalPosition;
                        pos.y += 200.0f * dt;
                        enemyMouse2.LocalPosition = pos;
                    }
                    if (bearClimbing)
                    {
                        pos = enemyBear.LocalPosition;
                        pos.y += 100.0f * dt;
                        enemyBear.LocalPosition = pos;
                    }

                    if (mouse1Climbing && enemyMouse1.LocalPosition.y > -60)
                    {
                        enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Death_blue");
                        mouse1Climbing = false;
                    }
                    if (mouse2Climbing && enemyMouse2.LocalPosition.y > 0)
                    {
                        enemyMouse2.GetComponent<Animation>().ChangeAnim("Mouse_Death_brown");
                        mouse2Climbing = false;
                    }
                    if (bearClimbing && enemyBear.LocalPosition.y > -60)
                    {
                        enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Death");
                        bearClimbing = false;
                    }

                    if (!mouse1Climbing && !mouse2Climbing && !bearClimbing && enemyMouse2.GetComponent<Animation>().IsAnimFinished())
                    {
                        enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Climb_blue");
                        enemyMouse2.GetComponent<Animation>().ChangeAnim("Mouse_Climb_brown");
                        enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Climb");
                        pos = enemyMouse1.LocalPosition;
                        pos.y = -310.0f;
                        enemyMouse1.LocalPosition = pos;
                        pos = enemyMouse2.LocalPosition;
                        pos.y = -260.0f;
                        enemyMouse2.LocalPosition = pos;
                        pos = enemyBear.LocalPosition;
                        pos.y = -280.0f;
                        enemyBear.LocalPosition = pos;
                        mouse1Climbing = mouse2Climbing = bearClimbing = true;
                    }
                    break;
                case 2:
                    /*
                    switch(aniProgress)
                    {
                        case 0:
                            someDir = (new Vector2(1, 1)) - cursor.LocalPosition;
                            someDir = someDir.Normalize();
                            ++aniProgress;
                            break;
                        case 1:
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if (pos.x < 1)
                            {
                                someDir = (new Vector2(460.0f, 150)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                isPlayerFacingLeft = true; // using this as a bool i'm lazy
                                ++aniProgress;
                            }
                            break;
                        case 2:
                            break;
                    }
                    */
                    // Cursor moves to 465, 155 (trap)
                    // Cursor moves to 465, 80 (Shooter)
                    // Cursor moves to -130, 75 (place shooter)
                    // Haves: Shooter (-135, 110) (0.666 x 0.666)

                    break;
                case 3:
                    switch (aniProgress)
                    {
                    // Thing falls to y = 115
                        case 0:
                            pos = power1Ico.LocalPosition;
                            pos.y -= 150.0f * dt;
                            power1Ico.LocalPosition = pos;
                            if (pos.y < 75.0f)
                            {
                                someDir = (new Vector2(55.0f, 75.0f)) - playerWalk.LocalPosition;
                                someDir = someDir.Normalize();

                                playerWalk.Scale = new Vector2(-0.666f, 0.666f);
                                playerWalk.GetComponent<Animation>().ChangeAnim("MC_Walk");
                                ++aniProgress;
                            }
                            break;
                    // Princess walks to 55,115 (flip x)
                        case 1:
                            pos = playerWalk.LocalPosition;
                            pos.x += someDir.x * dt * 100.0f;
                            pos.y += someDir.y * dt * 100.0f;
                            playerWalk.LocalPosition = pos;
                            // Thing teleports to 450, 190
                            // Thing grows to 1.3333
                            if(pos.x > 55.0f)
                            {
                                playerWalk.GetComponent<Animation>().ChangeAnim("MC_Idle");

                                power1Ico.LocalPosition = new Vector2(450.0f, 190.0f);
                                power1Ico.Scale = new Vector2(0.01f, 0.01f);

                                someDir = (new Vector2(460.0f, 150)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                isPlayerFacingLeft = true; // using this as a bool i'm lazy
                                ++aniProgress;
                            }
                            break;
                    // Mouse moves from offscreen to 460, 150
                        case 2:
                            if(isPlayerFacingLeft)
                            {
                                pos = power1Ico.Scale;
                                pos.x += dt;
                                pos.y += dt;

                                if(pos.x > 1.0f)
                                {
                                    pos.x = pos.y = 1.0f;
                                    isPlayerFacingLeft = false;
                                }
                                power1Ico.Scale = pos;
                            }
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if(pos.x < 460.0f)
                            {
                                isPlayerFacingLeft = false;
                                // Converts Head to Mage (scale 0.666)
                                power1Ico.Scale = new Vector2(-0.666f, 0.666f);
                                someDir = (new Vector2(-15.0f, 85.0f)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                power1Ico.GetComponent<Renderer>().ChangeTexture("NPC_SpriteSheet_Mage_Idle 0");
                                FindEntityWithName("HowToMage").GetComponent<Renderer>().SetAlpha(0.3f);
                                ++aniProgress;
                            }
                            break;
                    // Mouse moves to -15, 125 & Mage spawns
                        case 3:
                            pos = power1Ico.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            power1Ico.LocalPosition = pos;
                            if(!isPlayerFacingLeft && pos.x < 0)
                            {
                                power1Ico.Scale = new Vector2(0.666f, 0.666f);
                                isPlayerFacingLeft = true;
                            }
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if(pos.x < -15.0f)
                            {
                                someDir = (new Vector2(180.0f, 85.0f)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                power1Ico.GetComponent<Renderer>().ChangeTexture("UI_Spritesheet_Mage_Icon 0");
                                power1Ico.LocalPosition = new Vector2(100.0f, 450.0f);
                                FindEntityWithName("HowToMage").GetComponent<Renderer>().SetAlpha(1.0f);
                                isClicked = true;
                                clickTime = 2.0f;
                                ++aniProgress;
                            }
                            break;
                    // Mouse moves to 180, 125 (heal shooter (shooter located (40,110) -0.66,0.66 scale)
                        case 4:
                            if(isClicked)
                            {
                                clickTime -= dt;
                                if (clickTime < 0.0f)
                                    isClicked = false;
                                break;
                            }

                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if(pos.x > 180.0f)
                            {
                                isPlayerFacingLeft = true;
                                playerWalk.GetComponent<Animation>().ChangeAnim("MC_Heal");
                                ++aniProgress;
                            }
                            break;
                        case 5:
                            if (isPlayerFacingLeft)
                            {
                                if (playerWalk.GetComponent<Animation>().IsAnimFinished())
                                {
                                    power2Ico.GetComponent<Animation>().ChangeAnim("Shooter_Idle");
                                    playerWalk.GetComponent<Animation>().ChangeAnim("MC_Idle");
                                    isPlayerFacingLeft = false;
                                    pos.x = 0.0f;
                                }
                            }
                            else
                            {
                                pos.x += dt;
                                if (pos.x > 2.0f)
                                {
                                    playerWalk.LocalPosition = new Vector2(-160.0f, 105.0f);
                                    playerWalk.Scale = new Vector2(0.666f, 0.666f);
                                    FindEntityWithName("HowToMage").GetComponent<Renderer>().SetAlpha(0.0f);
                                    power2Ico.GetComponent<Animation>().ChangeAnim("Shooter_Mana");
                                    power1Ico.Scale = new Vector2(1, 1);
                                    cursor.LocalPosition = new Vector2(1000.0f, 100.0f);
                                    isPlayerFacingLeft = true;
                                    aniProgress = 0;
                                }
                            }
                            break;
                    }
                    // Mouse moves back to 1'000, 100
                    break;
            }
        }
    }
}

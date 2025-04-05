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
        int oldPanel = -1;
        int frameChanged = 2;// For Making SURE that things are loaded before being used
        //Entity nextBtn = null;
        //Entity backBtn = null;
        //Entity playBtn = null;
        //Entity homeBtn = null;
        bool boardMovingLeft = true;
        //float boardChangeTime = 0.0f;
        float boardChangeMaxTime = 2.0f;
        bool isPlayerFacingLeft = true;
        public int boardChangeType = 10;
        private bool isDestroying = false;

        private bool bootingUpHowToPlay = true;
        private float bootUpTime = 0.0f;
        const float totalBoolUpTime = 1.0f;
        const int totalPanels = 7;

        Dictionary<int ,Entity> howToNum = new Dictionary<int, Entity>();
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
        Entity actlObj1 = null;
        Entity actlObj2 = null;
        int aniProgress = 0;
        Vector2 someDir;
        Vector2 pos;
        int[] tutorial2Checki;
        bool[] tutorial2Checkb;

        bool isClicked = false;
        float clickTime = 2.0f;

        public override void OnCreate()
        {
            Entity panelExists = FindEntityWithName("HowToStep0");
            if (panelExists != null)
            {
                howToNum.Add(currentPanel, panelExists);
                panelExists.As<UISliding>().ChangeSlideDetails(boardChangeType, new Vector2(2880.0f, 540.0f), new Vector2(960.0f, 540.0f), boardChangeMaxTime);
            }
            else
                howToNum.Add(currentPanel, CreateGameObject("HowToStep0"));

            tutorial2Checkb = new bool[3];
            tutorial2Checki = new int[6];

            bootUpTime = 0.0f;
            bootingUpHowToPlay = true;
        }

        public void DestroyLaterAndJustHide()
        {
            //nextBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //playBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //homeBtn.GetComponent<Renderer>().SetAlpha(0.0f);

            Entity slideOutE;
            if (howToNum.TryGetValue(currentPanel, out slideOutE))
            {
                Vector2 p = slideOutE.LocalPosition;
                slideOutE.As<UISliding>().ChangeSlideDetails(boardChangeType, p, new Vector2(-960.0f, 540.0f), totalBoolUpTime);
            }
            bootUpTime = totalBoolUpTime;
            isDestroying = true;
        }

        private string GetPanelName(int pan)
        {
            if (pan < totalPanels)
                return "Tutorial_Spritesheet_How " + pan;
            return "Tutorial_Spritesheet_How 0";
        }

        public void ProgressScene(int num)
        {
            if (currentPanel + num >= totalPanels && currentPanel == totalPanels - 1)
                return;

            oldPanel = currentPanel;
            boardMovingLeft = (num > 0);
            Entity slideOutE;
            if (howToNum.TryGetValue(oldPanel, out slideOutE))
            {
                Vector2 p = slideOutE.LocalPosition;
                if (boardMovingLeft)
                    slideOutE.As<UISliding>().ChangeSlideDetails(boardChangeType, p, new Vector2(-960.0f, 540.0f), boardChangeMaxTime);
                else
                    slideOutE.As<UISliding>().ChangeSlideDetails(boardChangeType, p, new Vector2(3840.0f, 540.0f), boardChangeMaxTime);
            }
            // Set buttons visibility if any
            //if (oldPanel == 0)
            //{
            //    backBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            //}
            //else if (oldPanel == totalPanels - 1)
            //{
            //    nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            //    playBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //    homeBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //}

            // Advance Panel
            currentPanel += num;
            //boardChangeTime = 0.0f;

            // If Overshot, end
            if (currentPanel >= totalPanels)
            {
                //Destroy();
                return;
            }
            // Return to first panel
            //if (currentPanel <= 0)
            //{
            //    nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            //    backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //    currentPanel = 0;
            //}
            //// Last Panel
            //else if (currentPanel == totalPanels - 1)
            //{
            //    nextBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            //}
            // Create Objs for every panel
            Entity currPE;
            if(howToNum.TryGetValue(currentPanel, out currPE))
            {
                if (boardMovingLeft)
                    currPE.As<UISliding>().ChangeSlideDetails(boardChangeType, currPE.LocalPosition, new Vector2(960.0f, 540.0f), boardChangeMaxTime);
                else
                    currPE.As<UISliding>().ChangeSlideDetails(boardChangeType, currPE.LocalPosition, new Vector2(960.0f, 540.0f), boardChangeMaxTime);
            }
            else
            {
                Entity panelExists = FindEntityWithName("HowToStep" + currentPanel);
                if (panelExists != null)
                {
                    howToNum.Add(currentPanel, panelExists);
                    panelExists.As<UISliding>().ChangeSlideDetails(boardChangeType, new Vector2(3840, 540), new Vector2(960.0f, 540.0f), boardChangeMaxTime);
                }
                else
                    howToNum.Add(currentPanel, CreateGameObject("HowToStep" + currentPanel));
            }

            // Set uniforms
            isPlayerFacingLeft = true;
            isClicked = false;
            frameChanged = 2;
            aniProgress = 0;
        }

        public override void OnUpdate(float dt)
        {
            if (isDestroying)
            {
                bootUpTime -= dt;
                GetComponentInChildren<Renderer>().SetAlpha(bootUpTime / totalBoolUpTime * 0.29411765933036806f);
                if (bootUpTime < 0)
                {
                    bootingUpHowToPlay = false;
                    bootUpTime = 0.0f;
                    Destroy();
                }
                return;
            }

            // Whole purpose to allow for creation of things before getting entites
            if (frameChanged > 0)
            {
                if (frameChanged == 1)
                {
                    //if(nextBtn == null)
                    //    nextBtn = FindEntityWithName("HowToRightBtn");
                    //if(backBtn == null)
                    //    backBtn = FindEntityWithName("HowToLeftBtn");
                    //if(playBtn == null)
                    //    playBtn = FindEntityWithName("HowToPlayBtn");
                    //if(homeBtn == null)
                    //    homeBtn = FindEntityWithName("HowToHomeBtn");

                    if(!howToNum.ContainsKey(currentPanel))
                    {
                        Entity curPanel = FindEntityWithName("HowToStep" + currentPanel);
                        if (curPanel != null)
                            howToNum.Add(currentPanel, curPanel);
                    }

                    // Reset the progress of the prev. panels
                    switch(oldPanel)
                    {
                        case 3:
                            cursor.Position = new Vector2(850, 0);
                            power1Ico.Position = new Vector2(450, 135);
                            power2Ico.Position = new Vector2(450, 80);
                            power2Ico.GetComponent<Renderer>().ChangeTexture("UI2_Shooter_Icon 0");
                            actlObj1.GetComponent<Renderer>().SetAlpha(0.0f);
                            actlObj1.Depth = 52;
                            actlObj1.GetComponent<Animation>().ChangeAnim("CandyCone_Idle");
                            actlObj2.GetComponent<Renderer>().SetAlpha(0.0f);
                            actlObj2.GetComponent<Animation>().ChangeAnim("Shooter_Idle");
                            enemyMouse1.GetComponent<Renderer>().SetAlpha(0.0f);
                            enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Climb_Blue");
                            enemyBear.GetComponent<Renderer>().SetAlpha(0.0f);
                            enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Climb");
                            enemyMouse1.Position = new Vector2(105, -400);
                            enemyBear.Position = new Vector2(105, -400);
                            break;
                        case 4:
                            playerWalk.LocalPosition = new Vector2(-160.0f, 105.0f);
                            playerWalk.Scale = new Vector2(0.666f, 0.666f);
                            actlObj1.GetComponent<Renderer>().SetAlpha(0.0f);
                            power2Ico.GetComponent<Animation>().ChangeAnim("Shooter_Mana");
                            playerWalk.GetComponent<Animation>().ChangeAnim("MC_Idle");
                            power1Ico.GetComponent<Renderer>().ChangeTexture("UI2_Mage_Icon 0");
                            power1Ico.LocalPosition = new Vector2(100.0f, 450.0f);
                            power1Ico.Scale = new Vector2(1, 1);
                            power1Ico.Depth = 52;
                            cursor.LocalPosition = new Vector2(850, 0);
                            break;
                        case 5:
                            actlObj1.LocalPosition = new Vector2(0, 1000);
                            actlObj1.GetComponent<Animation>().ChangeAnim("CakeStrawberry_Fall");
                            power1Ico.GetComponent<Renderer>().SetAlpha(0);
                            power1Ico.Depth = 52;
                            break;
                    }

                    // Just re-get, it's easier ;w;
                    switch (currentPanel)
                    {
                        case 0:
                            playerWalk = FindEntityWithName("HowToPlayerWalk");
                            break;
                        case 1:
                            enemyMouse1 = FindEntityWithName("HowToDemoShooter");
                            enemyMouse2 = FindEntityWithName("HowToDemoMage");
                            enemyBear = FindEntityWithName("HowToDemoSpear");
                            power1Ico = FindEntityWithName("HowToDemoTrap");
                            power2Ico = FindEntityWithName("HowToDemoHoney");
                            actlObj1 = FindEntityWithName("HowToDemoExplosion");

                            tutorial2Checki[3] = enemyMouse1.GetComponent<Animation>().GetFrameNo();
                            tutorial2Checki[4] = enemyMouse2.GetComponent<Animation>().GetFrameNo();
                            tutorial2Checki[5] = enemyBear.GetComponent<Animation>().GetFrameNo();
                            break;
                        case 2:
                            enemyMouse1 = FindEntityWithName("HowToMice1");
                            enemyMouse2 = FindEntityWithName("HowToMice2");
                            enemyBear = FindEntityWithName("HowToBear");
                            mouse1Climbing = mouse2Climbing = bearClimbing = true;
                            break;
                        case 3:
                            cursor = FindEntityWithName("HowToCursorM");
                            power1Ico = FindEntityWithName("HowToTrapIco");
                            power2Ico = FindEntityWithName("HowToShootIco");
                            enemyMouse1 = FindEntityWithName("HowToHiddenMice");
                            enemyBear = FindEntityWithName("HowToHiddenBear");
                            enemyMouse2 = FindEntityWithName("HowToBullet");
                            actlObj1 = FindEntityWithName("HowToActualTrap");
                            actlObj2 = FindEntityWithName("HowToActualShooter");
                            break;
                        case 4:
                            playerWalk = FindEntityWithName("HowToPrincess");
                            cursor = FindEntityWithName("HowToCursor");
                            power1Ico = FindEntityWithName("HowToDrop");
                            power2Ico = FindEntityWithName("HowToShooter");
                            actlObj1 = FindEntityWithName("HowToMage");
                            break;
                        case 5:
                            actlObj1 = FindEntityWithName("HowToCakeFalling");
                            power1Ico = FindEntityWithName("HowToCakeFallVFXBack");
                            mouse1Climbing = false;
                            mouse2Climbing = true;
                            if (FunctionCalls.GetSceneName() != "Scene3")
                            {
                                FindEntityWithName("HowToFakeTutorialTop").GetComponent<Renderer>().ChangeTexture("UI2_Tutorial_Hide3 0");
                            }
                            break;
                        case 6:
                            if (FunctionCalls.GetSceneName() != "Scene3")
                            {
                                FindEntityWithName("HowToHomeBtn").GetComponent<Renderer>().SetAlpha(0.0f);
                                FindEntityWithName("HowToPlayBtn").GetComponent<Renderer>().SetAlpha(0.0f);
                            }
                            break;
                    }
                }
                --frameChanged;
                return;
            }

            if(bootingUpHowToPlay)
            {
                bootUpTime += dt;
                GetComponentInChildren<Renderer>().SetAlpha(bootUpTime / totalBoolUpTime * 0.29411765933036806f);
                if(bootUpTime > totalBoolUpTime)
                {
                    bootingUpHowToPlay = false;
                    bootUpTime = 0.0f;
                    GetComponentInChildren<Renderer>().SetAlpha(0.29411765933036806f);
                }
            }

            //if(boardChangeTime < boardChangeMaxTime)
            //{
            //    boardChangeTime += dt;
            //    if(boardChangeTime > boardChangeMaxTime)
            //    {
            //        if (currentPanel == totalPanels - 1)
            //        {
            //            playBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            //            homeBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            //        }
            //    }
            //    return;
            //}

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

                    tutorial2Checki[3] = enemyMouse1.GetComponent<Animation>().GetFrameNo();
                    tutorial2Checki[4] = enemyMouse2.GetComponent<Animation>().GetFrameNo();
                    tutorial2Checki[5] = enemyBear.GetComponent<Animation>().GetFrameNo();

                    if (enemyMouse1.GetComponent<Animation>().IsAnimFinished() || 
                        (tutorial2Checki[0] != 0 && tutorial2Checki[3] == 0))
                    {
                        if (tutorial2Checkb[0])
                            enemyMouse1.GetComponent<Animation>().ChangeAnim("Shooter_Idle");
                        else
                            enemyMouse1.GetComponent<Animation>().ChangeAnim("Shooter_Shoot");
                        tutorial2Checkb[0] = !tutorial2Checkb[0];
                        tutorial2Checki[3] = 0;
                    }
                    if (enemyMouse2.GetComponent<Animation>().IsAnimFinished() ||
                        (tutorial2Checki[1] != 0 && tutorial2Checki[4] == 0))
                    {
                        if (tutorial2Checkb[1])
                            enemyMouse2.GetComponent<Animation>().ChangeAnim("Mage_Idle");
                        else
                            enemyMouse2.GetComponent<Animation>().ChangeAnim("Mage_Attack");
                        tutorial2Checkb[1] = !tutorial2Checkb[1];
                        tutorial2Checki[4] = 0;
                    }
                    if (enemyBear.GetComponent<Animation>().IsAnimFinished() ||
                        (tutorial2Checki[2] != 0 && tutorial2Checki[5] == 0))
                    {
                        if (tutorial2Checkb[2])
                            enemyBear.GetComponent<Animation>().ChangeAnim("Spear_Idle");
                        else
                            enemyBear.GetComponent<Animation>().ChangeAnim("Spear_Throw");
                        tutorial2Checkb[2] = !tutorial2Checkb[2];
                        tutorial2Checki[5] = 0;
                    }
                    if (power1Ico.GetComponent<Animation>().IsAnimFinished())
                        power1Ico.GetComponent<Animation>().ChangeAnim("CandyCone");
                    if (power2Ico.GetComponent<Animation>().IsAnimFinished())
                        power2Ico.GetComponent<Animation>().ChangeAnim("TrapHoneyEnter");
                    if (actlObj1.GetComponent<Animation>().IsAnimFinished())
                        actlObj1.GetComponent<Animation>().ChangeAnim("Jellybean_Explode");

                    for (int i = 0; i < 3; ++i)
                        tutorial2Checki[i] = tutorial2Checki[3 + i];
                    break;
                case 2:
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
                        enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Dissolve_Blue");
                        mouse1Climbing = false;
                    }
                    if (mouse2Climbing && enemyMouse2.LocalPosition.y > 0)
                    {
                        enemyMouse2.GetComponent<Animation>().ChangeAnim("Mouse_Dissolve_Brown");
                        mouse2Climbing = false;
                    }
                    if (bearClimbing && enemyBear.LocalPosition.y > -60)
                    {
                        enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Dissolve_Death");
                        bearClimbing = false;
                    }

                    if(!isClicked && !mouse2Climbing && enemyMouse2.GetComponent<Animation>().IsAnimFinished())
                    {
                        isClicked = true;
                        clickTime = 1;
                    }
                    if (isClicked && ((clickTime -= dt) < 0))
                    {
                        enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Climb_Blue");
                        enemyMouse2.GetComponent<Animation>().ChangeAnim("Mouse_Climb_Brown");
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
                        isClicked = false;
                    }
                    break;
                case 3:
                    
                    if(isClicked)
                    {
                        clickTime -= dt;
                        if (clickTime < 0)
                            isClicked = false;
                        break;
                    }

                    switch (aniProgress)
                    {
                        case 0:
                            someDir = (new Vector2(465, 115)) - cursor.LocalPosition;
                            someDir = someDir.Normalize();
                            ++aniProgress;
                            break;
                        // Cursor moves to 465, 115 (trap)
                        case 1:
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if (pos.x < 465)
                            {
                                someDir = (new Vector2(120, -200)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                actlObj1.GetComponent<Renderer>().SetAlpha(0.3f);
                                power1Ico.Scale = new Vector2(0.666f, 0.666f);
                                ++aniProgress;
                                isClicked = true;
                                clickTime = 1;
                            }
                            break;
                        // Cursor moves to 120, -200 (place trap)
                        case 2:
                            pos = power1Ico.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            power1Ico.LocalPosition = pos;
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if (pos.x < 120)
                            {
                                actlObj1.GetComponent<Renderer>().SetAlpha(1.0f);
                                power1Ico.Scale = new Vector2(0.55f, 0.55f);
                                power1Ico.Position = new Vector2(2000, 135);

                                someDir = (new Vector2(465, 40)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();

                                ++aniProgress;
                                isPlayerFacingLeft = true;
                                isClicked = true;
                                clickTime = 0.5f;
                            }
                            break;
                        case 3:
                            if (isPlayerFacingLeft)
                            {
                                enemyMouse1.GetComponent<Renderer>().SetAlpha(1.0f);
                                isPlayerFacingLeft = false;
                            }

                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            pos = enemyMouse1.LocalPosition;
                            pos.y += dt * 150.0f;
                            enemyMouse1.LocalPosition = pos;
                            if (pos.y > -270)
                            {
                                enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Death_Blue");
                                actlObj1.GetComponent<Animation>().ChangeAnim("CandyCone");
                                ++aniProgress;
                            }
                            break;
                        // Cursor moves to 465, 40 (Shooter)
                        case 4:
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;

                            if (pos.x > 465)
                            {
                                someDir = (new Vector2(65, 20)) - cursor.LocalPosition;
                                someDir = someDir.Normalize();
                                actlObj2.GetComponent<Renderer>().SetAlpha(0.3f);
                                power2Ico.Scale = new Vector2(-0.666f, 0.666f);
                                power2Ico.GetComponent<Renderer>().ChangeTexture("NPC_SpriteSheet_Shooter_Idle 0");

                                ++aniProgress;
                                isClicked = true;
                                clickTime = 1;
                            }
                            break;
                        case 5:

                            pos = power2Ico.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            power2Ico.LocalPosition = pos;
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if (pos.x < 65)
                            {
                                actlObj2.GetComponent<Renderer>().SetAlpha(1.0f);
                                power2Ico.Scale = new Vector2(1.333f, 1.333f);
                                power2Ico.Position = new Vector2(2000, 80);
                                power2Ico.GetComponent<Renderer>().ChangeTexture("UI2_Shooter_Icon 0");
                                enemyBear.GetComponent<Renderer>().SetAlpha(1.0f);
                                ++aniProgress;
                            }
                            break;
                        case 6:
                            pos = enemyBear.LocalPosition;
                            pos.y += dt * 100.0f;
                            enemyBear.LocalPosition = pos;
                            if (pos.y > -270)
                            {
                                actlObj1.GetComponent<Animation>().ChangeAnim("Dissolve");
                                actlObj1.Depth = 56;
                                actlObj2.GetComponent<Animation>().ChangeAnim("Shooter_Shoot");
                                isPlayerFacingLeft = true;
                                ++aniProgress;
                            }
                            break;
                        case 7:
                            pos = enemyBear.LocalPosition;
                            pos.y += dt * 100.0f;
                            enemyBear.LocalPosition = pos;

                            if (isPlayerFacingLeft)
                            {
                                if (actlObj2.GetComponent<Animation>().GetFrameNo() >= 7)
                                {
                                    enemyMouse2.GetComponent<Renderer>().SetAlpha(1.0f);
                                    enemyMouse2.GetComponent<Animation>().ChangeAnim("Shooter_Projectile");
                                    isPlayerFacingLeft = false;
                                }
                            }
                            else
                            {
                                Vector2 pos2 = enemyMouse2.LocalPosition;
                                pos2.y -= dt * 100.0f;
                                pos2.x += dt * 40.0f;
                                enemyMouse2.LocalPosition = pos2;
                                if (pos2.y < pos.y)
                                {
                                    enemyMouse2.GetComponent<Renderer>().SetAlpha(0.0f);
                                    enemyMouse2.Position = new Vector2(75, -20);
                                    enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Death");
                                    ++aniProgress;
                                }
                            }
                            break;
                        case 8:
                            if (enemyBear.GetComponent<Animation>().IsAnimFinished())
                            {
                                cursor.Position = new Vector2(850, 0);
                                power1Ico.Position = new Vector2(450, 135);
                                power2Ico.Position = new Vector2(450, 80);
                                actlObj1.GetComponent<Renderer>().SetAlpha(0.0f);
                                actlObj1.Depth = 52;
                                actlObj1.GetComponent<Animation>().ChangeAnim("CandyCone_Idle");
                                actlObj2.GetComponent<Renderer>().SetAlpha(0.0f);
                                actlObj2.GetComponent<Animation>().ChangeAnim("Shooter_Idle");
                                enemyMouse1.GetComponent<Renderer>().SetAlpha(0.0f);
                                enemyMouse1.GetComponent<Animation>().ChangeAnim("Mouse_Climb_Blue");
                                enemyBear.GetComponent<Renderer>().SetAlpha(0.0f);
                                enemyBear.GetComponent<Animation>().ChangeAnim("Bear_Climb");
                                enemyMouse1.Position = new Vector2(105, -400);
                                enemyBear.Position = new Vector2(105, -400);

                                aniProgress = 0;
                            }
                            break;
                    }
                    
                    // Cursor moves to 65, 20 (place shooter)
                    // Haves: Shooter (-135, 110) (0.666 x 0.666)

                    break;
                case 4:
                    if (isClicked)
                    {
                        clickTime -= dt;
                        if (clickTime < 0.0f)
                            isClicked = false;
                        break;
                    }

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

                                power1Ico.LocalPosition = new Vector2(450.0f, 140.0f);
                                power1Ico.Scale = new Vector2(0.01f, 0.01f);

                                someDir = (new Vector2(460.0f, 140.0f)) - cursor.LocalPosition;
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
                                power1Ico.Depth = 54;
                                actlObj1.GetComponent<Renderer>().SetAlpha(0.3f);
                                ++aniProgress;
                                isClicked = true;
                                clickTime = 1;
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
                                power1Ico.GetComponent<Renderer>().ChangeTexture("UI2_Mage_Icon 0");
                                power1Ico.LocalPosition = new Vector2(100.0f, 450.0f);
                                power1Ico.Depth = 52;
                                actlObj1.GetComponent<Renderer>().SetAlpha(1.0f);
                                isClicked = true;
                                clickTime = 2.0f;
                                ++aniProgress;
                            }
                            break;
                    // Mouse moves to 180, 125 (heal shooter (shooter located (40,110) -0.66,0.66 scale)
                        case 4:
                            pos = cursor.LocalPosition;
                            pos.x += someDir.x * dt * 200.0f;
                            pos.y += someDir.y * dt * 200.0f;
                            cursor.LocalPosition = pos;
                            if(pos.x > 180.0f)
                            {
                                isPlayerFacingLeft = true;
                                playerWalk.GetComponent<Animation>().ChangeAnim("MC_Heal");
                                Vector2 pPos = playerWalk.LocalPosition;
                                pPos.x += 24;
                                playerWalk.LocalPosition = pPos;
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
                                    Vector2 pPos = playerWalk.LocalPosition;
                                    pPos.x -= 24;
                                    playerWalk.LocalPosition = pPos;
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
                                    actlObj1.GetComponent<Renderer>().SetAlpha(0.0f);
                                    power2Ico.GetComponent<Animation>().ChangeAnim("Shooter_Mana");
                                    power1Ico.Scale = new Vector2(1, 1);
                                    cursor.LocalPosition = new Vector2(850, 0);
                                    isPlayerFacingLeft = true;
                                    aniProgress = 0;
                                }
                            }
                            break;
                    }
                    // Mouse moves back to 1'000, 100
                    break;
                case 5:
                    // y == 660
                    // y == 480
                    switch(aniProgress)
                    {
                        case 0:
                            Vector2 fallPos = actlObj1.LocalPosition;
                            fallPos.y -= 700.0f * dt;
                            if(!mouse1Climbing && fallPos.y < 660)
                            {
                                power1Ico.Depth = 52;
                                power1Ico.GetComponent<Renderer>().SetAlpha(1.0f);
                                power1Ico.GetComponent<Animation>().ChangeAnim("CakeFallVFx");
                                mouse1Climbing = true;
                                mouse2Climbing = false;
                            }
                            if (!mouse2Climbing && power1Ico.GetComponent<Animation>().IsAnimFinished())
                            {
                                power1Ico.Depth = 54;
                                power1Ico.GetComponent<Animation>().ChangeAnim("CakeFallVFxEnd");
                                mouse2Climbing = true;
                            }
                            if(fallPos.y < 480)
                            {
                                fallPos.y = 480.0f;
                                actlObj1.GetComponent<Animation>().ChangeAnim("CakeStrawberry_Squish");
                                clickTime = 1.5f;
                                ++aniProgress;
                            }
                            actlObj1.LocalPosition = fallPos;
                            break;
                        case 1:
                            if (!mouse2Climbing && power1Ico.GetComponent<Animation>().IsAnimFinished())
                            {
                                power1Ico.Depth = 54;
                                power1Ico.GetComponent<Animation>().ChangeAnim("CakeFallVFxEnd");
                                mouse2Climbing = true;
                            }

                            clickTime -= dt;
                            if (clickTime < 0)
                                ++aniProgress;
                            break;
                        case 2:
                            actlObj1.LocalPosition = new Vector2(0, 1000);
                            actlObj1.GetComponent<Animation>().ChangeAnim("CakeStrawberry_Fall");
                            power1Ico.GetComponent<Renderer>().SetAlpha(0);
                            power1Ico.Depth = 52;
                            mouse1Climbing = false;
                            mouse2Climbing = true;
                            aniProgress = 0;
                            break;
                    }
                    break;
            }
        }
    }
}

using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TutorialController : Entity
    {
        private Entity enemy;
        private Entity[] powerIco = new Entity[4];// Reset 1 to (0, 240)
        private Entity[] traps = new Entity[4];
        private Entity[] npcs = new Entity[4];
        private bool[] placedObj = new bool[8];
        private Entity[] ditto = new Entity[2];   // can morph into WORLDSPACE trap / npc
        private Entity player;
        private Entity pauseManager;
        private Entity cam;
        private Entity endScreen;

        private int tutorialProgress = 0;
        // For loading certain things only 2 frames later igaf
        private int readyToLoad = 2;

        // For specifically delay effect
        private float delayTimer = 0.0f;

        // Text Things
        private Entity textObj, txtChild;
        private string currText = "";
        private int currPhraseProgress = 0, phraseLength = 0, whichPhrase = -1;
        private float wordsTimer = 0.0f, charSpeed = 0.05f;
        const int numaPhrases = 19;
        private string[] bunchaWords = new string[numaPhrases]{ 
                                        "Welcome to Sugar Strike!",
                                        "I am your guide and in this tutorial, enemies will only spawn on the right side",
                                        "If you want to skip this at any time press 5 :3",
                                        "Oop, here come a critter!",// Pause, Pans down onto the creature
                                        "Use R/F to pan the camera back up, we need to prepare",
                                        "Here, I'll provide you a trap to fend it off",                     // ----- 5 -----
                                        "The trap is on the menu on the right --->",                        // Optional txt
                                        "That's the wrong spot, here's another trap to try",                // Optional txt
                                        "You are not going to be able to keep these FYI",                   // Optional txt
                                        "This trap can only take 2 hits total before it is used up",
                                        "Perfect timing, reinforcements from above, use WASD to move and pick it up", // 4 reinforcements // ----- 10 -----
                                        "This is a Soldier unit, you can only place them on top of the tower",
                                        "They have limited number of attacks, but can be healed up by clicking on them",
                                        "Units are also placed on the right --->",                        // Optional txt
                                        "Yeap, just one more, I'll give you another one for free",
                                        "They usually start off healthy, but for this tutorial, I've drained their energy", // ----- 15 -----
                                        "You can click the Soldier to refuel them",
                                        "Be careful to not lose your Soldiers to random enemies",
                                        "Tutorial Ends here.. Good luck!",              // Dun matter win / lose
                                        };

        // Pan Effects
        private Entity[] entityToPan = new Entity[8];
        private Vector2[] panSpeed = new Vector2[8];
        private Vector2[] panEndGoal = new Vector2[8];
        private bool[] entityPanFinished = new bool[8];
        // Heal
        private bool healDone = true;
        private int healNPC;
        // Variables to use
        private bool[]  someBools = new bool[4];
        private Vector2[] someVec = new Vector2[2];
        private float[] someFloat = new float[2];
        private int     someInt;


        public override void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null && pauseManager.As<PauseManager>().IsPaused)
            {
                return;
            }

            //dt *= 4;

            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                tutorialProgress = -1;
            }

            // My Ver. of On Create, because I need garuntee the things are loaded in before I get them
            if(readyToLoad > 0)
            {
                if(readyToLoad == 1)
                {
                    Sound.PlayBGM("BGM_Tutorial_Mix1", 0.4f);
                    cam = FindEntityWithName("MainCamera");
                    enemy = FindEntityWithName("Tutorial_Enemy");
                    player = FindEntityWithName("MC");
                    endScreen = FindEntityWithName("EndScreen");
                    textObj = FindEntityWithName("SubText");
                    pauseManager = FindEntityWithName("PauseManager");
                    txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));

                    Entity UIBar = FindEntityWithName("UIBar");
                    {
                        Entity[] UIBarChild = UIBar.GetAllChildren();
                        int i = 0;
                        foreach (Entity child in UIBarChild)
                        {
                            if (i < 4)
                                powerIco[i++] = child;
                        }
                    }
                    UIBar = FindEntityWithName("Tutorial_HaveNots");
                    {
                        Entity[] UIBarChild = UIBar.GetAllChildren();
                        int i = 0;
                        foreach (Entity child in UIBarChild)
                        {
                            if (i < 4)
                                traps[i++] = child;
                            else
                                npcs[i++ - 4] = child;
                        }
                    }
                    UIBar = FindEntityWithName("Tutorial_Dittos");
                    {
                        Entity[] UIBarChild = UIBar.GetAllChildren();
                        int i = 0;
                        foreach (Entity child in UIBarChild)
                        {
                            if (i < 2)
                                ditto[i++] = child;
                        }
                    }
                    for(int i = 0; i < 5; ++i)
                    {
                        entityPanFinished[i] = true;
                    }

                }
                --readyToLoad;
                return;
            }


            player.Depth = 14 - player.LocalPosition.y;

            switch (tutorialProgress)
            {
                case 0:
                    TypeWords(dt, 0);
                    if (FinishedTyping() && Delay(dt, 1.0f))
                        ++tutorialProgress;
                    break;
                case 1:
                    TypeWords(dt, 1);
                    if (FinishedTyping() && Delay(dt, 1.0f))
                        ++tutorialProgress;
                    break;
                case 2:
                    TypeWords(dt, 2);
                    if (FinishedTyping() && Delay(dt, 1.0f))
                        ++tutorialProgress;
                    break;
                case 3:
                    TypeWords(dt, 3);
                    if(FinishedTyping())
                    {
                        enemy.LocalPosition = new Vector2(6.5f, -15.3f);
                        SetPanDetails(cam, new Vector2(0, -7.6f), 20, 0);
                        ++tutorialProgress;
                    }
                    break;
                case 4:
                    if (Panning(dt, 0))
                    {
                        TypeWords(dt, 4);
                        if (cam.LocalPosition.y > 2)
                            ++tutorialProgress;
                    }
                    break;
                case 5:
                    TypeWords(dt, 5);
                    if(FinishedTyping())
                    {
                        powerIco[0].GetComponent<Renderer>().SetAlpha(1f);
                        powerIco[0].Scale = Vector2.Zero;
                        someBools[1] = false;
                        someBools[2] = false;
                        someBools[3] = false;
                        someFloat[0] = 0.0f;
                        someFloat[1] = 0.0f;
                        someInt = 5;
                        ++tutorialProgress;
                    }
                    break;
                case 6:
                    // someBools[0] - To scale up / down
                    // someBools[1] - If click & drag
                    // someBools[2] - If need to flip the held obj or no
                    // someBools[3] - Has the player clicked the trap
                    // someFloat[0] - The scaling
                    // someFloat[1] - In Case the user nvr clicked the trap
                    // someInt      - progress of txt

                    TypeWords(dt, someInt);

                    if (!someBools[3])
                    {
                        someFloat[1] += dt;
                        if (someFloat[1] > 7.5f)
                            someInt = 6;
                    }
                    // First Half - to click the ico
                    if (!someBools[1])
                    {
                        // Not Hover
                        if (!(powerIco[0].As<TutorialBasic>().GetHover()))
                        {
                            if (someBools[0])
                            {
                                someFloat[0] += 0.3f * dt;
                                if (someFloat[0] > 0.7f)
                                    someBools[0] = false;
                            }
                            else
                            {
                                someFloat[0] -= 0.3f * dt;
                                if (someFloat[0] < 0.45f)
                                    someBools[0] = true;

                            }
                            powerIco[0].Scale = new Vector2(someFloat[0], someFloat[0]);
                            powerIco[0].LocalPosition = new Vector2(someFloat[0] * 83.33f, 232);
                            if (powerIco[0].As<TutorialBasic>().GetExitHover())
                                powerIco[0].GetComponent<Renderer>().SetColour(1f, 1f, 1f);
                        }
                        else
                        {
                            if (powerIco[0].As<TutorialBasic>().GetEnterHover())
                                Sound.PlaySFX("Item_Hover", 0.3f);
                                powerIco[0].GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);
                            if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
                            {
                                Sound.PlaySFX("UI_Select 2", 0.3f);
                                powerIco[0].GetComponent<Renderer>().SetColour(1f, 1f, 1f);
                                powerIco[0].GetComponent<Renderer>().SetAlpha(0.0f);
                                powerIco[0].Scale = new Vector2(0.6f, 0.6f);
                                ditto[0].GetComponent<Renderer>().ChangeTexture("candy_trap_spritesheet_Spike 9");
                                ditto[0].Scale = new Vector2(0.6f, 0.6f);

                                for (int i = 0; i < 4; ++i)
                                {
                                    if (placedObj[i])
                                        continue;

                                    traps[i].GetComponent<Renderer>().SetAlpha(0.3f);
                                }

                                someBools[1] = true;
                                someBools[2] = false;
                                someBools[3] = true;
                            }
                        }
                    }
                    // Second Half to drag the ico
                    else
                    {
                        //Vector2 mousePos = Input.GetMousePos();

                        //powerIco[0].LocalPosition += mousePos - someVec;
                        //someVec = mousePos;
                        ditto[0].LocalPosition = Input.GetMousePos();

                        if (!someBools[2] && ditto[0].LocalPosition.x < 0)
                        {
                            ditto[0].Scale = new Vector2(-0.6f, 0.6f);
                            someBools[2] = true;
                        }
                        else if (someBools[2] && ditto[0].LocalPosition.x > 0)
                        {
                            ditto[0].Scale = new Vector2(0.6f, 0.6f);
                            someBools[2] = false;
                        }

                        if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
                        {
                            int justPlaced = -1;

                            // Check loop for place obj
                            for (int i = 0; i < 4; ++i)
                            {
                                if (placedObj[i])
                                    continue;

                                Vector2 dim;
                                FunctionCalls.Transform_GetRenderingScale(traps[i].mID, out dim);
                                dim.x = dim.x * Math.Abs(traps[i].Scale.x) / 4.0f;
                                dim.y = dim.y * traps[i].Scale.y / 2.0f;

                                if (i < 2)
                                {
                                    if (ditto[0].LocalPosition.x < traps[i].LocalPosition.x + 2 * dim.x &&
                                        ditto[0].LocalPosition.x > traps[i].LocalPosition.x &&
                                        ditto[0].LocalPosition.y < traps[i].LocalPosition.y + dim.y &&
                                        ditto[0].LocalPosition.y > traps[i].LocalPosition.y - dim.y)
                                        justPlaced = i;
                                }
                                else if (ditto[0].LocalPosition.x < traps[i].LocalPosition.x &&
                                        ditto[0].LocalPosition.x > traps[i].LocalPosition.x - 2 * dim.x &&
                                        ditto[0].LocalPosition.y < traps[i].LocalPosition.y + dim.y &&
                                        ditto[0].LocalPosition.y > traps[i].LocalPosition.y - dim.y)
                                    justPlaced = i;
                            }
                            // If placed a trap correctly
                            if (justPlaced != -1)
                            {
                                Sound.PlaySFX("Trap_Placement_v2", 0.3f);
                                placedObj[justPlaced] = true;
                                traps[justPlaced].GetComponent<Renderer>().SetAlpha(1.0f);

                                if (placedObj[3] && placedObj[2])
                                {
                                    Vector2 enemyPos = enemy.LocalPosition;
                                    enemyPos.y = 10.0f;
                                    SetPanDetails(enemy, enemyPos, 2, 0);

                                    Vector2 enemyScale, trapScale;
                                    FunctionCalls.Transform_GetRenderingScale(enemy.mID, out enemyScale);
                                    FunctionCalls.Transform_GetRenderingScale(traps[3].mID, out trapScale);

                                    someVec[0].x = enemyScale.y * enemy.Scale.y / 2.0f;
                                    someVec[0].y = trapScale.y / 2.0f;

                                    // Set falling ico location already
                                    someBools[0] = false;
                                    if (player.LocalPosition.x > 0)
                                        ditto[1].LocalPosition = new Vector2(-4, 30);
                                    else
                                        ditto[1].LocalPosition = new Vector2(4, 30);
                                    ++tutorialProgress;
                                }
                                else
                                {
                                    if (justPlaced < 2)
                                    {
                                        someFloat[0] = 0.0f;
                                        switch(someInt)
                                        {
                                            case 5:
                                            case 6:
                                                someInt = 7;
                                                break;
                                            case 7:
                                                someInt = 8;
                                                break;
                                            case 14:
                                                someInt = 8;
                                                break;
                                        }
                                    }
                                    else someInt = 14;
                                    powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                                }
                            }
                            // Failed to place
                            else
                                powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                            // Hide back all not placed traps
                            for (int i = 0; i < 4; ++i)
                            {
                                if (placedObj[i] == false)
                                    traps[i].GetComponent<Renderer>().SetAlpha(0.0f);
                            }
                            ditto[0].LocalPosition = new Vector2(100.0f, 0.0f);
                            someBools[1] = false;
                        }
                    }
                    break;
                case 7:
                    // SomeVec - (enemyDim.y/2, trapDim.y/2)
                    if (!someBools[0])
                    {
                        if (player.LocalPosition.x > 0)
                            SetPanDetails(ditto[1], new Vector2(-4, 6), 4, 1);
                        else
                            SetPanDetails(ditto[1], new Vector2(4, 6), 4, 1);
                        someBools[0] = true;
                    }

                    TypeWords(dt, 10);
                    Panning(dt, 0);
                    Panning(dt, 1);

                    if (enemy.LocalPosition.y + someVec[0].x > traps[3].LocalPosition.y)
                    {
                        SetPanDetails(cam, new Vector2(0, 10), 20, 0);
                        FunctionCalls.Transform_GetRenderingScale(ditto[1].mID, out someVec[0]);
                        FunctionCalls.Transform_GetRenderingScale(player.mID, out someVec[1]);

                        someVec[0].x *= Math.Abs(ditto[1].Scale.x) / 2.0f;
                        someVec[0].y *= ditto[1].Scale.y / 2.0f;
                        someVec[1].x *= Math.Abs(player.Scale.x) / 2.0f;
                        someVec[1].y *= player.Scale.y / 2.0f;

                        enemy.GetComponent<Animation>().ChangeAnim("Mouse_Death_Blue");
                        traps[3].GetComponent<Animation>().ChangeAnim("CandyCone");
                        ++tutorialProgress;
                    }
                    break;
                case 8:
                    TypeWords(dt, 11);
                    Panning(dt, 0);
                    Panning(dt, 1);

                    if (player.LocalPosition.x + someVec[1].x > ditto[1].LocalPosition.x - someVec[0].x &&
                    player.LocalPosition.x - someVec[1].x < ditto[1].LocalPosition.x + someVec[0].x &&
                    player.LocalPosition.y + someVec[1].y > ditto[1].LocalPosition.y - someVec[0].y &&
                    player.LocalPosition.y - someVec[1].y < ditto[1].LocalPosition.y + someVec[0].y)
                    {
                        powerIco[0].GetComponent<Renderer>().SetAlpha(1);
                        powerIco[0].GetComponent<Renderer>().ChangeTexture("UI2_Shooter_Icon 0");
                        powerIco[0].LocalPosition = new Vector2(0, 240);
                        powerIco[0].Scale = new Vector2(1.2f, 1.2f);
                        ditto[1].LocalPosition = new Vector2(100, 0);


                        FunctionCalls.Transform_GetRenderingScale(npcs[3].mID, out someVec[0]);

                        someBools[0] = someBools[1] = someBools[2] = false;
                        someFloat[0] = 0.0f;
                        someInt = 12;
                        ++tutorialProgress;
                    }

                    break;
                case 9:
                    // someBools[0] - If click & drag
                    // someBools[1] - If need to flip the held obj or no
                    // someBools[2] - Has the player clicked the trap
                    // someFloat[0] - In Case the user nvr clicked the trap
                    // someVec[0]   - dim of the npc
                    // someInt      - progress of txt

                    TypeWords(dt, someInt);
                    RecoverHeal();

                    // If player decids to heal beforehand ;w;
                    if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
                    {
                        Vector2 mousePos = Input.GetMousePos();
                        for (int i = 6; i < 8; ++i)
                        {
                            if (placedObj[i] && !entityPanFinished[i])
                            {
                                if (mousePos.x < npcs[i - 4].LocalPosition.x + panEndGoal[i].x &&
                                    mousePos.x > npcs[i - 4].LocalPosition.x - panEndGoal[i].x &&
                                    mousePos.y < npcs[i - 4].LocalPosition.y + panEndGoal[i].y &&
                                    mousePos.y > npcs[i - 4].LocalPosition.y - panEndGoal[i].y)
                                {
                                    if(Heal(i - 4))
                                        entityPanFinished[i] = true;
                                }
                            }
                        }
                    }

                    if ((someInt < 13 || someInt == 15) && FinishedTyping() && Delay(dt, 2.0f))
                        ++someInt;

                    if (!someBools[2] && someInt == 13 && FinishedTyping())
                    {
                        someFloat[0] += dt;
                        if (someFloat[0] > 7.5f)
                            someInt = 14;
                    }
                    // First Half - to click the ico
                    if (!someBools[0])
                    {
                        // Not Hover
                        if (!(powerIco[0].As<TutorialBasic>().GetHover()))
                        {
                            if (powerIco[0].As<TutorialBasic>().GetExitHover())
                                powerIco[0].GetComponent<Renderer>().SetColour(1f, 1f, 1f);
                        }
                        else
                        {
                            if (powerIco[0].As<TutorialBasic>().GetEnterHover())
                                Sound.PlaySFX("Item_Hover", 0.3f);
                                powerIco[0].GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);
                            if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
                            {
                                powerIco[0].GetComponent<Renderer>().SetColour(1f, 1f, 1f);
                                powerIco[0].GetComponent<Renderer>().SetAlpha(0.0f);
                                ditto[0].GetComponent<Renderer>().ChangeTexture("NPC_SpriteSheet_Shooter_Idle 0");
                                ditto[0].Scale = new Vector2(-0.45f, 0.45f);

                                for (int i = 0; i < 4; ++i)
                                {
                                    if (placedObj[i + 4])
                                        continue;

                                    npcs[i].GetComponent<Renderer>().SetAlpha(0.3f);
                                }

                                someBools[0] = true;
                                someBools[1] = false;
                                someBools[2] = true;
                            }
                        }
                    }
                    // Second Half to drag the ico
                    else
                    {
                        //Vector2 mousePos = Input.GetMousePos();

                        //powerIco[0].LocalPosition += mousePos - someVec;
                        //someVec = mousePos;
                        ditto[0].LocalPosition = Input.GetMousePos();

                        if (!someBools[1] && ditto[0].LocalPosition.x < 0)
                        {
                            ditto[0].Scale = new Vector2(0.45f, 0.45f);
                            someBools[1] = true;
                        }
                        else if (someBools[1] && ditto[0].LocalPosition.x > 0)
                        {
                            ditto[0].Scale = new Vector2(-0.45f, 0.45f);
                            someBools[1] = false;
                        }

                        if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
                        {
                            int justPlaced = -1;

                            // Check loop for place obj
                            for (int i = 0; i < 4; ++i)
                            {
                                if (placedObj[i + 4])
                                    continue;

                                Vector2 dim;
                                FunctionCalls.Transform_GetRenderingScale(npcs[i].mID, out dim);
                                dim.x = dim.x * Math.Abs(npcs[i].Scale.x) / 2.0f;
                                dim.y = dim.y * npcs[i].Scale.y / 2.0f;

                                if (ditto[0].LocalPosition.x < npcs[i].LocalPosition.x + dim.x &&
                                    ditto[0].LocalPosition.x > npcs[i].LocalPosition.x - dim.x &&
                                    ditto[0].LocalPosition.y < npcs[i].LocalPosition.y + dim.y &&
                                    ditto[0].LocalPosition.y > npcs[i].LocalPosition.y - dim.y)
                                    justPlaced = i;
                            }
                            // If placed a trap correctly
                            if (justPlaced != -1)
                            {
                                
                                placedObj[justPlaced + 4] = true;
                                npcs[justPlaced].GetComponent<Renderer>().SetAlpha(1.0f);

                                if (justPlaced > 1)
                                {
                                    someInt = 15;
                                    npcs[justPlaced].GetComponent<Animation>().ChangeAnim("Shooter_Mana");

                                    FunctionCalls.Transform_GetRenderingScale(npcs[justPlaced].mID, out panEndGoal[justPlaced + 4]);
                                    panEndGoal[justPlaced + 4].x = panEndGoal[justPlaced + 4].x * Math.Abs(npcs[justPlaced].Scale.x) / 2.0f;
                                    panEndGoal[justPlaced + 4].y = panEndGoal[justPlaced + 4].y * npcs[justPlaced].Scale.y / 2.0f;
                                    entityPanFinished[justPlaced + 4] = false;
                                }

                                if (placedObj[6] && placedObj[7])
                                {
                                    someInt = Math.Max(15, someInt);
                                    ++tutorialProgress;
                                }
                                else
                                    powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                            }
                            // Failed to place
                            else
                                powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                            // Hide back all not placed npcs
                            for (int i = 0; i < 4; ++i)
                            {
                                if (placedObj[i + 4] == false)
                                    npcs[i].GetComponent<Renderer>().SetAlpha(0.0f);
                            }
                            ditto[0].LocalPosition = new Vector2(100.0f, 0.0f);
                            someBools[0] = false;
                        }
                    }
                    break;
                case 10:
                    TypeWords(dt, someInt);
                    RecoverHeal();

                    if (someInt == 15 && FinishedTyping() && Delay(dt, 2.0f))
                        ++someInt;


                    if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
                    {
                        Vector2 mousePos = Input.GetMousePos();
                        for (int i = 6; i < 8; ++i)
                        {
                            if (placedObj[i] && !entityPanFinished[i])
                            {
                                if (mousePos.x < npcs[i - 4].LocalPosition.x + panEndGoal[i].x &&
                                    mousePos.x > npcs[i - 4].LocalPosition.x - panEndGoal[i].x &&
                                    mousePos.y < npcs[i - 4].LocalPosition.y + panEndGoal[i].y &&
                                    mousePos.y > npcs[i - 4].LocalPosition.y - panEndGoal[i].y)
                                {
                                    if(Heal(i - 4))
                                        entityPanFinished[i] = true;
                                }
                            }
                        }
                    }

                    if (entityPanFinished[6] && entityPanFinished[7] && healDone)
                    {
                        someInt = 17;
                        ++tutorialProgress;
                    }
                    break;
                case 11:
                    TypeWords(dt, someInt);

                    if (someInt == 17 && FinishedTyping() && Delay(dt, 2.0f))
                        ++someInt;

                    if (someInt == 18 && FinishedTyping() && Delay(dt, 2.0f))
                    {
                        endScreen.LocalPosition = new Vector2(960.0f, 540.0f);
                        ++tutorialProgress;
                    }
                    break;
                case 12:
                        
                    break;
                default:
                    Scene.ChangeScene("Loading");
                    break;
            }
            //Sound.PlaySFX("Item_Hover", 0.4f);
            //this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);

        }

        bool FinishedTyping()
        {
            return currPhraseProgress == phraseLength;
        }
        void TypeWords(float dt, int phraseNum)
        {
            if(whichPhrase != phraseNum)
            {
                if (phraseNum >= 0 && phraseNum < numaPhrases)
                {
                    phraseLength = bunchaWords[phraseNum].Length;
                    textObj.GetComponent<TextRenderer>().SetText("");
                    txtChild.Scale = new Vector2(0.0f, 1.0f);
                }
                else
                    phraseLength = 0;
                currPhraseProgress = 0;
                wordsTimer = 0;
                whichPhrase = phraseNum;
                currText = "";
            }

            bool dunChangeBGTxt = currText.Length == 0;

            if (currPhraseProgress < phraseLength)
            {
                wordsTimer += dt;
                bool txtChanged = false;
                while(wordsTimer > charSpeed && currPhraseProgress < phraseLength)
                {
                    currText += bunchaWords[whichPhrase][currPhraseProgress++];
                    wordsTimer -= charSpeed;
                    txtChanged = true;
                }
                if(txtChanged)
                {
                    textObj.GetComponent<TextRenderer>().SetText(currText);

                    if(!dunChangeBGTxt)
                        txtChild.Scale = new Vector2(textObj.GetComponent<TextRenderer>().GetWidth() / 47.0f, 1.0f);
                    else
                        txtChild.Scale = new Vector2(0.0f, 0.0f);
                }
            }
        }

        bool Delay(float dt, float delay)
        {
            delayTimer += dt;
            if (delayTimer > delay)
            {
                delayTimer = 0;
                return true;
            }
            return false;
        }

        void SetPanDetails(Entity entity, Vector2 endPos, float speed, int num)
        {
            entityToPan[num] = entity;
            panSpeed[num] = (endPos - entity.LocalPosition).Normalize();
            panSpeed[num].x *= speed;
            panSpeed[num].y *= speed;
            panEndGoal[num] = endPos;
            entityPanFinished[num] = false;
        }

        bool Panning(float dt, int num)
        {
            if (!entityPanFinished[num])
            {
                Vector2 pos = entityToPan[num].LocalPosition;
                pos.x += panSpeed[num].x * dt;
                pos.y += panSpeed[num].y * dt;
                entityToPan[num].LocalPosition = pos;
                if ((panSpeed[num].x < 0 && pos.x < panEndGoal[num].x) ||
                    (panSpeed[num].x > 0 && pos.x > panEndGoal[num].x) ||
                    (panSpeed[num].y < 0 && pos.y < panEndGoal[num].y) ||
                    (panSpeed[num].y > 0 && pos.y > panEndGoal[num].y))
                    entityPanFinished[num] = true;
            }
            return entityPanFinished[num];
        }

        bool Heal(int npcNum)
        {
            if (healDone && placedObj[npcNum + 4])
            {
                Sound.PlaySFX("Princess_Heal", 0.3f);
                healNPC = npcNum;
                pauseManager.As<PauseManager>().IsPaused = true;
                player.GetComponent<Animation>().ChangeAnim("MC_Heal");
                Vector2 playerPosEdit = player.LocalPosition;
                if(player.Scale.x > 0)
                    playerPosEdit.x -= 0.7f;
                else
                    playerPosEdit.x += 0.7f;
                player.LocalPosition = playerPosEdit;
                healDone = false;
                return true;
            }
            return false;
        }
        
        void RecoverHeal()
        {
            if(!healDone && player.GetComponent<Animation>().IsAnimFinished())
            {
                Sound.PlaySFX("VO_Shooter_Placement_04", 0.5f);
                healDone = true;
                npcs[healNPC].GetComponent<Animation>().ChangeAnim("Shooter_Idle");
                player.GetComponent<Animation>().ChangeAnim("MC_Idle");
                Vector2 playerPosEdit = player.LocalPosition;
                if (player.Scale.x > 0)
                    playerPosEdit.x += 0.7f;
                else
                    playerPosEdit.x -= 0.7f;
                player.LocalPosition = playerPosEdit;

                pauseManager.As<PauseManager>().UnPause();
            }
        }
    }
}

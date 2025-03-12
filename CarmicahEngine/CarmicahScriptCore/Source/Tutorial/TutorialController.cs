using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
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
        private Entity[] ditto = new Entity[4];   // can morph into WORLDSPACE trap / npc
        private Entity player;
        private Entity playerHP;
        private Entity cam;

        private int tutorialProgress = 3;
        // For loading certain things only 2 frames later igaf
        private int readyToLoad = 2;

        // For specifically delay effect
        private float delayTimer = 0.0f;

        // Text Things
        private Entity textObj, txtChild;
        private string currText = "";
        private int currPhraseProgress = 0, phraseLength = 0, whichPhrase = -1;
        private float wordsTimer = 0.0f, charSpeed = 0.05f;
        private string[] bunchaWords = new string[16]{ 
                                        "Welcome to Sugar Strike!",
                                        "I am your guide and in this tutorial, enemies will only spawn on the rightmost lane",
                                        "If you want to skip this at any time press 5 :3",
                                        "Oop, here come a critter!",// Pause, Pans down onto the creature
                                        "Use R/F to pan the camera back up, we need to prepare",
                                        "Here, I'll provide you a trap to fend it off",                     // ----- 5 -----
                                        "Hey! that's the wrong spot, here's another trap to try",           // Optional txt
                                        "You know that you're not going to be able to keep these right?",   // Optional txt
                                        ". . . . .",                                                        // Optional txt
                                        "This trap can only take 2 hits total before it is used up",
                                        "Perfect timing, reinforcements from above, use WASD to move to pick it up", // 4 reinforcements // ----- 10 -----
                                        "This is a Soldier unit, you can only place them on top of the tower",
                                        "They have limited charges, but can be healed up by clicking on them",
                                        "They usually start off full charge, but this is what a drained one looks like",
                                        "Be careful to not lose your Soldiers to random enemies",
                                        "Here comes some enemies . . . on 1 lane.. Good luck!",              // Dun matter win / lose
                                        };

        // Pan Effects
        private Entity[] entityToPan = new Entity[5];
        private Vector2[] panSpeed = new Vector2[5];
        private Vector2[] panEndGoal = new Vector2[5];
        private bool[] entityPanFinished = new bool[5];
        // Variables to use
        private bool[]  someBools = new bool[3];
        private Vector2 someVec;
        private float   someFloat;
        private int     someInt;


        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                tutorialProgress = -1;
            }

            // My Ver. of OnCreate, because I need garuntee the things are loaded in before I get them
            if(readyToLoad > 0)
            {
                if(readyToLoad == 1)
                {
                    cam = FindEntityWithName("MainCamera");
                    enemy = FindEntityWithName("Tutorial_Enemy");
                    player = FindEntityWithName("MC");
                    playerHP = FindEntityWithName("Princess_HealthBar");
                    textObj = FindEntityWithName("SubText");
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
                            if (i < 4)
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
                        someFloat = 0.0f;
                        someInt = 5;
                        ++tutorialProgress;
                    }
                    break;
                case 6:
                    // someBools[0] - To scale up / down
                    // someBools[1] - If click & drag
                    // someBools[2] - If need to flip the held obj or no
                    // someFloat    - The scaling
                    // someInt      - 

                    TypeWords(dt, someInt);

                    // First Half - to click the ico
                    if (!someBools[1])
                    {
                        // Not Hover
                        if (!(powerIco[0].As<TutorialBasic>().GetHover()))
                        {
                            if (someBools[0])
                            {
                                someFloat += 0.3f * dt;
                                if (someFloat > 0.7f)
                                    someBools[0] = false;
                            }
                            else
                            {
                                someFloat -= 0.3f * dt;
                                if (someFloat < 0.45f)
                                    someBools[0] = true;

                            }
                            powerIco[0].Scale = new Vector2(someFloat, someFloat);
                            powerIco[0].LocalPosition = new Vector2(someFloat * 83.33f, 232);
                            if (powerIco[0].As<TutorialBasic>().GetExitHover())
                            {
                                powerIco[0].GetComponent<Renderer>().SetColour(1f, 1f, 1f);
                            }
                        }
                        else
                        {
                            if (powerIco[0].As<TutorialBasic>().GetEnterHover())
                            {
                                powerIco[0].GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);
                            }
                            if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
                            {
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
                            CMConsole.Log($"ditto: {ditto[0].LocalPosition.x}, {ditto[0].LocalPosition.y};");

                            // Check loop for place obj
                            for (int i = 0; i < 4; ++i)
                            {
                                if (placedObj[i])
                                    continue;

                                Vector2 dim;
                                FunctionCalls.Transform_GetRenderingScale(traps[i].mID, out dim);
                                dim.x = dim.x * Math.Abs(traps[i].Scale.x) / 4.0f;
                                dim.y = dim.y * traps[i].Scale.y / 2.0f;

                                CMConsole.Log($"DIM: {i},{traps[i].LocalPosition.x}, {traps[i].LocalPosition.y}, {dim.x}, {dim.y};");

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
                                placedObj[justPlaced] = true;
                                traps[justPlaced].GetComponent<Renderer>().SetAlpha(1.0f);

                                if (justPlaced == 3)
                                {
                                    Vector2 enemyPos = enemy.LocalPosition;
                                    enemyPos.y = 10.0f;
                                    SetPanDetails(enemy, enemyPos, 2, 0);

                                    Vector2 enemyScale, trapScale;
                                    FunctionCalls.Transform_GetRenderingScale(enemy.mID, out enemyScale);
                                    FunctionCalls.Transform_GetRenderingScale(traps[3].mID, out trapScale);

                                    someVec.x = enemyScale.y * enemy.Scale.y / 2.0f;
                                    someVec.y = trapScale.y / 2.0f;
                                    ++tutorialProgress;
                                }
                                else
                                {
                                    someFloat = 0.0f;
                                    ++someInt;
                                    powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                                }
                            }
                            // Failed to place
                            else
                            {
                                powerIco[0].GetComponent<Renderer>().SetAlpha(1.0f);
                            }
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
                    TypeWords(dt, 9);
                    Panning(dt, 0);
                    if(enemy.LocalPosition.y + someVec.x > traps[3].LocalPosition.y)
                    {
                        enemy.GetComponent<Animation>().ChangeAnim("Mouse_Death_blue");
                        traps[3].GetComponent<Animation>().ChangeAnim("CandyCone");
                        someBools[0] = false;
                        ++tutorialProgress;
                    }
                    break;
                case 8:
                    if (enemy.GetComponent<Animation>().IsAnimFinished())
                    {
                        if (player.LocalPosition.x > 0)
                            ditto[1].LocalPosition = new Vector2(-4, 20);
                        else
                            ditto[1].LocalPosition = new Vector2(4, 20);
                        someBools[0] = true;
                    }
                    if (someBools[0])
                    {
                        SetPanDetails(cam, new Vector2(0, 10), 20, 0);
                        if (player.LocalPosition.x > 0)
                            ditto[1].LocalPosition = new Vector2(-4, 6);
                        else
                            ditto[1].LocalPosition = new Vector2(4, 6);

                        ++tutorialProgress;
                    }
                    break;
                case 9:
                    TypeWords(dt, 10);
                    Panning(dt, 0);
                    Panning(dt, 1);
                    break;
                case 10:
                    break;
                case 11:
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
                currPhraseProgress = 0;
                phraseLength = bunchaWords[phraseNum].Length;
                wordsTimer = 0;
                whichPhrase = phraseNum;
                currText = "";
            }
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

                    txtChild.Scale = new Vector2(textObj.GetComponent<TextRenderer>().GetWidth() / 47.0f, 1.0f);
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
    }
}

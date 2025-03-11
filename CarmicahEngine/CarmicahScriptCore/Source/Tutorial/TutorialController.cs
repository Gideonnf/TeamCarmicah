using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CarmicahScript
{
    public class TutorialController : Entity
    {
        private Entity enemy;
        private Entity[] powerIco = new Entity[4];// Reset 1 to (0, 240)
        private Entity player;
        private Entity playerHP;
        private Entity cam;

        private float delayTimer = 0.0f;

        // Text Things
        private Entity textObj, txtChild;
        private string currText = "";
        private int currPhraseProgress = 0, phraseLength = 0, whichPhrase = -1;
        private float wordsTimer = 0.0f, charSpeed = 0.05f;
        private string[] bunchaWords = new string[15]{ 
                                        "Welcome to Sugar Strike",
                                        "I will be here to guide you through the tutorial",
                                        "If you want to skip this at any time press 5 :3",
                                        "Oop, here come a critter!",// Pause, Pans down onto the creature
                                        "Use R/F to pan the camera back up, we need to prepare",
                                        "Here, I'll provide you a trap to fend it off",
                                        "Hey! that's the wrong spot, here's another trap to try",           // Optional txt
                                        "You know that you're not going to be able to keep these right?",   // Optional txt
                                        "This trap can only take 2 hits total before it is used up",
                                        "Perfect timing, reinforcements from above, use WASD to move to pick it up", // 4 reinforcements
                                        "This is a Soldier unit, you can only place them on top of the tower",
                                        "They have limited charges, but can be healed up by clicking on them",
                                        "They usually start off full charge, but this is what a drained one looks like",
                                        "Be careful to not lose your Soldiers to random enemies",
                                        "Here comes a enemy rush ~x~; . . . Erm.. Good luck!",              // Dun matter win / lose
                                        };

        // Pan Effects
        private Entity entityToPan;
        private Vector2 panSpeed;
        private Vector2 panEndGoal;
        private bool entityPanFinished = true;
        

          int tutorialProgress = 0;

        public override void OnCreate()
        {
            cam = FindEntityWithName("MainCamera");
            enemy = FindEntityWithName("Tutorial_Enemy");
            Entity UIBar = FindEntityWithName("UIBar");
            {
                Entity[] UIBarChild = UIBar.GetAllChildren();
                int i = 0;
                foreach(Entity child in UIBarChild)
                {
                    if(i < 4)
                        powerIco[i++] = child;
                }
            }
            player = FindEntityWithName("MC");
            playerHP = FindEntityWithName("Princess_HealthBar");
            textObj = FindEntityWithName("SubText");
            txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
            //Input.GetMousePos();
        }

        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                tutorialProgress = -1;
            }


            // Mouse Position things
            //if(powerIco.As<TutorialBasic>().GetEnterHover())
            //{
            //
            //}
            //
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
                        SetPanDetails(cam, new Vector2(0, -7.6f), 20);
                        ++tutorialProgress;
                    }
                    break;
                case 4:
                    if (Panning(dt))
                    {
                        TypeWords(dt, 4);
                        if (cam.LocalPosition.y > 3)
                            ++tutorialProgress;
                    }
                    break;
                case 5:
                    TypeWords(dt, 5);
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                case 9:
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

        void SetPanDetails(Entity entity, Vector2 endPos, float speed)
        {
            entityToPan = entity;
            panSpeed = (endPos - entity.LocalPosition).Normalize();
            panSpeed.x *= speed;
            panSpeed.y *= speed;
            panEndGoal = endPos;
            entityPanFinished = false;
        }

        bool Panning(float dt)
        {
            if(!entityPanFinished)
            {
                Vector2 pos = entityToPan.LocalPosition;
                pos.x += panSpeed.x * dt;
                pos.y += panSpeed.y * dt;
                entityToPan.LocalPosition = pos;
                if ((panSpeed.x < 0 && pos.x < panEndGoal.x) ||
                    (panSpeed.x > 0 && pos.x > panEndGoal.x) ||
                    (panSpeed.y < 0 && pos.y < panEndGoal.y) ||
                    (panSpeed.y > 0 && pos.y > panEndGoal.y))
                    entityPanFinished = true;
            }
            return entityPanFinished;
        }
    }
}

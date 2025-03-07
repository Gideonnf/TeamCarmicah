/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Cutscene.cs
  
 author:		YANG YUJIE (40%)

 email:		    y.yujie@digien.edu

 brief:			The cut scene system is responsible for managing the cut scene. It will change the image of the cut scene every few seconds.
                The cut scene will be displayed for a few seconds before transitioning to the next image. The system will also handle the fade in and fade out effect.
                The system will also play the background music for the cut scene.

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
    public class Cutscene : Entity
    {
        public float timer = 0.0f;
        // Running Total for how long for each Panel
        private float panel1 = 7.0f;     // In a Lang far far away...
        private float panel2a = 16.0f;   // The kingdom stood...
        private float panel2b = 20.0f;   // But one day (pause)
        private float panel3 = 25.0f;    // When the critters attacked!
        private float panel4a = 30.5f;   // Known for...
        private float panel4b = 36.0f;
        // Cutscene Img 2
        private float panel5 = 45.0f;    // Determined to save her kingdom...
        private float panel6 = 55.0f;    // With Courage in their hearts...
        // White Color
        private float panel7 = 64.0f;    // And thus... (FadeOut)

        public float fadeSpeed = 0.5f;

        private string cutscenePrefab = "CutSceneImage";
        private string img1Name = "Opening_Cutscene1_SpriteSheet_Yes";
        private int img1Size = 6;
        private string img2Name = "Opening_Cutscene2_SpriteSheet_Yes";
        private int img2Size = 2;
        private string img3Name = "Wall_Invis";
        private int img3Size = 1;

        // Background music configuration  
        public string backgroundMusicTrack = "Cutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        // Dictionary to store custom display times for each panel
        private float[] panelTimings = new float[9];
        private string[] panelWords = new string[9]{ "In a land far far away, filled with sweet treats and goodness",
                                        "The Candy Kingdom stood proud and tall under the rule of princess Strawberry",
                                        "But one day everything changed",
                                        "When the Critters attacked!",
                                        "Known for devouring everything in their path,",
                                        "the fall of the Candy Kingdom would be imminent if nothing was done",
                                        "Determined to save her kingdom, the princess rallied her troops to prepare for the oncoming threat.",
                                        "With courage in their hearts, the fate of the Candy Kingdom lied in the palm of their hands",
                                        "And thus began the battle all would remember as..."};
        private bool musicStarted = false;


        int currentPanel = 0;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        Entity textObj;
        string currText = "";
        float currAlpha;

        // If i need to load all the panels
        // i need them stored in a string 

        void OnCreate()
        {
            Sound.StopSoundBGM(this.mID, backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");
            cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
            textObj = FindEntityWithName("CutsceneText");

            //initialize panel timings
            SetupPanelTimings();

            // Start background music when cutscene begins
            if (!musicStarted)
            {
                Sound.PlaySFX(this.mID, backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }

        }
        void SetupPanelTimings()
        {
            // Set custom display times for each panel
            panelTimings[0] = panel1;
            panelTimings[1] = panel2a;
            panelTimings[2] = panel2b;
            panelTimings[3] = panel3;
            panelTimings[4] = panel4a;
            panelTimings[5] = panel4b;
            panelTimings[6] = panel5;
            panelTimings[7] = panel6;
            panelTimings[8] = panel7;
        }

        private string GetCurrPanelName()
        {
            int panelCounter = currentPanel;
            if (panelCounter < img1Size)
            {
                return img1Name + " " + panelCounter;
            }
            panelCounter -= img1Size;
            if(panelCounter < img2Size)
            {
                return img2Name + " " + panelCounter;
            }
            panelCounter -= img2Size;
            if (panelCounter < img3Size)
            {
                return img3Name + " " + panelCounter;
            }
            return "";
        }

        private void UpdateCutsceneText()
        {
            if (currentPanel < panelWords.Length)
            {
                float startTime = 0;
                if (currentPanel != 0)
                    startTime = panelTimings[currentPanel - 1];
                float percentage = (timer - startTime) / (panelTimings[currentPanel] - startTime - 1.0f);
                int numTextToAdd = Math.Min((int)(percentage * panelWords[currentPanel].Length), panelWords[currentPanel].Length);
                while (numTextToAdd > currText.Length)
                {
                    currText += panelWords[currentPanel][currText.Length];
                }
                textObj.GetComponent<TextRenderer>().SetText(currText);
            }
        }

        private void ProgressScene()
        {
            Scene.ChangeScene("Loading");
        }

        void OnUpdate(float dt)
        {
            if(Input.IsKeyPressed(Keys.KEY_5))
            {
                ProgressScene();
            }
        }

        public void OnStateEnter(string stateName)
        {
            CMConsole.Log($"State : {stateName}");

            // Change image state
            if (stateName == "ChangeImage")
            {
                if (nextCutsceneEntity == null)
                {
                    nextCutsceneEntity = CreateGameObject(cutscenePrefab);
                    currAlpha = 1.0f;

                    string imageName = GetCurrPanelName();
                    CMConsole.Log($"image name:{imageName}");
                    // change texture for image
                    nextCutsceneEntity.GetComponent<Renderer>().ChangeTexture(imageName);
                }
                currText = "";
                textObj.GetComponent<TextRenderer>().SetText(currText);
            }
            // Idle state
            else if (stateName == "Idle")
            {
                if (cutsceneEntity == null)
                {
                    cutsceneEntity = nextCutsceneEntity;
                    cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
                    nextCutsceneEntity = null;
                }
            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
            timer += dt;

            UpdateCutsceneText();

            //Console.WriteLine(panelTimings.Count());
            CMConsole.Log($"Alpha : {currAlpha}, timer: {timer}");
            if (stateName == "ChangeImage")
            {
                currAlpha -= fadeSpeed * dt;
                cutsceneEntity.GetComponent<Renderer>().SetAlpha(currAlpha);
                if (currAlpha < 0.0f)
                {
                    cutsceneEntity.Destroy();
                    cutsceneEntity = null;
                    //CMConsole.Log("Changing??");

                    // change state to idle
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }

            if (stateName == "Idle")
            {
                float currentPanelDisplayTime = panelTimings[currentPanel];

                if (timer >= currentPanelDisplayTime)
                {
                    //CMConsole.Log("Changing??");
                    // change image state
                    ++currentPanel;
                    if (currentPanel < panelTimings.Length)
                    {
                        GetComponent<StateMachine>().SetStateCondition(2);
                    }
                    else
                    {
                        ProgressScene();
                    }
                }
                else
                {
                    //Sound.StopSound(backgroundMusicTrack);
                }
            }

        }

        public void OnStateExit(string stateName)
        {

        }

    }
}

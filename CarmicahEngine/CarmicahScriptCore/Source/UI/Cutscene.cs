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
        //public float timeToChange = 2.0f;
        public float panelDisplayTime = 10.0f;     // How long each panel stays visible
        public float fadeTransitionTime = 3.0f;   // How long the fade transition takes
        public float fadeSpeed = 0.25f;

        public string cutscenePrefab = "CutSceneImage";
        public string panel1Name = "Opening_Cutscene1_SpriteSheet_Yes";
        public int numOfPanels1 = 6;
        public string panel2Name = "Opening_Cutscene2_SpriteSheet_Yes";
        public int numOfPanels = 8;

        // Background music configuration  
        public string backgroundMusicTrack = "Cutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        // Dictionary to store custom display times for each panel
        private Dictionary<int, float> panelTimings = new Dictionary<int, float>();
        private bool musicStarted = false;


        int currentPanel = 1;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        float currAlpha;

        // If i need to load all the panels
        // i need them stored in a string 

        void OnCreate()
        {

            Sound.StopSoundBGM(backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");

            //initialize panel timings
            SetupPanelTimings();

            // Start background music when cutscene begins
            if (!musicStarted)
            {
                Sound.PlaySFX(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }

        }
        void SetupPanelTimings()
        {
            // Set custom display times for each panel
            panelTimings[1] = 25.0f;
            panelTimings[2] = 3.0f;
            panelTimings[3] = 10.0f;
            panelTimings[4] = 10.0f;
            panelTimings[5] = 10.0f;
            panelTimings[6] = 15.0f;

            panelTimings[7] = 10.0f;
            panelTimings[8] = 20.0f;
        }

        float GetCurrentPanelTime()
        {
            // Get the custom timing for current panel, or use default if not specified
            if (panelTimings.ContainsKey(currentPanel))
            {
                return panelTimings[currentPanel];
            }
            return 5.0f; // Default display time if no custom timing is set
        }

        void OnUpdate(float dt)
        {
            //CMConsole.Log($"Current Panel: {currentPanel}");
            if(Input.IsKeyPressed(Keys.KEY_5))
            {
                Scene.ChangeScene("Loading");
            }
        }

        public void OnStateEnter(string stateName)
        {
            timer = 0.0f;
            CMConsole.Log($"State : {stateName}");

            // Change image state
            if (stateName == "ChangeImage")
            {
                if (nextCutsceneEntity == null)
                {
                    nextCutsceneEntity = CreateGameObject(cutscenePrefab);
                    nextCutsceneEntity.Depth = cutsceneEntity.Depth - 0.5f;
                    currAlpha = 1.0f;

                    string imageName;
                    if (currentPanel < numOfPanels1)
                        imageName = panel1Name + " " + currentPanel;
                    else
                        imageName = panel2Name + " " + (currentPanel - numOfPanels1);
                    CMConsole.Log($"image name:{imageName}");
                    // change texture for image
                    nextCutsceneEntity.GetComponent<Renderer>().ChangeTexture(imageName);
                }

            }
            // Idle state
            else if (stateName == "Idle")
            {
                if (cutsceneEntity == null)
                {
                    cutsceneEntity = nextCutsceneEntity;
                    nextCutsceneEntity = null;
                }
            }
        }

        public void OnStateUpdate(string stateName, float dt)
        {
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
                timer += dt;
                float currentPanelDisplayTime = GetCurrentPanelTime();

                if (timer >= currentPanelDisplayTime)
                {
                    //CMConsole.Log("Changing??");
                    // change image state
                    currentPanel++;
                    if (currentPanel <= numOfPanels)
                    {
                        GetComponent<StateMachine>().SetStateCondition(2);
                    }
                    else
                    {
                        Scene.ChangeScene("Loading");
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

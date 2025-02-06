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
        private bool musicStarted = false;

        int currentPanel = 1;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        float currAlpha;

        // If i need to load all the panels
        // i need them stored in a string 

        void OnCreate()
        {
            cutsceneEntity = FindEntityWithName("CutSceneImage");

            // Start background music when cutscene begins
            if (!musicStarted)
            {

                Sound.PlaySFX(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            } 
        }

        void OnUpdate(float dt)
        {
        }

        public void OnStateEnter(string stateName)
        {
            timer = 0.0f;
            CMConsole.Log($"State : {stateName}");

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
                if (timer >= panelDisplayTime)
                {
                    //CMConsole.Log("Changing??");
                    // change image state
                    currentPanel++;
                    if (currentPanel <= numOfPanels)
                        GetComponent<StateMachine>().SetStateCondition(2);
                }


                else
                {
                    Sound.StopSound(backgroundMusicTrack);
                }
            }

        }

        public void OnStateExit(string stateName)
        {

        }

    }
}

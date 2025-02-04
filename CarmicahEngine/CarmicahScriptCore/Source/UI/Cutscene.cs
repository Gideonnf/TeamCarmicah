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
        public float timeToChange = 2.0f;
        public string cutscenePrefab = "CutSceneImage";
        public string panelName = "panel";
        public int numOfPanels = 8;
        int currentPanel = 1;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        float currAlpha;

        // If i need to load all the panels
        // i need them stored in a string 

        void OnCreate()
        {
            cutsceneEntity = FindEntityWithName("CutSceneImage");
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
                    string imageName = panelName + "_" + currentPanel;
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
                currAlpha -= 0.2f * dt;
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
                if (timer >= timeToChange)
                {
                    //CMConsole.Log("Changing??");
                    // change image state
                    currentPanel++;
                    if (currentPanel <= numOfPanels)
                        GetComponent<StateMachine>().SetStateCondition(2);
                }
            }

        }

        public void OnStateExit(string stateName)
        {

        }

    }
}

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
        private float panel1 = 11.0f;    //In a land far far away, filled with sweet treats and goodness the candy kingdom ...11s
        private float panel2a = 16.0f;   //under the rule of princess Strawberry (5s + 11s)
        private float panel2b = 20.0f;   //But one day everything changed (4s + 16s)
        private float panel3 = 24.0f;    //When the Critters attacked! (4s + 20s)
        private float panel4a = 29.0f;   //Known for devouring everything in their path (5s + 24s)
        private float panel4b = 33.0f;   //the fall of the Candy Kingdom would be imminent (4s + 29s)
        // Cutscene Img 2
        private float panel5 = 37.0f;    //if nothing was done (4s + 33s)
        private float panel6 = 40.0f;    //Determined to save her kingdom (3s + 37s)
        private float panel7 = 46.0f;    //princess rallied her troops to prepare for the oncoming threat (6s + 40s)
        private float panel8 = 49.5f;    //with courage in their hearts (3.5s + 46s)
        private float panel9 = 55.0f;    //the fate of candy kingdom (5.5s + 49.5s)
        private float panel10 = 64.0f;   //And thus began (2.5s + 55s)
       // private float panel11 = 64.0f;   //the battle all would remember as... (6.5s + 57.5s)

        public float fadeSpeed = 0.5f;

        private string cutscenePrefab = "CutSceneImage";
        private string img1Name = "Opening_Cutscene1_SpriteSheet_Yes";
        private int img1Size = 6;
        private string img2Name = "Opening_Cutscene2_SpriteSheet_Yes";
        private int img2Size = 6; // changed to 6
        private string img3Name = "Wall_Invis";
        private int img3Size = 1;

        // Background music   
        // Background music   
        public string backgroundMusicTrack = "Cutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        private float[] panelTimings = new float[12]; // Increased to 11 panels
        private string[] panelWords = new string[12]{
            "In a land far far away, filled with sweet treats and goodness",
            "The Candy Kingdom stood proud and tall under the rule of princess Strawberry",
            "But one day everything changed",
            "When the Critters attacked!",
            "Known for devouring everything in their path,",
            "the fall of the Candy Kingdom would be imminent",
            "if nothing was done",
            "Determined to save her kingdom",
            "the princess rallied her troops to prepare for the oncoming threat.",
            "With courage in their hearts,",
            "the fate of the Candy Kingdom lied in the palm of their hands",
            "And thus began, the battle all would remember as..."
        };
        private bool musicStarted = false;

        int currentPanel = 0;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        Entity textObj;
        string currText = "";
        float currAlpha;

        //srore in a string for the panel 

        public override void OnCreate()
        {
            Sound.StopSoundBGM(backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");
            cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
            textObj = FindEntityWithName("CutsceneText");

            SetupPanelTimings();

            if (!musicStarted)
            {
                Sound.PlaySFX(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }
        }

        void SetupPanelTimings()
        {
            // Set custom display times for each panel
            //
            panelTimings[0] = panel1;
            panelTimings[1] = panel2a;
            panelTimings[2] = panel2b;
            panelTimings[3] = panel3;
            panelTimings[4] = panel4a;
            panelTimings[5] = panel4b;
            panelTimings[6] = panel5;
            panelTimings[7] = panel6;
            panelTimings[8] = panel7;
            panelTimings[9] = panel8;
            panelTimings[10] = panel9;
            panelTimings[11] = panel10;
        }

        private string GetCurrPanelName()
        {
            int panelCounter = currentPanel;
            if (panelCounter < img1Size)
            {
                return img1Name + " " + panelCounter;
            }
            panelCounter -= img1Size;
            if (panelCounter < img2Size)
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

                bool toUpdateTxtYet = currText.Length != 0;

                while (numTextToAdd > currText.Length)
                {
                    currText += panelWords[currentPanel][currText.Length];
                }
                if(toUpdateTxtYet)
                {
                    Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                    float txtWidth = textObj.GetComponent<TextRenderer>().GetWidth();

                    txtChild.Scale = new Vector2(txtWidth / 47.0f, 1.2f);
                    Vector2 txtBgPos = Vector2.Zero;
                    txtBgPos.x += txtWidth / 2.0f;
                    txtChild.LocalPosition = txtBgPos;
                }
                textObj.GetComponent<TextRenderer>().SetText(currText);
            }
        }

        private void ProgressScene()
        {
            Scene.ChangeScene("Loading");
        }

        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                ProgressScene();
            }
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            timer += fixedDt;
        }

        public override void OnStateEnter(string stateName)
        {
           // CMConsole.Log($"State : {stateName}");

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
                Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                txtChild.Scale = new Vector2(0.0f, 0.0f);

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

        public override void OnStateUpdate(string stateName, float dt)
        {
            

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

        public override void OnStateExit(string stateName)
        {

        }
    }
}
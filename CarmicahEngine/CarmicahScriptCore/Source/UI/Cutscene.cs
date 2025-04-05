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
        private float panel1 = 7.0f;
        private float panel2a = 10.0f;
        private float panel2b = 14.0f;
        private float panel3 = 18.0f;
        private float panel4a = 21.0f;
        private float panel4b = 24.5f;
        private float panel5 = 26.8f;
        private float panel6 = 31.0f;
        private float panel7 = 33.0f;
        private float panel8 = 38.5f;
        private float panel9 = 44.0f;
        private float panel10 = 46.0f;
        private float panelFadeOut = 48.0f;

        public float fadeSpeed = 0.5f;

        private string cutscenePrefab = "CutSceneImage";
        private string img1Name = "Opening_Cutscene1_SpriteSheet_Yes";
        private int img1Size = 6;
        private string img2Name = "Opening_Cutscene2_SpriteSheet_Yes";
        private int img2Size = 6;
        private string img3Name = "Wall_White";
        private int img3Size = 1;

        public string backgroundMusicTrack = "Cutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        // Flags for special transitions
        //private bool isWhiteOutTransition = false;
        //private Entity whiteFlashEntity = null;
        //private float whiteFlashAlpha = 0.0f;
        //private float whiteOutSpeed = 2.0f;

        // Animation speeds
        private float zoomSpeed = 0.05f;
        private float panSpeed = 0.25f;

        private float[] panelTimings = new float[13];
        private string[] panelWords = new string[13]{
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
            "And thus began, the battle all would remember as...",
            ""
        };
        private bool musicStarted = false;

        int currentPanel = 0;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        Entity textObj;
        string currText = "";
        float currAlpha;
        Vector2 originalScale;

        public override void OnCreate()
        {
            Sound.StopSoundBGM(backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");
            cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
            textObj = FindEntityWithName("CutsceneText");

            originalScale = cutsceneEntity.Scale;

            SetupPanelTimings();

            if (!musicStarted)
            {
                Sound.PlaySFX(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }
        }

        void SetupPanelTimings()
        {
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
            panelTimings[12] = panelFadeOut;
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
                if (toUpdateTxtYet)
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
            Sound.StopAllSFX();
            FindEntityWithName("SceneTransition").As<SceneTransition>().FadeOut("Loading");
            //Scene.ChangeScene("Loading");
        }

        public override void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                ProgressScene();
            }

            //if (isWhiteOutTransition && whiteFlashEntity != null)
            //{
            //    whiteFlashAlpha += whiteOutSpeed * dt;
            //    if (whiteFlashAlpha > 1.0f)
            //        whiteFlashAlpha = 1.0f;
            //
            //    whiteFlashEntity.GetComponent<Renderer>().SetAlpha(whiteFlashAlpha);
            //
            //    if (whiteFlashAlpha >= 1.0f)
            //    {
            //        ProgressScene();
            //    }
            //}
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            timer += fixedDt;
        }

        public override void OnStateEnter(string stateName)
        {
            if (stateName == "ChangeImage")
            {
                if (nextCutsceneEntity == null)
                {
                    nextCutsceneEntity = CreateGameObject(cutscenePrefab);
                    currAlpha = 1.0f;

                    string imageName = GetCurrPanelName();
                    nextCutsceneEntity.GetComponent<Renderer>().ChangeTexture(imageName);

                    if(currentPanel == 12)
                    {
                        nextCutsceneEntity.Scale = new Vector2(1000.0f, 1000.0f); 
                    }
                }

                Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                txtChild.Scale = new Vector2(0.0f, 0.0f);

                currText = "";
                textObj.GetComponent<TextRenderer>().SetText(currText);
            }
            else if (stateName == "Idle")
            {
                if (cutsceneEntity == null)
                {
                    cutsceneEntity = nextCutsceneEntity;
                    cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
                    nextCutsceneEntity = null;

                    if (currentPanel == 6)
                    {
                        cutsceneEntity.Scale = new Vector2(originalScale.x * 1.1f, originalScale.y * 1.1f);
                    }
                    else if (currentPanel == 12)
                    {
                    //    if (whiteFlashEntity == null)
                    //    {
                    //        whiteFlashEntity = CreateGameObject("WhiteOverlay");
                    //        if (whiteFlashEntity != null)
                    //        {
                    //            whiteFlashEntity.Depth = cutsceneEntity.Depth + 1.0f;
                    //            whiteFlashEntity.GetComponent<Renderer>().SetAlpha(0.0f);
                    //            isWhiteOutTransition = true;
                    //        }
                    //    }
                    }
                    else
                    {
                        cutsceneEntity.Scale = originalScale;
                    }
                }
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            UpdateCutsceneText();

            if (stateName == "ChangeImage")
            {
                float currentFadeSpeed = fadeSpeed;

                if (currentPanel == 2 || currentPanel == 7 || currentPanel == 9)
                {
                    currentFadeSpeed = fadeSpeed * 1.5f;
                }

                currAlpha -= currentFadeSpeed * dt;
                cutsceneEntity.GetComponent<Renderer>().SetAlpha(currAlpha);

                if (currAlpha < 0.0f)
                {
                    cutsceneEntity.Destroy();
                    cutsceneEntity = null;
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }

            if (stateName == "Idle")
            {
                if (currentPanel == 0)
                {
                    Vector2 scale = cutsceneEntity.Scale;
                    scale.x += zoomSpeed * dt;
                    scale.y += zoomSpeed * dt;
                    cutsceneEntity.Scale = scale;
                }

                else if (currentPanel == 6)
                {
                    Vector2 pos = cutsceneEntity.Position;
                    pos.y -= panSpeed * dt;
                    cutsceneEntity.Position = pos;
                }

                //if (currentPanel == 11 && isWhiteOutTransition)
                //{
                //    return;
                //}

                float currentPanelDisplayTime = panelTimings[currentPanel];
                if (timer >= currentPanelDisplayTime)
                {
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
            }
        }

        public override void OnStateExit(string stateName)
        {
        }
    }
}
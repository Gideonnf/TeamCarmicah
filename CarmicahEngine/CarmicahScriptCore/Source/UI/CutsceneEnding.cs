using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class CutsceneEnding : Entity
    {
        public float timer = 0.0f;

        // Panel timings
        private float panel1 = 15.0f;        // First panel fade in
        private float panel2 = 27.0f;        // Second panel fade in
        private float panelFadeOut = 33.0f;  // Final fade out

        public float fadeSpeed = 0.5f;
        public float finalFadeOutDuration = 1.5f;
        private bool isFadingOut = false;
        private float fadeOutTimer = 0.0f;

        private string cutscenePrefab = "CutSceneImage";
        private string baseImgName = "EndingCutscene_SpriteSheet_Yes";
        private string img3Name = "Wall_White_1";
        private int img3Size = 1;

        // Background music
        public string backgroundMusicTrack = "Endingcutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        // White overlay for transitions
        private bool isWhiteOutTransition = false;
        private Entity whiteFlashEntity = null;
        private float whiteFlashAlpha = 0.0f;
        private float whiteOutSpeed = 2.0f;

        private float[] panelTimings = new float[3];
        private string[] panelWords = new string[3]{
            "As the last Critter fell, a sense of relief washed over Princess Strawberry, knowing all their efforts had not been in vain as she heard her kingdom erupt in cheers of victory over their fallen enemy.",
            "And from then on the Candy Kingdom lived happily ever after",
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
        float particlesDownTime = 4.0f;
        Entity leftParticles;
        Entity rightParticles;

        public override void OnCreate()
        {
            Sound.StopSoundBGM(backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");
            textObj = FindEntityWithName("Default");
            leftParticles = FindEntityWithName("Left Particles");
            rightParticles = FindEntityWithName("Right Particles");

            if (cutsceneEntity != null)
            {
                originalScale = cutsceneEntity.Scale;
                cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
            }

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
            panelTimings[1] = panel2;
            panelTimings[2] = panelFadeOut;
        }

        private string GetCurrPanelName()
        {
            if (currentPanel < 2)
                return baseImgName + " " + currentPanel;
            else
                return img3Name + " 0";
        }

        private void UpdateCutsceneText()
        {
            if (currentPanel < panelWords.Length && panelWords[currentPanel].Length > 0)
            {
                float startTime = 0;
                if (currentPanel != 0)
                    startTime = panelTimings[currentPanel - 1];

                float percentage = (timer - startTime) / (panelTimings[currentPanel] - startTime - 1.0f);
                percentage = Math.Min(Math.Max(percentage, 0), 1); // Clamp between 0 and 1

                int numTextToAdd = Math.Min((int)(percentage * panelWords[currentPanel].Length), panelWords[currentPanel].Length);

                bool toUpdateTxtYet = currText.Length != 0;

                while (numTextToAdd > currText.Length && currText.Length < panelWords[currentPanel].Length)
                {
                    currText += panelWords[currentPanel][currText.Length];
                }

                if (toUpdateTxtYet && textObj != null)
                {
                    Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                    float txtWidth = textObj.GetComponent<TextRenderer>().GetWidth();

                    txtChild.Scale = new Vector2(txtWidth / 47.0f, 1.2f);
                    Vector2 txtBgPos = Vector2.Zero;
                    txtBgPos.x += txtWidth / 2.0f;
                    txtChild.LocalPosition = txtBgPos;
                }

                if (textObj != null)
                    textObj.GetComponent<TextRenderer>().SetText(currText);
            }
        }

        private void ProgressScene()
        {
            Sound.StopAllSFX();
            Scene.ChangeScene("Scene3");
        }

        public override void OnUpdate(float dt)
        {
            if (leftParticles == null)
                leftParticles = FindEntityWithName("Left Particles");
            else if (rightParticles == null)
                rightParticles = FindEntityWithName("Right Particles");

            if (leftParticles != null && !leftParticles.GetComponent<ParticleEmitter>().GetActive())
            {
                particlesDownTime += dt;
                if (particlesDownTime > 4.0f)
                {
                    leftParticles.GetComponent<ParticleEmitter>().SetActive();
                    rightParticles.GetComponent<ParticleEmitter>().SetActive();
                    particlesDownTime = 0.0f;
                }
            }

            if (isWhiteOutTransition && whiteFlashEntity != null)
            {
                whiteFlashAlpha += whiteOutSpeed * dt;
                if (whiteFlashAlpha > 1.0f)
                    whiteFlashAlpha = 1.0f;

                whiteFlashEntity.GetComponent<Renderer>().SetAlpha(whiteFlashAlpha);

                if (whiteFlashAlpha >= 1.0f)
                {
                    ProgressScene();
                }
            }
            else if (isFadingOut)
            {
                fadeOutTimer += dt;
                if (fadeOutTimer >= finalFadeOutDuration)
                {
                    Sound.StopAllSFX();
                    //FindEntityWithName("SceneTransition").As<SceneTransition>().FadeOut("Scene3");
                    Scene.ChangeScene("Scene3");
                }
            }

            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                ProgressScene();
            }

            UpdateCutsceneText();
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

                    if (currentPanel == 2) // White overlay for transition
                    {
                        nextCutsceneEntity.Scale = new Vector2(1000.0f, 1000.0f);
                    }
                }

                if (textObj != null)
                {
                    Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                    txtChild.Scale = new Vector2(0.0f, 0.0f);

                    currText = "";
                    textObj.GetComponent<TextRenderer>().SetText(currText);
                }
            }
            else if (stateName == "Idle")
            {
                if (cutsceneEntity == null)
                {
                    cutsceneEntity = nextCutsceneEntity;
                    if (cutsceneEntity != null)
                        cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;
                    nextCutsceneEntity = null;

                    if (currentPanel == 2) // White overlay for the final scene
                    {
                        if (whiteFlashEntity == null)
                        {
                            whiteFlashEntity = CreateGameObject("WhiteOverlay");
                            if (whiteFlashEntity != null)
                            {
                                whiteFlashEntity.Depth = cutsceneEntity.Depth + 1.0f;
                                whiteFlashEntity.GetComponent<Renderer>().SetAlpha(0.0f);
                                isWhiteOutTransition = true;
                            }
                        }
                    }
                }
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (stateName == "ChangeImage")
            {
                currAlpha -= fadeSpeed * dt;
                if (cutsceneEntity != null)
                    cutsceneEntity.GetComponent<Renderer>().SetAlpha(currAlpha);

                if (currAlpha < 0.0f)
                {
                    if (cutsceneEntity != null)
                        cutsceneEntity.Destroy();
                    cutsceneEntity = null;
                    GetComponent<StateMachine>().SetStateCondition(1);
                }
            }

            if (stateName == "Idle")
            {
                if (cutsceneEntity != null && currentPanel < 2)
                {
                    // Slightly zoom in for dramatic effect
                    Vector2 scale = cutsceneEntity.Scale;
                    scale.x += 0.005f * dt;
                    scale.y += 0.005f * dt;
                    cutsceneEntity.Scale = scale;
                }

                if (currentPanel == 1 && isWhiteOutTransition)
                {
                    return;
                }

                float currentPanelDisplayTime = panelTimings[currentPanel];
                if (timer >= currentPanelDisplayTime)
                {
                    ++currentPanel;
                    if (currentPanel < panelTimings.Length - 1)
                    {
                        GetComponent<StateMachine>().SetStateCondition(2);
                    }
                    else if (!isFadingOut && !isWhiteOutTransition)
                    {
                        // Start the final fade out
                        isFadingOut = true;
                        fadeOutTimer = 0.0f;
                        Sound.SwitchBGM("zero", finalFadeOutDuration, finalFadeOutDuration, false);
                    }
                }

                if (isFadingOut && cutsceneEntity != null && !isWhiteOutTransition)
                {
                    float fadePercent = fadeOutTimer / finalFadeOutDuration;
                    cutsceneEntity.GetComponent<Renderer>().SetAlpha(1.0f - fadePercent);

                    if (textObj != null)
                    {
                        textObj.GetComponent<Renderer>().SetAlpha(1.0f - fadePercent);
                        Entity txtChild = new Entity(FunctionCalls.Entity_GetChild(textObj.mID));
                        txtChild.GetComponent<Renderer>().SetAlpha(1.0f - fadePercent);
                    }
                }
            }
        }

        public override void OnStateExit(string stateName)
        {
        }
    }
}
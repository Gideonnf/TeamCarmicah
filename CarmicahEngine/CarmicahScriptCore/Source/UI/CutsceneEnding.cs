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
        private float timeToSwapPanel = 30.0f;

        public float fadeSpeed = 0.5f;
        public float finalFadeOutDuration = 1.5f;
        private bool isFadingOut = false;
        private float fadeOutTimer = 0.0f;

        private string cutscenePrefab = "CutSceneImage";
        private string nextImgName = "EndingCutscene_SpriteSheet_Yes 1";
        //private int img1Size = 2;

        // Background music
        public string backgroundMusicTrack = "Endingcutscene";
        public string backgroundMusicTrack1 = "BGM_SetupPhase_Mix1";

        private bool musicStarted = false;

        int currentPanel = 0;
        Entity cutsceneEntity;
        Entity nextCutsceneEntity;
        float particlesDownTime = 4.0f;
        Entity leftParticles;
        Entity rightParticles;
        float currAlpha;

        public override void OnCreate()
        {
            Sound.StopSoundBGM(backgroundMusicTrack1);
            cutsceneEntity = FindEntityWithName("CutSceneImage");
            leftParticles = FindEntityWithName("Left Particles");
            rightParticles = FindEntityWithName("Right Particles");
            cutsceneEntity.Depth = cutsceneEntity.Depth + 1.0f;

            if (!musicStarted)
            {
                Sound.PlaySFX(backgroundMusicTrack, 0.5f);
                musicStarted = true;
            }
        }

        public override void OnUpdate(float dt)
        {
            if (leftParticles == null)
                leftParticles = FindEntityWithName("Left Particles");
            else if (rightParticles == null)
                rightParticles = FindEntityWithName("Right Particles");

            if (!leftParticles.GetComponent<ParticleEmitter>().GetActive())
            {
                particlesDownTime += dt;
                if (particlesDownTime > 4.0f)
                {
                    leftParticles.GetComponent<ParticleEmitter>().SetActive();
                    rightParticles.GetComponent<ParticleEmitter>().SetActive();
                    particlesDownTime = 0.0f;
                }
            }

            if (isFadingOut)
            {
                fadeOutTimer += dt;
                if (fadeOutTimer >= finalFadeOutDuration)
                {
                    Sound.StopAllSFX();
                    Scene.ChangeScene("Scene3");
                }
            }

            if (Input.IsKeyPressed(Keys.KEY_5))
            {
                Scene.ChangeScene("Scene3");
            }
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            timer += fixedDt;
        }

        public override void OnStateEnter(string stateName)
        {
            CMConsole.Log($"State : {stateName}");

            if (stateName == "ChangeImage")
            {
                if (nextCutsceneEntity == null)
                {
                    nextCutsceneEntity = CreateGameObject(cutscenePrefab);
                    currAlpha = 1.0f;

                    nextCutsceneEntity.GetComponent<Renderer>().ChangeTexture(nextImgName);
                }
            }
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
            if (stateName == "ChangeImage")
            {
                currAlpha -= fadeSpeed * dt;
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
                    Vector2 a = cutsceneEntity.Scale;
                    a.x += 0.01f * dt;
                    a.y += 0.01f * dt;
                    cutsceneEntity.Scale = a;
                    if (timer >= timeToSwapPanel)
                    {
                        ++currentPanel;
                        GetComponent<StateMachine>().SetStateCondition(2);
                    }
                }
                else
                {
                    if (currentPanel == 1 && !isFadingOut && timer >= timeToSwapPanel * 2)
                    {
                        isFadingOut = true;
                        fadeOutTimer = 0.0f;

                        Sound.SwitchBGM("zero", finalFadeOutDuration, finalFadeOutDuration, false);

                        // Start fading out the cutscene image
                        float alpha = cutsceneEntity.GetComponent<Renderer>().Red;
                        currAlpha = alpha;
                    }

                    if (isFadingOut)
                    {
                        currAlpha -= (dt / finalFadeOutDuration);
                        if (currAlpha < 0.0f) currAlpha = 0.0f;
                        cutsceneEntity.GetComponent<Renderer>().SetAlpha(currAlpha);
                    }
                }
            }
        }

        public override void OnStateExit(string stateName)
        {

        }
    }
}
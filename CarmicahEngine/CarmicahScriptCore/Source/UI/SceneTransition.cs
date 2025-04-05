using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class SceneTransition : Entity
    {
        public float targetAlpha = 1.0f;
        public float duration = 1.0f;
        public float timer = 0.0f;
        public bool fadeOut = true;
        public bool isWhite = false;
        public string targetScene = "";

        public override void OnCreate()
        {
            if (isWhite)
            {
                GetComponent<Renderer>().SetColour(1, 1, 1);
            }

            if (fadeOut)
            {
                GetComponent<Renderer>().SetAlpha(1.0f);
            }
            else
            {
                GetComponent<Renderer>().SetAlpha(0.0f);
            }
        }
        public override void OnUpdate(float dt)
        {
            timer += dt;
            if (!fadeOut)
            {
                float alpha = (timer / duration) * targetAlpha;
                GetComponent<Renderer>().SetAlpha(alpha);
                if (alpha >= targetAlpha)
                {
                    alpha = targetAlpha;
                    ChangeScene();
                    // end
                }
            }
            else
            {
                float alpha = targetAlpha - (timer / duration) * targetAlpha;
                GetComponent<Renderer>().SetAlpha(alpha);
                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                }
            }
        }

        public void FadeOut(string scene)
        {
            fadeOut = false;
            timer = 0.0f;
            targetScene = scene;
        }

        public void ChangeScene()
        {
            if (targetScene == "")
            {
                CMConsole.Log("ERROR: No target scene");
            }
            else
            {
                Scene.ChangeSceneReal(targetScene);
            }
        }

        
    }
}

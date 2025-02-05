using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class SceneChanger : Entity
    {
        
        float sceneChangeTimer = -1f;
        public string buttonType;
        string nextScene;

        public void SetScene(string sceneName, float delay = 1.0f)
        {
            sceneChangeTimer = delay;
            nextScene = sceneName;
        }

        void OnUpdate(float dt)
        {

            if (sceneChangeTimer > 0)
            {
                sceneChangeTimer -= dt;
                if (sceneChangeTimer <= 0)
                {
                    sceneChangeTimer = -1f;
                    if(nextScene == "quit")
                    {
                        Scene.CloseGame();
                    }
                    else
                    {

                        Scene.ChangeScene(nextScene);

                    }
                }
            }
        }

        void OnClick()
        {
            Console.WriteLine($"Testing Play Button {mID}");
            Sound.PlaySFX("SFX_Button",0.5f);
            if(buttonType.ToLower() == "play")
            {
                nextScene = "Scene1";

            }else if(buttonType.ToLower() == "quit")
            {
                nextScene = "quit";
            }
                SetScene(nextScene);
        }
    }
}

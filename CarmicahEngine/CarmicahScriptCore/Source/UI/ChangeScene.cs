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
        
        private static float sceneChangeTimer = -1f;
        private static string nextScene = "";

        public static void SetScene(string sceneName, float delay = 2.0f)
        {
            sceneChangeTimer = delay;
            nextScene = sceneName;
        }

        void OnUpdate(float dt)
        {
            if (sceneChangeTimer > 0)
            {
                Console.WriteLine("PLEASE");
                sceneChangeTimer -= dt;
                if (sceneChangeTimer <= 0)
                {
                    sceneChangeTimer = -1f;
                    Scene.ChangeScene(nextScene);
                }
            }
        }
    }
}

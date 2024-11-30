using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PlayButton : Entity
    {
        void OnClick()
        {
            Console.WriteLine($"Testing Play Button {mID}");
            //Entity sceneSystem = FindEntityWithName("SceneSystem");
            //if (sceneSystem != null)
            //{
                FunctionCalls.ChangeScene("Scene1");
           // }
            //std::string sceneName; 
            // this should be modular in the sense that when passed to function call changescene in scenesystem it should change the scene

            // call the wrapped function to change the scene
            // this should pass the string of scene name to function call changescene in scenesystem and change the scene 
            // but it isnt doing it somehow idk why
            //FunctionCalls.ChangeScene("Scene1");
        }
    }
}

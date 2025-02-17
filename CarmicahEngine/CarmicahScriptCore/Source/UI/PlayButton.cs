/* File Documentation ----------------------------------------------------------------------------- 
file:           PlayButton.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          This file defines the `PlayButton` class, a UI entity that triggers a scene change 
                when clicked. It demonstrates interaction with the `Scene` system to switch to a 
                specified scene within the Carmicah engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using Carmicah;
using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PlayButton : Entity
    {
        public string Type;
        
        void OnCreate()
        {
            CMConsole.Log("Hello");
        }
        void OnClick()
        {
            CMConsole.Log($"Testing Play Button {mID}");
            //Entity sceneSystem = FindEntityWithName("SceneSystem");
            //if (sceneSystem != null)
            //{
            //FunctionCalls.ChangeScene("Scene1");
            // }
            //std::string sceneName; 
            // this should be modular in the sense that when passed to function call changescene in scenesystem it should change the scene

            // call the wrapped function to change the scene
            // this should pass the string of scene name to function call changescene in scenesystem and change the scene 
            // but it isnt doing it somehow idk why
            //FunctionCalls.ChangeScene("Scene1");
            
            //SceneChanger.SetScene(nextScene, 2.0f);
            if(Type == "Play")
            {
                
                Sound.PlayBGM("BGM_SetupPhase_Mix1");
            }else
            {
                Sound.StopSoundBGM("BGM_SetupPhase_Mix1");
            }
        }
    }
}

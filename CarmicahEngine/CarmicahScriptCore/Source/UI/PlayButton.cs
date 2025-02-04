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
        void OnClick()
        {
            Console.WriteLine($"Testing Play Button {mID}");

            Scene.ChangeScene("Scene1");
        }
    }
}

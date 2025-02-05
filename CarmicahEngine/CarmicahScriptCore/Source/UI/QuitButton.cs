/* File Documentation ----------------------------------------------------------------------------- 
file:           QuitButton.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          This file defines the `QuitButton` class, a UI entity that logs its ID to the console 
                when clicked. It is intended for testing quit functionality within the Carmicah engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class QuitButton : Entity
    {

        void OnClick()
        {
            Sound.PlaySFX("SFX_Button");
            Scene.CloseGame();
            //Console.WriteLine($"Testing Button {mID}");
        }
    }
}

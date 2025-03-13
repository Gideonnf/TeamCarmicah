/* File Documentation ----------------------------------------------------------------------------- 
file:           CountDown.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu.sg

brief:          This file contains the Countdown class which is responsible for displaying the countdown
                animation before the game starts.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class MainMenuBGM : Entity
    {
        

        

        public override void OnCreate()
        {
            // display first animation
            Sound.PlayBGM("BGM_SetupPhase_Mix1", 0.4f);
        }


    }
}
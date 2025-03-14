/* File Documentation ----------------------------------------------------------------------------- 
file:           MainMenuBGM.cs

author:		    Lee Yong Yee (100%)

email:			l.yongyee@digipen.edu.sg

brief:          This file contains the MainMenuBGM class which is responsible for playing the main menu BGM
                when the scene starts

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
    public class FPSCounter : Entity
    {

        bool kPressed = false;


        public override void OnCreate()
        {
            // display first animation
            GetComponent<TextRenderer>().SetText("");
            
        }

        public override void OnUpdate(float dt)
        {
            if(Input.IsKeyPressed(Keys.KEY_K))
            {
                kPressed = !kPressed;
            }

            int fps = (int)CMTime.GetFPS();

            if (kPressed)
            {
                GetComponent<TextRenderer>().SetText(fps.ToString());
            }else
            {
                GetComponent<TextRenderer>().SetText("");
            }
        }


    }
}
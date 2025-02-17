/* File Documentation ----------------------------------------------------------------------------- 
file:           SettingButton.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu
  
brief:          This file defines the `SettingButton` class, derived from `Entity`. It represents a 
                UI button that logs its ID to the console when clicked. Intended for testing button 
                functionality within the Carmicah engine.

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
    public class SettingButton : Entity
    {
        public string SettingsMenu = "Settings_Menu";
        public string SettingsCloseButton = "Close_Button";
        public static bool IsCreated = false;
        bool hovering = false;

        void OnCreate()
        {
            Sound.PlayBGM("BGM_SetupPhase_Mix1", 0.4f);
        }
        void OnClick()
        {
            if (!IsCreated)
            {
                CreateGameObject(SettingsMenu);
                CreateGameObject(SettingsCloseButton);
                IsCreated = true;
            }
            CMConsole.Log($"Testing Button {mID}");
            Sound.PlaySFX("SFX_Button", 0.5f);
        }

        public void OnMouseHover()
        {


            if (!hovering && !IsCreated)
            {
                hovering = true;

                ChangeAnim("Button_HS_Settings");
            }
        }

        public void OnMouseClick()
        {
            hovering = false;
        }

        public void OnMouseExit()
        {
            hovering = false;
            if(!IsCreated)
            {
                
                ChangeAnim("Button_HE_Settings");

            }
        }
    }
}



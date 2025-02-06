/* File Documentation ----------------------------------------------------------------------------- 
file:           CreditsButton.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          This file defines the `CreditsButton` class, a UI entity that logs its ID to the console 
                when clicked. It is intended for testing CREDIT functionality within the Carmicah engine.

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
    public class CreditsButton : Entity
    {
        //test
        string Animation0 = "Button_Click_Credits";
        string Animation1 = "Button_Credits";
        bool hovering = false;
        private HealthSystem healthSystem;
        //private HealthSystem healthSystem;
        public string CreditsMenu = "Credits";
        public string CreditsCloseButton = "Close_Button_2";
        public bool IsCreated = false;

        public CreditsButton()
        {
            // init a HealthSystem instance with max health of 100
            //ealthSystem = new HealthSystem(100);
        }

        void OnClick()
        {
            // 1. pause game state
            // 2. open credits scene
            // Tell scene to open animation of the credits
            if (!IsCreated)
            {
                CreateGameObject(CreditsMenu);
                CreateGameObject(CreditsCloseButton);
            }

            // test the HealthSystem's TakeDamage function
            //healthSystem.TakeDamage(20); // Inflict 20 damage
            //Console.WriteLine($"Testing Button {mID}: Current Health is {healthSystem.CurrentHealth}");

            Console.WriteLine($"Testing Button {mID}");
            ChangeAnim(Animation0);
            //Console.WriteLine($"Testing Button {mID}");
            Sound.PlaySFX("SFX_Button", 0.5f);
        }

        public void OnMouseHover()
        {


            if (!hovering)
            {
                hovering = true;

                ChangeAnim(Animation1);
            }
        }

        public void OnMouseClick()
        {
            hovering = false;
        }

        public void OnMouseExit()
        {
            hovering = false;
            ChangeAnim("Bear_Climb");
        }
    }
}
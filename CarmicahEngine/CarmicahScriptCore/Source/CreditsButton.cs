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
        private HealthSystem healthSystem;

        public CreditsButton()
        {
            // Initialize a HealthSystem instance with max health of 100
            healthSystem = new HealthSystem(100);
        }

        void OnClick()
        {
            // 1. Pause game state
            // 2. Open Credits Scene
            // Tell scene to open animation of the credits

            // test the HealthSystem's TakeDamage function
            healthSystem.TakeDamage(20); // Inflict 20 damage
            Console.WriteLine($"Testing Button {mID}: Current Health is {healthSystem.CurrentHealth}");

            Console.WriteLine($"Testing Button {mID}");
        }
    }
}

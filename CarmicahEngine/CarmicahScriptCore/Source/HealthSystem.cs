/* File Documentation ----------------------------------------------------------------------------- 
file:           HealthSystem.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          This file defines the `HealthSystem` class, a system that manages the health of entities
                in the game. It is intended for testing health functionality within the Carmicah engine.

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
    // creat a HealthSystem class that inherits from Entity
    public class HealthSystem
    {
        // health vals
        private int mCurHealth;
        private int mMaxHealth = 100;

        // constructor
        public HealthSystem(int maxHealth)
        {
            // set health vals
            mCurHealth = maxHealth;
            mMaxHealth = maxHealth;
        }

        // variable to represent current and max health
        public int CurrentHealth => mCurHealth;
        public int MaxHealth => mMaxHealth;



        // function to take damage
        public void TakeDamage(int damage)
        {
            mCurHealth -= damage;

            Console.WriteLine($"entity has {mCurHealth} health left");

            // if health is less than 0, set it to 0
            if (mCurHealth < 0)
            {
                mCurHealth = 0;
            }

            if(mCurHealth == 0)
            {
                // call death function which doesn't do much right now
                OnDeath();
            }
        }

        // function for death
        public void OnDeath()
        {
            // log death
            Console.WriteLine("entity has no more health");

            // remove entity logic down here if need be
        }

        // function to adjust or set max health
        public void SetMaxHealth(int maxHealth)
        {
            // set max health
            mMaxHealth = maxHealth;

            // adjust curhealth if above new max health
            if (mCurHealth > mMaxHealth)
            {
                mCurHealth = mMaxHealth;
            }
        }

    }
}
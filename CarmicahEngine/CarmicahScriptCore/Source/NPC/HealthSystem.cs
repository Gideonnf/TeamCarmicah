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
    public class HealthSystem : Entity
    {
        // health vals
        public float mCurHealth;
        public float mMaxHealth = 100;

        Entity parentEntity;

        public float endPos = 0.0f;
        public float endScale = 4.4f;
        public float startPos = 0.0f;
        public float startScale = 0.0f;
        public float posDiff = 0.0f;

        // constructor
        //public HealthSystem(int maxHealth)
        //{
        //    // set health vals
        //    mCurHealth = maxHealth;
        //    mMaxHealth = maxHealth;
        //}

        // variable to represent current and max health
        //public int CurrentHealth => mCurHealth;
        //public int MaxHealth => mMaxHealth;

        public void OnCreate()
        {
            parentEntity = GetParent();
            SetMaxHealth(100);
            mCurHealth = 100;
            startPos = LocalPosition.x;
            startScale = Scale.x;
            posDiff = endPos - startPos;
        }

        public void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_Q))
            {
                //CMConsole.Log($"CurrHealth: {mCurHealth}");

                TakeDamage(20);
            }
        }

        // function to take damage
        public void TakeDamage(int damage)
        {
            if (mCurHealth <= 0) return;

            // subtract damage from current health
            mCurHealth -= damage;

            if (mCurHealth <= 0) { mCurHealth = 0; }

            // print out to console how much health is left
            // Console.WriteLine($"entity has {mCurHealth} health left");
            ///CMConsole.Log($"CurrHealth: {mCurHealth}");

            // health loss / max health
            float scalePercentage = (float)(mMaxHealth - mCurHealth) / (float)mMaxHealth;
            // start val + the value to scale up by
            float scaleVal = startScale + ((endScale - startScale) * scalePercentage);

            Scale = new Vector2(scaleVal, Scale.y);

            float posPercentage = (float)(posDiff) * scalePercentage;
            float posVal = startPos + posPercentage;

            //CMConsole.Log($"posPercentage: {posPercentage}, posVal: {posVal}");
           // CMConsole.Log($"Position: {Position.x}, {Position.y}");
            Position = new Vector2(posVal, LocalPosition.y);


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
            //Console.WriteLine("entity has died like a biatch");
            //Destroy();
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

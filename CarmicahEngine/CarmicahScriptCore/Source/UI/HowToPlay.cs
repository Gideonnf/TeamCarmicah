/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			HowToPlay.cs
  
 author:		

 email:		    

 brief:			

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class HowToPlay : Entity
    {
        public int currentPanel = 0;       // panel 0 ~ 5 (inclusive)
        Entity nextBtn = null;
        Entity backBtn = null;
        Entity playBtn = null;
       

        void OnCreate()
        {
            nextBtn = FindEntityWithName("HowToRightBtn");
            backBtn = FindEntityWithName("HowToLeftBtn");
            playBtn = FindEntityWithName("HowToPlayBtn");
        }

        private string GetPanelName(int pan)
        {
            if (pan < 6)
                return "Tutorial_Spritesheet_How " + pan;
            return "Tutorial_Spritesheet_How 0";
        }

        public void ProgressScene(int num)
        {
            // Delete The How To Prefab
            if(currentPanel != 4)
            {
                Entity toDestroy = FindEntityWithName("HowToStep" + currentPanel);
                if (toDestroy != null)
                {
                    toDestroy.Destroy();
                }
            }
            // Set buttons visibility if any
            if(currentPanel == 0)
            {
                backBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            }
            else if (currentPanel == 5)
            {
                backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                playBtn.GetComponent<Renderer>().SetAlpha(0.0f);
            }

            // Advance Panel
            currentPanel += num;
            GetComponent<Renderer>().ChangeTexture(GetPanelName(currentPanel));

            // If Overshot, end
            if(currentPanel >= 6)
            {
                Destroy();
                return;
            }
            // Return to first panel
            if(currentPanel <= 0)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(1.0f);
                backBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                currentPanel = 0;
            }
            // Last Panel
            else if(currentPanel == 5)
            {
                nextBtn.GetComponent<Renderer>().SetAlpha(0.0f);
                playBtn.GetComponent<Renderer>().SetAlpha(1.0f);
            }
            // Create Objs for every panel
            if(currentPanel != 4)
            {
                CreateGameObject("HowToStep" + currentPanel);
            }
        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyPressed(Keys.KEY_6))
            {
                ProgressScene(1);
            }
        }
    }
}

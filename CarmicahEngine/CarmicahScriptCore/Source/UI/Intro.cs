/* File Documentation ----------------------------------------------------------------------------- 
file:           Intro.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          

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
    public class Intro : Entity
    {
        public float timer = 0.0f;                           // track elapsed time
        public int state = 0;                              // track which image is currently displayed
        public const float maxDuration = 3.0f;               // duration for each image in seconds
        public const float FadeDuration = 1.0f;              // duration for fade in/out in seconds

        public string schoolLogoName = "digipen_logo";     // name of school logo entity
        public string schoolRightsName = "digipen_rights";   // name of school rights entity
        public string schoolBGName = "black_background"; // name of school background entity
        public string gameLogoName = "game_logo";        // name of game logo entity

        Entity schoolLogoEntity;                             // entity for school logo
        Entity schoolRightsEntity;                           // entity for school rights
        Entity schoolBGEntity;                               // entity for school background
        Entity gameLogoEntity;                               // entity for game logo

        void OnCreate()
        {
            schoolLogoEntity = CreateGameObject(schoolLogoName);
            schoolRightsEntity = CreateGameObject(schoolRightsName);
            schoolBGEntity = CreateGameObject(schoolBGName);
            gameLogoEntity = CreateGameObject(gameLogoName);


            // only the first logo is visible initially, don't touch fading for now
            /*if (schoolLogoEntity != null)
            {
                schoolLogoEntity.GetComponent<Sprite>().SetTransparency(1.0f);
            }
            if (gameLogoEntity != null)
            {
                gameLogoEntity.GetComponent<Sprite>().SetTransparency(0.0f);
            }*/

            // for update
            //float alpha = 1.0f - (fadeOutTime / FadeDuration);
            //schoolLogoEntity.GetComponent<Sprite>().SetTransparency(alpha);
        }

        void OnUpdate(float dt)
        {
            timer += dt;

            if (timer >= maxDuration) // Each state lasts 3 seconds
            {
                if (state == 0)
                {
                    // Destroy first set of objects
                    if (schoolLogoEntity != null) schoolLogoEntity.Destroy();
                    if (schoolRightsEntity != null) schoolRightsEntity.Destroy();
                    if (schoolBGEntity != null) schoolBGEntity.Destroy();

                    CMConsole.Log("Transitioning to state 1");
                    state = 1;
                }
                else if (state == 1)
                {
                    // Destroy game logo and move to the next scene
                    if (gameLogoEntity != null) gameLogoEntity.Destroy();

                    CMConsole.Log("Transitioning to Scene 3");
                    Scene.ChangeScene("Scene3");
                }

                timer = 0.0f; // Reset timer for the next state
            }
        }
    }
}
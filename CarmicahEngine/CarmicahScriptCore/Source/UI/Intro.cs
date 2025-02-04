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
        public float timer = 0.0f;                          // track elapsed time
        public int state = 0;                             // track which image is currently displayed
        public const float maxDuration = 3.0f;          // duration for each image in seconds
        public const float FadeDuration = 1.0f;             // duration for fade in/out in seconds

        public string schoolLogoName = "Digipen_Logo";    // name of school logo entity
        public string schoolRightsName = "Digipen_Rights";  // name of school rights entity
        public string schoolBGName = "black_background";// name of school background entity
        public string gameLogoName = "main_menu_bg";    // name of game logo entity

        Entity schoolLogoEntity;                            // entity for school logo
        Entity schoolRightsEntity;                          // entity for school rights
        Entity schoolBGEntity;                              // entity for school background
        Entity gameLogoEntity;                              // entity for game logo

        void OnCreate()
        {
            schoolLogoEntity = FindEntityWithName(schoolLogoName);
            schoolRightsEntity = FindEntityWithName(schoolRightsName);
            schoolBGEntity = FindEntityWithName(schoolBGName);
            gameLogoEntity = FindEntityWithName(gameLogoName);

            // only the first logo is visible initially, don't touch fading for now
            /*if (schoolLogoEntity != null)
            {
                schoolLogoEntity.GetComponent<Sprite>().SetTransparency(1.0f);
            }
            if (gameLogoEntity != null)
            {
                gameLogoEntity.GetComponent<Sprite>().SetTransparency(0.0f);
            }*/
        }

        void OnUpdate(float dt)
        {
            // timer will increase until max duration of 3.0f
            timer += dt;

            // display school logo and school rights
            if (state == 0)
            {
                // display school logo 
                if (timer < maxDuration)
                {
                    return;
                }

                // timer to track fade out time
                float fadeOutTime = timer - maxDuration;


                CMConsole.Log($"{fadeOutTime}");
                // fade out school logo and school rights
                if (/*fadeOutTime <= FadeDuration && */ schoolLogoEntity != null && schoolRightsEntity != null)
                {
                    //float alpha = 1.0f - (fadeOutTime / FadeDuration);
                    //schoolLogoEntity.GetComponent<Sprite>().SetTransparency(alpha);

                    //destroy school logo entity and school rights entity, this doesn't work for some reason
                    schoolLogoEntity.Destroy();
                    schoolRightsEntity.Destroy();
                    schoolBGEntity.Destroy();
                    CMConsole.Log($"I arrived here");
                }
                else
                {
                    // move to next state, this works
                    state = 1;
                    timer = 0.0f;
                }
            }
            // transition to game logo
            else if (state == 1)
            {
                if (/*timer <= FadeDuration && */timer == 3.0f && gameLogoEntity != null)
                {
                    //float alpha = timer / FadeDuration;
                    //gameLogoEntity.GetComponent<Sprite>().SetTransparency(alpha);

                    gameLogoEntity.Destroy();
                    CMConsole.Log($"I arrived here too");
                }
                else if (timer > FadeDuration + maxDuration)
                {
                    // transition to main menu, this works
                    Scene.ChangeScene("Scene3");
                }
            }
        }
    }
}
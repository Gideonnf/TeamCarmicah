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


            // set initial transparency
            if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
        }

        void OnUpdate(float dt)
        {
            timer += dt;
            float alpha = timer / maxDuration;

            if (state == 0)
            {
                // fade in at start
                if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(alpha);

                // once 3s pass, start fading out
                if (timer >= maxDuration)
                {
                    // fade out
                    float fadeOutAlpha = 1.0f - alpha;
                    if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(fadeOutAlpha);
                    if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(fadeOutAlpha);
                    if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(fadeOutAlpha);

                    // once fully faded out, destroy and move to state 1
                    if (fadeOutAlpha <= 0.0f)
                    {
                        //if (schoolLogoEntity != null) schoolLogoEntity.Destroy();
                        //if (schoolRightsEntity != null) schoolRightsEntity.Destroy();
                        //if (schoolBGEntity != null) schoolBGEntity.Destroy();

                        CMConsole.Log("Transitioning to state 1");
                        state = 1;
                        timer = 0.0f;
                    }
                }
            }
            else if (state == 1)
            {
                // fade in game logo
                if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);

                if (timer >= maxDuration)
                {
                    // fade out game logo
                    float fadeOutAlpha = 1.0f - alpha;
                    if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(fadeOutAlpha);

                    if (fadeOutAlpha <= 0.0f)
                    {
                        //if (gameLogoEntity != null) gameLogoEntity.Destroy();
                        CMConsole.Log("Transitioning to Scene 3");
                        Scene.ChangeScene("Scene3");
                    }
                }
            }
        }
    }
}
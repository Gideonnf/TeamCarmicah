/* File Documentation ----------------------------------------------------------------------------- 
file:           Intro.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          I hate that this took me so long.

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
        public int state = 0;                               // track which image is currently displayed
        public const float maxDuration = 3.0f;              // duration for each image in seconds
        public const float FadeDuration = 1.0f;             // duration for fade in/out in seconds

        public string schoolLogoName = "digipen_logo";      // name of school logo entity
        public string schoolRightsName = "digipen_rights";  // name of school rights entity
        public string schoolBGName = "black_background";    // name of school background entity
        public string gameLogoName = "game_logo";           // name of game logo entity
        public string teamLogoName = "team_logo";           // name of team logo entity

        Entity schoolLogoEntity;                            // entity for school logo
        Entity schoolRightsEntity;                          // entity for school rights
        Entity schoolBGEntity;                              // entity for school background
        Entity gameLogoEntity;                              // entity for game logo
        Entity teamLogoEntity;                              // entity for team logo

        void OnCreate()
        {
            schoolLogoEntity = CreateGameObject(schoolLogoName);
            schoolRightsEntity = CreateGameObject(schoolRightsName);
            schoolBGEntity = CreateGameObject(schoolBGName);
            gameLogoEntity = CreateGameObject(gameLogoName);
            teamLogoEntity = CreateGameObject(teamLogoName);


            // set initial transparency
            if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (teamLogoEntity != null) teamLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
        }

        void OnUpdate(float dt)
        {
            timer += dt;
            float halfDuration = maxDuration / 2.0f; // 1.5 sec
            float alpha;

            
            if (timer <= halfDuration)  // first 1.5s: fade in
            {
                alpha = timer / halfDuration;  // 0 to 1
            }
            else  // next 1.5s: fade out
            {
                alpha = 1.0f - ((timer - halfDuration) / halfDuration);  // 1 to 0
            }

            if (state == 0)
            {
                if (schoolLogoEntity != null)   schoolLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (schoolBGEntity != null)     schoolBGEntity.GetComponent<Renderer>().SetAlpha(alpha);
            }
            else if (state == 1)
            {
                if (teamLogoEntity != null) teamLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
            }
            else if (state == 2)
            {
                if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
            }

            // move to next state after 3 secs
            if (timer >= maxDuration)
            {
                CMConsole.Log($"Transitioning to state {state + 1}");
                state += 1;
                timer = 0.0f;

                // if final state, change scene, hardcoded values ik but idk 
                if (state == 3)
                {
                    Scene.ChangeScene("Scene3");
                }
            }
        }
     }
 }
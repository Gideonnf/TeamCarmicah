/* File Documentation ----------------------------------------------------------------------------- 
file:           Intro.cs

author:		    Micah Lim (100%)

email:			micahshengyao.lim@digipen.edu

brief:          I hate that this took me so long. This file defines the `Intro` class, a UI entity 
                that displays a series of images in sequence. It is intended for testing the 
                functionality of the Carmicah engine's UI system. It displays the DigiPen logo, 
                game logo, and team logo in sequence, fading in and out between each image.

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
        public const float FadeDuration = 1.0f;             // duration for fade in/out in seconds
        public const float HoldDuration = 2.0f;             // new hold time for state 0
        public const float maxDuration = 4.0f;              // total duration for state 0
        public const float otherStateDuration = 3.0f;       // duration for other states

        public string schoolLogoName = "digipen_logo";      // name of school logo entity
        public string schoolRightsName = "digipen_rights";  // name of school rights entity
        //public string schoolBGName = "black_background";    // name of school background entity
        public string gameLogoName = "game_logo";           // name of game logo entity
        public string teamLogoName = "team_logo";           // name of team logo entity

        Entity schoolLogoEntity;                            // entity for school logo
        Entity schoolRightsEntity;                          // entity for school rights
        //Entity schoolBGEntity;                              // entity for school background
        Entity gameLogoEntity;                              // entity for game logo
        Entity teamLogoEntity;                              // entity for team logo

        public override void OnCreate()
        {
            schoolLogoEntity = CreateGameObject(schoolLogoName);
            schoolRightsEntity = CreateGameObject(schoolRightsName);
            //schoolBGEntity = CreateGameObject(schoolBGName);
            gameLogoEntity = CreateGameObject(gameLogoName);
            teamLogoEntity = CreateGameObject(teamLogoName);

            // set initial transparency
            if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            //if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
            if (teamLogoEntity != null) teamLogoEntity.GetComponent<Renderer>().SetAlpha(0.0f);
        }

        public override void OnUpdate(float dt)
        {
            timer += dt;
            float alpha = 0.0f;

            if (state == 0)
            {
                if (timer <= FadeDuration)  // first 1s: fade in
                {
                    alpha = timer / FadeDuration;  // 0 to 1
                }
                else if (timer <= FadeDuration + HoldDuration) // next 2s: hold full visibility
                {
                    alpha = 1.0f;
                }
                else // last 1s: fade out
                {
                    alpha = 1.0f - ((timer - (FadeDuration + HoldDuration)) / FadeDuration);  // 1 to 0
                }

                if (schoolLogoEntity != null) schoolLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (schoolRightsEntity != null) schoolRightsEntity.GetComponent<Renderer>().SetAlpha(alpha);
               // if (schoolBGEntity != null) schoolBGEntity.GetComponent<Renderer>().SetAlpha(alpha);

                if (timer >= maxDuration)  // move to next state, game and team logo
                {
                    state += 1;
                    timer = 0.0f;
                }
            }
            else // other states remain the same
            {
                float halfDuration = otherStateDuration / 2.0f; // duration for fade in/out
                if (timer <= halfDuration)
                {
                    alpha = timer / halfDuration; // 0 to 1
                }
                else
                {
                    alpha = 1.0f - ((timer - halfDuration) / halfDuration); // 1 to 0
                }

                if (state == 1 && teamLogoEntity != null) teamLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);
                if (state == 2 && gameLogoEntity != null) gameLogoEntity.GetComponent<Renderer>().SetAlpha(alpha);

                if (timer >= otherStateDuration)
                {
                    state += 1;
                    timer = 0.0f;

                    if (state == 3)
                    {
                        Scene.ChangeScene("Scene3"); // go main menu
                    }
                }
            }
        }
    }
}
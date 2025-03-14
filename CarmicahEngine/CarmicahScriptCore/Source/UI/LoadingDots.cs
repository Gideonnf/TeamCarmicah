/* File Documentation ----------------------------------------------------------------------------- 
file:           LoadingDots.cs

author:		    Rainne Won (100%)

email:			@digipen.edu

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
    public class LoadingDots : Entity
    {
        public int maxLoops = 4;

        Entity Text;
        Entity cake1;
        Entity cake2;
        Entity cake3;
        Entity cake4;

        //float timeForOneLoop;
        //float animationTime = 0.0025f;
        int numberOfLoops = 0;
        bool cake1Created = false;
        bool cake2Created = false;
        bool cake3Created = false;
        bool cake4Created = false;
        bool thingHappened = false;


        float elapsedTime = 0.0f;
        int textProgress = 0;
        string currText = "";
        string finalText = "Tips: Use Buffs To Increase Your Chances Of Survival";
        public override void OnCreate()
        {
            //timeForOneLoop = GetComponent<Animation>().GetMaxTime();
            //CMConsole.Log($"{timeForOneLoop}");
            Text = FindEntityWithName("LoadingTipsText");
        }

        public override void OnUpdate(float dt)
        {
            elapsedTime += dt;
            float currFrameMaxTime = GetComponent<Animation>().GetFrameMaxTime() - 0.251f;
            if (currFrameMaxTime > 0.0f)
            {
                if (!thingHappened)
                {
                    if (!cake1Created)
                    {
                        cake1 = CreateGameObject("LoadingCake1");
                        cake1Created = true;

                    }
                    else if (!cake2Created)
                    {
                        cake2 = CreateGameObject("LoadingCake2");
                        cake2Created = true;
                    }
                    else if (!cake3Created)
                    {
                        cake3 = CreateGameObject("LoadingCake3");
                        cake3Created = true;
                    }
                    else if (!cake4Created)
                    {
                        cake4 = CreateGameObject("LoadingCake4");
                        cake4Created = true;
                    }
                    else
                    {
                        CMConsole.Log("DESTROY!");
                        cake1.Destroy();
                        cake2.Destroy();
                        cake3.Destroy();
                        cake4.Destroy();

                        cake1Created = false;
                        cake2Created = false;
                        cake3Created = false;
                        cake4Created = false;
                        numberOfLoops++;
                        Scene.ChangeScene("Scene1");
                    }
                    thingHappened = true;
                }
            }
            else thingHappened = false;

            if(elapsedTime > 0.05f)
            {
                if (textProgress != finalText.Length)
                {
                    currText += finalText[textProgress];
                    Text.GetComponent<TextRenderer>().SetText(currText);
                    ++textProgress;
                }
                //else
                //{
                //    currText = "";
                //    Text.GetComponent<TextRenderer>().SetText(currText);
                //    ++numberOfLoops;
                //    textProgress = 0;
                //}
                elapsedTime = 0.0f;
            }
        }
    }
}
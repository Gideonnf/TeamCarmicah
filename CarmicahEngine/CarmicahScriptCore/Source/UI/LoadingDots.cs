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
        bool cake1Created = false;
        bool cake2Created = false;
        bool cake3Created = false;
        bool cake4Created = false;
        List<int> cakeKeyNum;

        float finalCountdown = 0.0f;
        const float totalFinal = 0.5f;
        float elapsedTime = 0.0f;
        int textProgress = 0;
        string currText = "";
        string finalText = "Tips: Use Buffs To Increase Your Chances Of Survival";
        public override void OnCreate()
        {
            //timeForOneLoop = GetComponent<Animation>().GetMaxTime();
            //CMConsole.Log($"{timeForOneLoop}");
            Text = FindEntityWithName("LoadingTipsText");
            Sound.PlaySFX("loading", 1.0f);
            cakeKeyNum = new List<int>();
            cakeKeyNum.Add(0);
            cakeKeyNum.Add(9);
            cakeKeyNum.Add(18);
            cakeKeyNum.Add(23);
        }

        public override void OnUpdate(float dt)
        {
            int frameNo = GetComponent<Animation>().GetFrameNo();
            if (!cake1Created)
            {
                if (frameNo >= cakeKeyNum[0])
                {
                    cake1 = CreateGameObject("LoadingCake1");
                    cake1Created = true;
                }
            }
            else if (!cake2Created)
            {
                if (frameNo >= cakeKeyNum[1])
                {
                    cake2 = CreateGameObject("LoadingCake2");
                    cake2Created = true;
                }
            }
            else if (!cake3Created)
            {
                if (frameNo >= cakeKeyNum[2])
                {
                    cake3 = CreateGameObject("LoadingCake3");
                    cake3Created = true;
                }
            }
            else if (!cake4Created)
            {
                if (frameNo >= cakeKeyNum[3])
                {
                    cake4 = CreateGameObject("LoadingCake4");
                    cake4Created = true;
                }
            }
            else if (textProgress == finalText.Length)
            {
                finalCountdown += dt;
                if(finalCountdown > totalFinal)
                {
                    FindEntityWithName("SceneTransition").As<SceneTransition>().FadeOut("Scene1");
                    //Scene.ChangeScene("Scene1");
                }
            }
            
            elapsedTime += dt;
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
/* File Documentation ----------------------------------------------------------------------------- 
file:           LoadingBackground.cs

author:		    Nicholas Lai (100%)

email:			n.lai@digipen.edu

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
        

        Entity cake1;
        Entity cake2;
        Entity cake3;
        Entity cake4;

        float timeForOneLoop;
        float animationTime = 0.0025f;
        float elapsedTime = 0.0f;
        int numberOfLoops = 0;
        bool cake1Created = false;
        bool cake2Created = false;
        bool cake3Created = false;
        bool cake4Created = false;
        void OnCreate()
        {
            timeForOneLoop = GetComponent<Animation>().GetMaxTime();
            CMConsole.Log($"{timeForOneLoop}");
        }

        void OnUpdate(float dt)
        {
            elapsedTime += dt;

            //float quarterTime = timeForOneLoop / 4;


            if (elapsedTime >= 0.5 && !cake1Created)
            {
                cake1 = CreateGameObject("LoadingCake1");
                cake1Created = true;
                
            }

            if (elapsedTime >= 0.775 && !cake2Created)
            {
                cake2 = CreateGameObject("LoadingCake2");
                cake2Created = true;
            }

            if (elapsedTime >= 1.0 && !cake3Created)
            {
                cake3 = CreateGameObject("LoadingCake3");
                cake3Created = true;
            }

            if (elapsedTime >= 1.075 && !cake4Created)
            {
                cake4 = CreateGameObject("LoadingCake4");
                cake4Created = true;
            }

            if (elapsedTime >= timeForOneLoop)
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
                elapsedTime = 0.0f;
            }
        }
    }
}
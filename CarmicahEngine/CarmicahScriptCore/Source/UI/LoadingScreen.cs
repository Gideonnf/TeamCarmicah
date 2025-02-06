/* File Documentation ----------------------------------------------------------------------------- 
file:           LoadingScreen.cs

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
    public class LoadingScreen : Entity
    {
        //float timer = 0.0f;
        //public string PolkaDotAnim = "Polka_Dots";
        //bool isCreated = false;
        // changeAnim(PolkaDotAnim)

        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            //timer += dt;

            //if (!isCreated)
            //{
            //    CreateGameObject(PolkaDotAnim);
            //    isCreated = true;
            //}

            //ChangeAnim(PolkaDotAnim);
            //if (timer >= 2.5f)
            //{
            //    Scene.ChangeScene("Scene3");
            //}

        }
    }
}
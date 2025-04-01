/* File Documentation ----------------------------------------------------------------------------- 
file:           MouseParticles.cs

author:		    

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
    public class MouseParticles : Entity
    {
        Vector2 mousePosOffset;
        
        public override void OnCreate()
        {
            FunctionCalls.Transform_GetRenderingScale(mID, out mousePosOffset);
            mousePosOffset.y = mousePosOffset.y * 0.5f + 0.015f;
            mousePosOffset.x = -mousePosOffset.x * 0.5f + 0.08f;
        }

        public override void OnUpdate(float dt)
        {
            Position = Input.GetMousePos() - new Vector2(mousePosOffset.x * Scale.x, mousePosOffset.y * Scale.y);
        }
    }
}
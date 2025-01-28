/* File Documentation ----------------------------------------------------------------------------- 
file:           MouseState.cs

author:		    Gideon Francis (100%)

email:			g.francis@digipen.edu

brief:          This file defines the `MouseChase` and `MouseDead` classes, which are states for 
                managing mouse behavior in a game. The `MouseChase` state simulates movement toward 
                a target position, while the `MouseDead` state provides a placeholder for handling 
                death-related logic for mouse entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{

    public class MouseChase : State
    {
        public MouseChase(string m_stateID) : base(m_stateID) { }
        //Vector2 targetPos;

        public override void Enter()
        {
           // targetPos = new Vector2(0, 4.0f);
        }

        public override void Update(ref StateMachine sm)
        {
            // Move to the target position
            //Vector2f dir = targetPos;

            // If the distance to target position is less than equal to like 0.5 or smth
            // then set the state to dead
           // sm.SetNextState("MouseDead");
        }

        public override void Exit() 
        {
        
        }
    }

    public class MouseDead : State
    {
        // atm mouse dead wont do anything
        // its just so i can call destroy gameobject in mouseAI script
        
        public MouseDead(string m_stateID) : base(m_stateID) { }

        public override void Enter()
        {
            // Can play death sound or animation here

        }

        public override void Update(ref StateMachine sm)
        {
            // Reset handled in MouseAI's OnUpdate
        }

        public override void Exit()
        {
            // Can play respawn sound or animation here

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class MouseAI : Entity
    {
        StateMachine mouseSM;

        void OnCreate()
        {
            MouseChase chaseState = new MouseChase("MouseChase");
            MouseChase deadState = new MouseChase("MouseDead");

            mouseSM.AddState(chaseState);
            mouseSM.AddState(deadState);
        }

        void OnUpdate(float dt)
        {
            mouseSM.Update(ref mouseSM);

            // If its dead, kill it
            if (mouseSM.GetCurrentState() == "MouseDead")
            {
                // kill it
            }
        }

    }
}

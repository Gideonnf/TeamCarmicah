using CarmicahScriptCore.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Camera : Entity
    {
        
        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            if (Input.IsKeyHold(Keys.KEY_R))
            {
                Vector2 pos = GetComponent<Transform>().Position;
                pos.y += 4 * dt;
                GetComponent<Transform>().Position = pos;
            }
            
            if (Input.IsKeyHold(Keys.KEY_F))
            {
                Vector2 pos = GetComponent<Transform>().Position;
                pos.y -= 4 * dt;
                GetComponent<Transform>().Position = pos;
            }
        }
    }
}

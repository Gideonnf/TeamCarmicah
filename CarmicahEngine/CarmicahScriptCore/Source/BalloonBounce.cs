using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class BalloonBounce : Entity
    {
        public float bounceSpeed = 2.0f; // How fast it bounces
        public float YOffset = 1.0f; // The height 
        public float holdTimer = 0.2f; // For how long it'll hold a lil bit at the top or bottom
        float maxHeight;
        float origHeight;
        float timer = 0.0f;
        bool deBounce = false;
        bool freeze = false;

        void OnCreate()
        {
            maxHeight = Position.y + YOffset;
            origHeight = Position.y;
        }

        void OnUpdate(float dt)
        {
            if (!freeze)
            {
                if (deBounce)
                {
                    Vector2 Pos = Position;
                    Pos.y -= bounceSpeed * dt;
                    Position = Pos;
                }
                else
                {
                    Vector2 Pos = Position;
                    Pos.y += bounceSpeed * dt;
                    Position = Pos;
                }
            }

            if (Position.y >= maxHeight)
            {
                deBounce = true;
                freeze = true;
            }
            else if (Position.y <= origHeight)
            {
                deBounce = false;
                freeze = true;
            }

            if(freeze)
            {
                timer += dt;
                if (timer >= holdTimer)
                {
                    freeze = false;
                    timer = 0.0f;
                }
            }

        }


    }
}

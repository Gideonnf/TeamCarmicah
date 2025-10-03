using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class LanternFloater : Entity
    {
        public float floatSpeed = 1.0f; // How fast it floats up
        public float maxHeight = 30.0f;
        public float startHeight = 0.0f;

        public override void OnUpdate(float dt)
        {
            Vector2 Pos = Position;
            Pos.y += floatSpeed * dt;

            if (Pos.y > maxHeight)
            {
                Pos = new Vector2(Pos.x, startHeight);
            }

            Position = Pos;
        }

    }
}

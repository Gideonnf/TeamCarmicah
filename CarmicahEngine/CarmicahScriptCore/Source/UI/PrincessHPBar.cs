using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PrincessHPBar : Entity
    {
        public float percentHP;
        private float lastKnownHPPercent = 100.0f;

        public override void OnCreate()
        {
        }

        public override void OnUpdate(float dt)
        {
            if(Math.Abs(percentHP - lastKnownHPPercent) > 0)
            {
                uint child = FunctionCalls.Entity_GetChild(mID);
                Entity childEntity = new Entity(child);

                float percentage = percentHP / 100.0f;

                childEntity.Scale = new Vector2(percentage, 1.0f);
                childEntity.LocalPosition = new Vector2(-(1.0f - percentage) * 178.0f, 0);
                lastKnownHPPercent = percentHP;
            }
        }
    }
}

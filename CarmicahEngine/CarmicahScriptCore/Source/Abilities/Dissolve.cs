using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Dissolve : Entity
    {
        public override void OnUpdate(float dt)
        {
            if (HasComponent<Animation>())
            {
                if (GetComponent<Animation>().IsAnimFinished())
                {
                    Destroy();
                }
            }
        }
    }
}

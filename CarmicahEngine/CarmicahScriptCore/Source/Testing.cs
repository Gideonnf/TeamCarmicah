using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Testing : Entity
    {
        public float testVar;
        void OnCreate()
        {
            Console.WriteLine($"On Create {mID}");
        }

        void OnUpdate(float dt)
        {
            Console.WriteLine($"On Update {mID}");

        }

    }
}

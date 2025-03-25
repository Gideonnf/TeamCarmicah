using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class BaseTest : Entity
    {
        public int temp = 0;
        public virtual void GetTarget()
        {
          //  CMConsole.Log("Getting base target");
        }

        public virtual void Shoot()
        {
            //CMConsole.Log("Shooting base");
        }
    }
}

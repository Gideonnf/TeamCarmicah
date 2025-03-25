using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TestScript : BaseTest
    {
        public override void OnCreate()
        {
            temp = 5;
            //CMConsole.Log("Test SCript Create");
            base.OnCreate();
            GetTarget();
            Shoot();
        }
        public override void GetTarget()
        {
            base.GetTarget();
            CMConsole.Log("Getting test script target");
        }

        public override void Shoot()
        {
            base.Shoot();
           // CMConsole.Log("Shooting testCript");
        }
    }
}

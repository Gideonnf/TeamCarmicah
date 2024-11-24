using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class SettingButton : Entity
    {

        void OnClick()
        {
            Console.WriteLine($"Testing Button {mID}");
        }
    }
}

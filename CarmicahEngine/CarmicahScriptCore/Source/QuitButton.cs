using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class QuitButton : Entity
    {

        void OnClick()
        {
            Console.WriteLine($"Testing Button {mID}");
        }
    }
}

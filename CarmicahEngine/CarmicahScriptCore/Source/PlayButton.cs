using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PlayButton : Entity
    {
        void OnClick()
        {
            Console.WriteLine($"Testing Play Button {mID}");

            // call the wrapped function to change the scene
            FunctionCalls.ChangeScene("Scene1");
        }
    }
}

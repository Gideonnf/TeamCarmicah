using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Input
    {
        public static bool IsKeyPressed(Keys keyCode)
        {
            //Console.WriteLine("KeyPressed");
            return FunctionCalls.IsKeyPressed(keyCode);
        }

        public static bool IsKeyHold(Keys keyCode)
        {
            return FunctionCalls.IsKeyHold(keyCode);
        }
    }
}

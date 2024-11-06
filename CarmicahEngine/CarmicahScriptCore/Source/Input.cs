using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScriptCore.Source
{
    public class Input
    {
        public static bool IsKeyPressed(Keys keyCode)
        {
            return FunctionCalls.IsKeyPressed(keyCode);
        }
    }
}

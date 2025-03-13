using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public static class CMTime
    {
        public static double GetFPS()
        {
            return FunctionCalls.Time_GetFPS();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Sound
    {
        public static void PlaySFX(string soundName)
        {
            FunctionCalls.Sound_PlaySFX(soundName);
        }
    }
}

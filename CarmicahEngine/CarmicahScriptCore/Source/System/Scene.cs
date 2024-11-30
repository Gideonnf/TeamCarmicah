using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScriptCore.Source
{
    public class Scene
    {
        public static bool ChangeScene(string sceneName)
        {
            // can do some kinda console log stuff here
           if(FunctionCalls.ChangeScene(sceneName))
            {
            }
           else
            {

            }

            return true;
        }
    }
}

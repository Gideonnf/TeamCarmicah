﻿using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Scene
    {
        //public static bool ChangeScene(string sceneName)
        //{
        //    // can do some kinda console log stuff here
        //   if(FunctionCalls.ChangeScene(sceneName))
        //    {
        //    }
        //   else
        //    {

        //    }

        //    return true;
        //}

        public static bool ChangeScene(string sceneName)
        {

            FunctionCalls.ChangeScene(sceneName);
            return true;
        }

        public static void CloseGame()
        {
            FunctionCalls.CloseGame();
        }

        public static bool IsDoneLoading()
        { 
            return FunctionCalls.IsDoneLoading(); 
        }
    }
}

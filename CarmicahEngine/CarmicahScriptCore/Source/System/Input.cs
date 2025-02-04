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

        public static bool IsMousePressed(MouseButtons button)
        {
            return FunctionCalls.IsMousePressed(button);
        }

        public static bool IsMouseReleased(MouseButtons button)
        {
            return FunctionCalls.IsMouseReleased(button);
        }

        public static Vector2 GetMousePos()
        {
            FunctionCalls.GetMousePos(out Vector2 mousePos);

            return mousePos;
        }
    }
}

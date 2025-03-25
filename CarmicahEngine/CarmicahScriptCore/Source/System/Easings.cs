using System;
using System.IO;
using System.Collections.Generic;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;

// Formula from https://easings.net/

namespace Carmicah
{
    public enum SLIDE_CURVE
        {
            LINEAR = 0,
            QUADRATIC,
            CUBIC,
            QUART,
            SINE,
            EXPO,
            CIRC,
            BOUNCE_OUT,
            BOUNCE,
            BACK,
            ELASTIC
        }
    public class Easings
    {
        public static Vector2 GetInterpolate(SLIDE_CURVE curve, Vector2 start, Vector2 end, float percent)
        {
            percent = GetPercentage(curve, percent, 1.0f);
            return start + (end - start) * percent;
        }

        public static float GetPercentage(SLIDE_CURVE curve, float t, float totalTime)
        {
            double x = t / totalTime;
            if (x >= 1.0f)
                return 1.0f;
            else if (x <= 0.0f)
                return 0.0f;

            switch (curve)
            {
                case SLIDE_CURVE.QUADRATIC:
                    if (x < 0.5)
                        x = 2.0 * x * x;
                    else
                        x = 1.0 - Math.Pow(-2.0 * x + 2.0, 2.0) / 2.0;
                    break;
                case SLIDE_CURVE.CUBIC:
                    if (x < 0.5)
                        x = 4.0 * x * x * x;
                    else
                        x = 1.0 - Math.Pow(-2.0 * x + 2.0, 3.0) / 2.0;
                    break;
                case SLIDE_CURVE.QUART:
                    if (x < 0.5)
                        x = 8.0 * x * x * x;
                    else
                        x = 1.0 - Math.Pow(-2.0 * x + 2.0, 4.0) / 2.0;
                    break;
                case SLIDE_CURVE.SINE:
                    x = -(Math.Cos(Math.PI * x) - 1.0) / 2.0;
                    break;
                case SLIDE_CURVE.EXPO:
                    if (x < 0.5)
                        x = Math.Pow(2.0, 20.0 * x - 10.0) / 2.0;
                    else
                        x = 1.0 - Math.Pow(2.0, -20.0 * x + 10.0) / 2.0;
                    break;
                case SLIDE_CURVE.CIRC:
                    if (x < 0.5)
                        x = 0.5 - Math.Sqrt(1.0 - Math.Pow(2.0 * x, 2.0)) / 2.0;
                    else
                        x = 0.5 + Math.Sqrt(1.0 - Math.Pow(-2.0 * x + 2.0f, 2.0)) / 2.0;
                    break;
                case SLIDE_CURVE.BOUNCE_OUT:
                    {
                        const double n = 7.5625;
                        const double d = 2.75;
                        if (x < 1 / d)
                            x = n * x * x;
                        else if (x < 2 / d)
                        {
                            x -= 1.5;
                            x = n * (x / d) * x + 0.75;
                        }
                        else if (x < 2.5 / d)
                        {
                            x -= 2.25;
                            x = n * (x / d) * x + 0.9375;
                        }
                        else
                        {
                            x -= 2.625;
                            x = n * (x / d) * x + 0.984375;
                        }
                    }
                    break;
                case SLIDE_CURVE.BOUNCE:
                    if (x < 0.5)
                        x = 0.5 - GetPercentage(SLIDE_CURVE.BOUNCE_OUT, (float)(1.0 - 2.0 * x), 1.0f) / 2.0;
                    else
                        x = 0.5 + GetPercentage(SLIDE_CURVE.BOUNCE_OUT, (float)(2.0 * x - 1.0), 1.0f) / 2.0;
                    break;
                case SLIDE_CURVE.BACK:
                    {
                        const double c = 1.70158 * 1.525;

                        if (x < 0.5)
                            x = Math.Pow(2.0 * x, 2.0) * ((c + 1.0) * 2.0 * x - c) / 2.0;
                        else
                            x = 1.0 + Math.Pow(2.0 * x - 2.0, 2.0) * ((c + 1.0) * (2.0 * x - 2) + c) / 2.0;
                    }
                    break;
                case SLIDE_CURVE.ELASTIC:
                    {
                        const double c = (2 * Math.PI) / 4.5;
                        if (x < 0.5)
                            x = -Math.Pow(2.0, 20.0 * x - 10.0) * Math.Sin((20.0 * x - 11.125) * c) / 2.0;
                        else
                            x = 1.0 + (Math.Pow(2.0, -20.0 * x + 10.0) * Math.Sin((20.0 * x - 11.125) * c)) / 2.0;
                    }
                    break;
            }
            return (float)x;
        }
    }
}

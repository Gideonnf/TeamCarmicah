using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{ 
    public class CMRand
    {
        static Random random  = new Random();

        static public float Range(float minVal, float maxVal)
        {
            return (float)(random.NextDouble() * (maxVal - minVal) + minVal);
        }

       static  public double Range(double minVal, double maxVal)
        {
            return (random.NextDouble() * (maxVal - minVal) + minVal);
        }

        static public int Range(int minVal, int maxVal)
        {
            return random.Next(minVal, maxVal);
        }
    }
}


using System;

namespace Carmicah
{
    public struct Vector2
    {
        public float x, y;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public Vector2(float val)
        {
            this.x = val;
            this.y = val;
        }

        public static Vector2 Zero => new Vector2(0.0f, 0.0f);

        public static Vector2 operator -(Vector2 v1, Vector2 v2)
        {
            return new Vector2(v1.x - v2.x, v1.y - v2.y);
        }

        public Vector2 Normalize()
        {
            float magnitude = (float)Math.Sqrt(x * x + y * y);
            if (magnitude > 0)
            {
                return new Vector2(x / magnitude, y / magnitude);
            }
            else
            {
                return Vector2.Zero;
            }
        }

        public float Distance(Vector2 other)
        {
            float dx = other.x - x;
            float dy = other.y - y;
            return (float)Math.Sqrt(dx * dx + dy * dy);
        }
    }
}

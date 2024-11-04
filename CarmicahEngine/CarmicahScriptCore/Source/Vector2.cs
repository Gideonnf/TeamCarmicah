
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


    }
}

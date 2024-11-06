using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public class Transform : Component
    {
        public Vector2 Scale
        {
            get
            {
                FunctionCalls.Transform_GetScale(Entity.mID, out Vector2 scale);
                return scale;
            }
            set
            {
                FunctionCalls.Transform_SetScale(Entity.mID, ref value);
            }
        }
    }

    public class RigidBody : Component
    {
        public void ApplyForce(Vector2 direction, float magnitude, float lifeTime)
        {

        }

        public void ApplyForce(Vector2 direction, float magnitude)
        {

        }
    }
}

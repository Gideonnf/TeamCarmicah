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

    public class TransformComponent : Component
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
}

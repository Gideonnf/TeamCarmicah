using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
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

        public Vector2 Position
        {
            get
            {
                //Console.WriteLine($"Entity ID:{Entity.mID}");
                FunctionCalls.Transform_GetPosition(Entity.mID, out Vector2 position);
                return position;
            }
            set
            {
                //Console.WriteLine($"Entity ID:{Entity.mID}");

                FunctionCalls.Transform_SetPosition(Entity.mID, ref value);
            }
        }

        public float Depth
        {
            get
            {
                FunctionCalls.Transform_GetDepth(Entity.mID, out float depth);
                return depth;
            }
            set
            {
                FunctionCalls.Transform_SetDepth(Entity.mID, ref value);
            }
        }
    }

    public class RigidBody : Component
    {
        public void ApplyForce(Vector2 direction, float magnitude, float lifeTime)
        {
            FunctionCalls.RigidBody_ApplyForceWithTime(Entity.mID,  direction, magnitude,  lifeTime);
        }

        public void ApplyForce(Vector2 direction, float magnitude)
        {
            FunctionCalls.RigidBody_ApplyForce(Entity.mID, direction, magnitude);
        }
    }

    public class Animation : Component
    {
        public void ChangeAnim(string animName)
        {
            FunctionCalls.Animation_ChangeAnim(Entity.mID, animName);
        }
    }
}

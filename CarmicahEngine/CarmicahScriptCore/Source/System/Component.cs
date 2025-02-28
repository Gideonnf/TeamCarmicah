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

        public float Rot
        {
            get
            {
                FunctionCalls.Transform_GetRot(Entity.mID, out float rot);
                return rot;
            }
            set
            {
                FunctionCalls.Transform_SetRot(Entity.mID, ref value);
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

        public void StopForces()
        {
            FunctionCalls.RigidBody_StopForces();
        }
    }

    public class Animation : Component
    {
        public void ChangeAnim(string animName)
        {
            FunctionCalls.Animation_ChangeAnim(Entity.mID, animName);
        }

        public float GetMaxTime()
        {
            //FunctionCalls.Transform_SetPosition(Entity.mID, ref value);
            return FunctionCalls.GetMaxTime(Entity.mID);
        }

        public float GetFrameMaxTime()
        {
            return FunctionCalls.Animation_GetCurrFrameTime(Entity.mID);
        }
    }

    public class StateMachine : Component
    {
        public void SetStateCondition(object condition)
        {
            FunctionCalls.SetStateCondition(Entity.mID, condition);
        }

        public float GetStateTimer()
        {
           return FunctionCalls.GetStateTimer(Entity.mID);
        }
    }

    public class Renderer : Component
    {
        public void SetAlpha(float val)
        {
            FunctionCalls.SetAlpha(Entity.mID, val);
        }

        public void ChangeTexture(string textureName)
        {
            FunctionCalls.ChangeTexture(Entity.mID, textureName);
        }

        public void SetColour(float r, float g, float b)
        {
            FunctionCalls.SetColour(Entity.mID, r, g, b);
        }

        public float Red
        {
            get
            {
                FunctionCalls.GetRedColour(Entity.mID, out float red);
                return red;
            }
            set
            {
                FunctionCalls.SetRedColour(Entity.mID, ref value);
            }
        }

        //public float Blue
        //{
        //    get
        //    {
        //        FunctionCalls.GetBlueColour(Entity.mID, out float blue);
        //        return blue;
        //    }
        //    set
        //    {
        //        FunctionCalls.SetBlueColour(Entity.mID, ref value);
        //    }
        //}

        //public float Green
        //{
        //    get
        //    {
        //        FunctionCalls.GetRedColour(Entity.mID, out float red);
        //        return red;
        //    }
        //    set
        //    {
        //        FunctionCalls.SetRedColour(Entity.mID, ref value);
        //    }
        //}
    }

    public class TextRenderer : Component
    {
        public void SetText(string text)
        {
            FunctionCalls.ChangeText(Entity.mID, text);
        }
    }
}

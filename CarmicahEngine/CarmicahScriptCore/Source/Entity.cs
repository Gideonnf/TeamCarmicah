using System;
using System.Runtime.CompilerServices;


namespace Carmicah
{
    public class Entity
    {
        public uint mID;

        protected Entity() { mID = 0; }

        protected Entity(uint id) { mID = id; }

        public Vector2 Scale
        {
            get
            {
                FunctionCalls.Transform_GetScale(mID, out Vector2 scale);
                return scale;
            }
            set
            {
                FunctionCalls.Transform_SetScale(mID, ref value);
            }
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return FunctionCalls.Entity_HasComponent(mID, componentType);
        }

        //public T GetComponent<T>() 
    }
}

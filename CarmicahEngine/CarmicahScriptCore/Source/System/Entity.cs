using System;
using System.Runtime.CompilerServices;


namespace Carmicah
{
    public class Entity
    {
        public uint mID;

        protected Entity() { mID = 0; }

        internal Entity(uint id) { mID = id; }

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

        public Vector2 Position
        {
            get
            {
                FunctionCalls.Transform_GetPosition(mID, out Vector2 position);
                return position;
            }
            set
            {
                FunctionCalls.Transform_SetPosition(mID, ref value);
            }
        }

        public float Depth
        {
            get
            {
                FunctionCalls.Transform_GetDepth(mID, out float depth);
                return depth;
            }
            set
            {
                FunctionCalls.Transform_SetDepth(mID, ref value);
            }
        }


        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return FunctionCalls.Entity_HasComponent(mID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            // if theres no component, return null
            if (!HasComponent<T>()) return null;

            T component = new T() { Entity = this };
            return component;
        }

        public T As<T>() where T : Entity, new()
        {
            Object scriptInstance = FunctionCalls.GetScriptInstance(mID);

            return scriptInstance as T;
        }

        public Entity FindEntityWithName(string name)
        {
            uint entityID = FunctionCalls.Entity_FindEntityWithName(name);
            
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        public Entity CreateGameObject(string prefabName)
        {
            Entity newEntity = new Entity(FunctionCalls.CreateNewGameObject(prefabName));

            return newEntity;
        }

        public void Destroy()
        {
            FunctionCalls.Destroy(mID);
        }

        //public T GetComponent<T>() 
    }
}

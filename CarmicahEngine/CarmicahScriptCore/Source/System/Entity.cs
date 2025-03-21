using System;
using System.Runtime.CompilerServices;


namespace Carmicah
{
    public class Entity
    {
        public uint mID;

        protected Entity() { mID = 0; }
        ~Entity() { mID = 0; }

        public virtual void OnCreate() { }

        public virtual void OnUpdate(float dt) { }

        public virtual void OnFixedUpdate(float fixedDt) { }

        public virtual void OnCollide(uint collidedEntity) { }

        public virtual void OnTriggerEnter(uint collidedEntity) { }

        public virtual void OnTriggerStay(uint collidedEntity) { }

        public virtual void OnTriggerExit() { }

        public virtual void OnMouseEnter() { }

        public virtual void OnMouseHover() { }

        public virtual void OnMouseExit() { }

        public virtual void OnClick() { }

        public virtual void OnStateEnter(string stateName) { }

        public virtual void OnStateUpdate(string stateName, float dt) { }

        public virtual void OnStateExit(string stateName) { }

        internal Entity(uint id) { mID = id; }

        public Vector2 Scale
        {
            get
            {
               // Console.WriteLine($"Scale Get ID:{mID}");

                FunctionCalls.Transform_GetScale(mID, out Vector2 scale);
                return scale;
            }
            set
            {
               // Console.WriteLine($"Scale SetEntity ID:{mID}");

                FunctionCalls.Transform_SetScale(mID, ref value);
            }
        }

        public Vector2 Position
        {
            get
            {
                //Console.WriteLine($"Transform GetEntity ID:{mID}");

                FunctionCalls.Transform_GetPosition(mID, out Vector2 position);
                return position;
            }
            set
            {
               // Console.WriteLine($"Transform Set Entity ID:{mID}");

                FunctionCalls.Transform_SetPosition(mID, ref value);
            }
        }

        public Vector2 LocalPosition
        {
            get
            {
                //Console.WriteLine($"Transform GetEntity ID:{mID}");

                FunctionCalls.Transform_GetLocalPosition(mID, out Vector2 position);
                return position;
            }
            set
            {
                // Console.WriteLine($"Transform Set Entity ID:{mID}");

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

        public float Rot
        {
            get
            {
                FunctionCalls.Transform_GetRot(mID, out float rot);
                return rot;
            }
            set
            {
                FunctionCalls.Transform_SetRot(mID, ref value);
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

        public T GetComponentInChildren<T>() where T : Component, new()
        {
            uint child = FunctionCalls.Entity_GetChild(mID);
            Entity childEntity = new Entity(child);

            if (!childEntity.HasComponent<T>()) return null;

            T component = new T() { Entity = childEntity };

            return component;
        }

        public T AsChild<T>() where T : Entity, new()
        {
            Object scriptInstance = FunctionCalls.GetScriptInstanceFromChildren(mID);

            return scriptInstance as T;
        }

        public T As<T>() where T : Entity, new()
        {
            string baseClassName = typeof(T).Name;
            //CMConsole.Log($"Trying to retrieve script instance of {baseClassName} for entity {mID}");
            Object scriptInstance = FunctionCalls.GetScriptInstance(mID, baseClassName);

            if (scriptInstance == null)
            {
              //  CMConsole.Log($"Script instance is null");
                return null;
            }

            return scriptInstance as T;
        }

        public Entity FindEntityWithName(string name)
        {
            uint entityID = FunctionCalls.Entity_FindEntityWithName(name);
            
            if (entityID == 0)
            {
               // CMConsole.Log("entity not found");
                return null;
            }

            return new Entity(entityID);
        }

        public Entity FindEntityWithID(uint id)
        {
            uint entityID = FunctionCalls.Entity_FindEntityWithID(id);  

            if ( entityID == 0)
                return null;
            return new Entity(entityID);
        }

        public Entity[] FindEntitiesWithTag(string tag)
        {
            uint[] entityIDs = FunctionCalls.Entity_FindEntitiesWithTag(tag);

            Entity[] entities = new Entity[entityIDs.Length];  

            for (int i = 0; i < entityIDs.Length; ++i)
            {
                entities[i] = new Entity(entityIDs[i]);
            }

            return entities;
        }

        public Entity[] GetAllChildren()
        {
            uint[] entityIDs = FunctionCalls.Entity_GetAllChildren(mID);

            Entity[] entities = new Entity[entityIDs.Length];

            for(int i = 0; i < entityIDs.Length; ++i)
            {
                entities[i] = new Entity(entityIDs[i]);
            }

            return entities;
        }

        public Entity GetParent()
        {
            uint entityID = FunctionCalls.Entity_GetParent(mID);
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
            mID = 0;
        }

        public void ChangeAnim(string animName)
        {
            FunctionCalls.Animation_ChangeAnim(mID, animName);
        }

        public string GetTag()
        {
            return FunctionCalls.Transform_GetTag(mID);
        }


        //public T GetComponent<T>() 
    }
}

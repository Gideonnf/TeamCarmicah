using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TrapJellybean : TrapAI
    {
        public List<Entity> collidedEntities;

        public override void OnCreate()
        {
            collidedEntities = new List<Entity>();
        }
        public override void OnTriggerEnter(uint id)
        {
            base.OnTriggerEnter(id);

            Entity collidedEntity = FindEntityWithID(id);
            if (collidedEntity.GetTag() == "Enemy")
            {
                collidedEntities.Add(collidedEntity);
            }
        }

        public override void KillCollidedEnemies()
        {
            foreach( Entity entity in collidedEntities)
            {
                // idk if flying enemies have the same tag 
                // if it does then this will break
                if (entity.GetTag() == "Enemy")
                {
                    entity.As<MouseAI>().KillMouse();
                }
               // entity.Destroy();
            }
        }

        // i need to make on trigger exit pass in a int :pepepray:
        public override void OnTriggerExit(uint collidedEntity)
        {
            if (!built) return;

            if (isDead) return;

            Entity entity = FindEntityWithID(collidedEntity);
            if (collidedEntities.Contains(entity))
            {
                collidedEntities.Remove(entity);
            }
        }

        public override void OnUpdate(float dt)
        {
            base.OnUpdate(dt);
        }

        public override void OnStateEnter(string stateName)
        {
            base.OnStateEnter(stateName);
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            base.OnStateUpdate(stateName, dt);
        }

        public override void OnStateExit(string stateName)
        {
            base.OnStateExit(stateName);
        }

    }
}

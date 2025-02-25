using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class TrapAI : Entity
    {
        public bool built = false;
        public float life = 4.0f;
        public float timer = 0.0f;
        public bool isDead = false;
        public Entity buildSpotEntity = null;

        public override void OnCollide(uint id)
        {
            if (!built) return;

            if (isDead) return;

            if (mID == 0)
            {
                return;
            }

            Entity collidedEntity = FindEntityWithID(id);
            if(collidedEntity != null)
            {
                if(collidedEntity.GetTag() == "Enemy")
                {
                    Sound.PlaySFX("trap extend");
                    ChangeAnim("CandyCone");
                    life -= 1;
                }
            }
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            if (isDead)
            {
                timer += fixedDt;
            }

            if (life <= 0)
            {
                // change to dead state
                GetComponent<StateMachine>().SetStateCondition(1);
                //CMConsole.Log($"Dead");
            }
        }

        public override void OnStateEnter(string stateName)
        {
            CMConsole.Log($"State name : {stateName}");

            if (stateName == "Dead")
            {
                CMConsole.Log($"State name : {stateName}");

                isDead = true;
                CMConsole.Log($"isDead : {isDead}");
                // change to dead anim

               
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if (stateName == "Dead")
            {
                if (timer >= 2)
                {
                    // Play death sound?

                    buildSpotEntity.As<TrapBuild>().TrapDead();
                    // Die
                    timer = 0.0f;
                    // for now
                    Destroy();
                }
            }
        }

        public void SetBuildEntity(Entity entity)
        {
            if (buildSpotEntity == null)
                buildSpotEntity = entity;
        }

        public override void OnStateExit(string stateName)
        {
            
        }
    }
}

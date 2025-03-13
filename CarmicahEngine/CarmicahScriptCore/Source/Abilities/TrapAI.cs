using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    //public enum TrapType
    //{
    //    CANDY_CONE,
    //    HONEY
    //}
    public class TrapAI : Entity
    {
        public string idleAnim;
        public string enterAnim;
        public string exitAnim;
        public bool built = false;
        public float life = 4.0f;
        public float timer = 0.0f;
        public bool isDead = false;
        public Entity buildSpotEntity = null;
        public AbilityType type;


        //float maxAnimTime;
        //float animTimer;
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

        public override void OnTriggerEnter(uint id)
        {
            if (!built) return;

            if (isDead) return;

            if (mID == 0)
            {
                return;
            }

            Entity collidedEntity = FindEntityWithID(id);
            if (collidedEntity != null)
            {

            }
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            if (!isDead)
            {
                timer += fixedDt;
            }

            if (life <= 0)
            {
                // change to dead state
                GetComponent<StateMachine>().SetStateCondition(4);
                //CMConsole.Log($"Dead");
            }

            if(type == AbilityType.HONEY)
            {
                if(timer > life)
                {
                    GetComponent<StateMachine>().SetStateCondition(4);
                }    
            }
        }

        public override void OnStateEnter(string stateName)
        {
            CMConsole.Log($"State name : {stateName}");

            if (stateName == "Created")
            {
                if (type == AbilityType.JELLYBEAN)
                {
                    GetComponent<StateMachine>().SetStateCondition(3);
                    return;
                }
                ChangeAnim(enterAnim);
            }
            else if (stateName == "Enter")
            {
                if(!string.IsNullOrEmpty(enterAnim))
                {
                    ChangeAnim(enterAnim);
                }
                else
                {
                    GetComponent<StateMachine>().SetStateCondition(3);
                }
            }
            else if (stateName == "Idle")
            {
                ChangeAnim(idleAnim);
            }
            else if (stateName == "Dead")
            {
                // change to dead anim
                if (!string.IsNullOrEmpty(exitAnim))
                {
                    ChangeAnim(exitAnim);
                }
                isDead = true;
            }
        }

        public override void OnStateUpdate(string stateName, float dt)
        {
            if(stateName == "Created")
            {
                GetComponent<StateMachine>().SetStateCondition(2);
            }
            else if(stateName == "Enter")
            {
                //animTimer += dt;

                if(GetComponent<Animation>().IsAnimFinished())
                {
                    GetComponent<StateMachine>().SetStateCondition(3);
                }
            }

            else if(stateName == "Idle")
            {
                if(type == AbilityType.JELLYBEAN)
                {
                    if (GetComponent<Animation>().IsAnimFinished())
                    {
                        GetComponent<StateMachine>().SetStateCondition(4);
                    }
                }
            }

            else if (stateName == "Dead")
            {
                // animTimer += dt;
                if (GetComponent<Animation>().IsAnimFinished())
                {
                    buildSpotEntity.As<TrapBuild>().TrapDead();
                    timer = 0.0f;
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

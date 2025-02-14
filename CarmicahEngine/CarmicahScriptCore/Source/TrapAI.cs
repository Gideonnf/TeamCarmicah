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

        void OnCollide(uint id)
        {
            if (!built) return;

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

                }
            }
        }

        

    }
}

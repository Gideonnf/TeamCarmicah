using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
   public class TrapBuild : Entity
    {
        public string TrapPrefabName = "Trap_1";
        Entity trapEntity;
        public void OnMouseEnter()
        {
            trapEntity = CreateGameObject(TrapPrefabName);
            trapEntity.Position = Position;
        }

        public void OnMouseExit()
        {
            trapEntity.Destroy();
        }
    }
}

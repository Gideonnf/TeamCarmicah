using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarmicahScript
{
    public class TutorialController : Entity
    {
        Entity enemy;

        void OnCreate()
        {
            enemy = FindEntityWithName("Tutorial_Enemy");
        }

        void OnUpdate(float dt)
        {
            //Sound.PlaySFX("Item_Hover", 0.4f);
            //this.GetComponent<Renderer>().SetColour(1.5f, 1.5f, 1.5f);

        }
    }
}

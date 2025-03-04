using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class AbilityBar : Entity
    {
        public string ShooterIconPrefab = "ShooterIcon";
        public string MageIconPrefab = "MageIcon";
        public string TrapIconPrefab = "TrapIcon";
        public string HoneyIconPrefab = "HoneyIcon";

        public List<TrapIcon> trapIcons = new List<TrapIcon>();
        public float yOffset = 1.5f;

        public override void OnCreate()
        {
            
        }

        public void IconRemoved(TrapIcon icon)
        {

        }

        public void CreateIcon(IconType type)
        {
            switch (type)
            {
                case IconType.CANDY_ICON:
                    {
                        
                        break;
                    }
                case IconType.SHOOTER_ICON:
                    {
                       

                        break;
                    }
                case IconType.HONEY_ICON:
                    {
                        

                        break;
                    }
                case IconType.MAGE_ICON:
                    {
                        

                        break;
                    }
            }

        }
    }
}

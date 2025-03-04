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

        public List<BaseIcon> trapIcons = new List<BaseIcon>();
        public float yOffset = 75.0f;
        public float startingPos = 760.0f;

        public override void OnCreate()
        {
            
        }

        public void IconRemoved(BaseIcon icon)
        {

        }

        public void CreateIcon(IconType type)
        {
            Entity newIcon = null;
            switch (type)
            {
                case IconType.CANDY_ICON:
                    {
                        newIcon = CreateGameObject(TrapIconPrefab);
                        break;
                    }
                case IconType.SHOOTER_ICON:
                    {

                        newIcon = CreateGameObject(ShooterIconPrefab);
                        break;
                    }
                case IconType.HONEY_ICON:
                    {
                        newIcon = CreateGameObject(HoneyIconPrefab);

                        break;
                    }
                case IconType.MAGE_ICON:
                    {
                        newIcon = CreateGameObject(MageIconPrefab);

                        break;
                    }
            }

            if (newIcon != null)
            {
                newIcon.Position = new Vector2(Position.x, startingPos + (yOffset * trapIcons.Count));
                trapIcons.Add(newIcon.As<BaseIcon>());
            }

        }
    }
}

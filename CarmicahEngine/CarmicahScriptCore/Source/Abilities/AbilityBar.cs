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
            CreateIcon(IconType.CANDY_ICON);
            CreateIcon(IconType.SHOOTER_ICON);
            CreateIcon(IconType.MAGE_ICON);
            CreateIcon(IconType.HONEY_ICON);

        }

        public void IconRemoved(BaseIcon icon)
        {
            trapIcons.Remove(icon);
            CMConsole.Log("Removing base icon");
        }

        public void CreateIcon(IconType type)
        {
            CMConsole.Log($"Icon list{trapIcons.Count}");
            if (trapIcons.Count >= 6)
            {
                return;
            }

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
                // I dont know if we should just hard code the 7 spot positions
                // because the offset is uneven since the size of sprites are different
                newIcon.Position = new Vector2(Position.x, startingPos - ((yOffset * trapIcons.Count) + (15 * trapIcons.Count)));
                trapIcons.Add(newIcon.As<BaseIcon>());
            }

        }
    }
}

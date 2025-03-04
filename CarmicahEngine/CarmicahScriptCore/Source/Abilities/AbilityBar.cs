using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Carmicah
{
    public class AbilityBar : Entity
    {
        public string ShooterIconPrefab = "ShooterIcon";
        public string MageIconPrefab = "MageIcon";
        public string TrapIconPrefab = "TrapIcon";
        public string HoneyIconPrefab = "HoneyIcon";

       // public List<BaseIcon> trapIcons = new List<BaseIcon>();

        public BaseIcon[] listOfIcons = new BaseIcon[6];
        int lastIcon = 0;
        public float yOffset = 75.0f;
        public float startingPos = 760.0f;

        public override void OnCreate()
        {
            CreateIcon(IconType.CANDY_ICON);
            CreateIcon(IconType.HONEY_ICON);
            CreateIcon(IconType.CANDY_ICON);
            CreateIcon(IconType.HONEY_ICON);

        }

        public void IconRemoved(BaseIcon icon)
        {
           // trapIcons.Remove(icon);
           for(int i = 0; i < listOfIcons.Length; i++)
            {
                if (listOfIcons[i] == icon)
                {
                    for (int j = i + 1; j < lastIcon; j++)
                    {
                        listOfIcons[j].Position = listOfIcons[j - 1].Position;
                        listOfIcons[j - 1] = listOfIcons[j];
                    }
                    lastIcon--;
                    break;
                }
            }

           //bring down by 1
            CMConsole.Log("Removing base icon");
        }

        public void CreateIcon(IconType type)
        {
            //CMConsole.Log($"Icon list{trapIcons.Count}");
            if (lastIcon >= 6)
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
                newIcon.Position = new Vector2(Position.x, startingPos - ((yOffset * lastIcon) + (15 * lastIcon)));
                listOfIcons[lastIcon] = newIcon.As<BaseIcon>();
                lastIcon++;
                // trapIcons.Add(newIcon.As<BaseIcon>());
            }

        }
    }
}

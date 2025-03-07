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
        public float startingYPos = 760.0f;
        public float startingXPos = 1820.0f;

        public float shakeTimer = 1.0f;
        public float shakeMagnitude = 0.2f;
        public float timer = 0.0f;
        public bool shake = false;
        Vector2 originalPos;
        public Vector2[] iconOriginalPos = new Vector2[6];

        public override void OnCreate()
        {
            CreateIcon(IconType.HONEY_ICON);
            CreateIcon(IconType.HONEY_ICON);
            CreateIcon(IconType.HONEY_ICON);
            CreateIcon(IconType.SHOOTER_ICON);
            CreateIcon(IconType.SHOOTER_ICON);
            CreateIcon(IconType.SHOOTER_ICON);

            originalPos = this.Position;
        }

        public override void OnFixedUpdate(float fixedDt)
        {
            if (shake)
            {
                timer += fixedDt;
                if (timer>= shakeTimer)
                {
                    //shake = false;
                    Position = originalPos;
                    for (int i = 0; i < lastIcon; ++i)
                    {
                        listOfIcons[i].Position = iconOriginalPos[i];
                    }
                    // cause I want a gap between shaking if the player just stands ontop of the bar
                    if (timer >= shakeTimer + 2.0f)
                    {
                        shake = false;
                    }
                }
                else
                {
                    float offsetX = (CMRand.Range(0, 2.0f) - 1.0f) * shakeMagnitude;
                    float offsetY = (CMRand.Range(0, 2.0f) - 1.0f) * shakeMagnitude;
                    Position = originalPos + new Vector2(offsetX, offsetY);
                    for(int i = 0; i < lastIcon; ++i)
                    {
                        
                        listOfIcons[i].Position = iconOriginalPos[i] + new Vector2(offsetX, offsetY);
                    }
                }
            }
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

        public bool CreateIcon(IconType type)
        {
            //CMConsole.Log($"Icon list{trapIcons.Count}");
            if (lastIcon >= 6)
            {
                if (!shake)
                {
                    timer = 0.0f;
                    shake = true;
                }
                return false;
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
                CMConsole.Log($"Position {Position.x}, {Position.y}");
                newIcon.Position = new Vector2(startingXPos, startingYPos - ((yOffset * lastIcon) + (15 * lastIcon)));
                CMConsole.Log($"new Icon Pos {newIcon.Position.x}, {newIcon.Position.y}");
                listOfIcons[lastIcon] = newIcon.As<BaseIcon>();
                //CMConsole.Log("HSUDHIASDHIA");
                iconOriginalPos[lastIcon] = new Vector2(startingXPos, startingYPos - ((yOffset * lastIcon) + (15 * lastIcon)));
                //CMConsole.Log($"Position of icons {iconOriginalPos[lastIcon].x}, {iconOriginalPos[lastIcon].y}");
                lastIcon++;
                // trapIcons.Add(newIcon.As<BaseIcon>());
            }

            return true;
        }
    }
}

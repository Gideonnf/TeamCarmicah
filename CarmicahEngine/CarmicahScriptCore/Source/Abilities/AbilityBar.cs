﻿using System;
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
        public string SpearIconPrefab = "SpearIcon";
        public string JellybeanIconPrefab = "JellybeanIcon";

       // public List<BaseIcon> trapIcons = new List<BaseIcon>();

        public BaseIcon[] listOfIcons = new BaseIcon[6];
        int lastIcon = 0;
        public float yOffset = 75.0f;
        public float startingPos = 760.0f;

        public float shakeTimer = 1.0f;
        public float shakeMagnitude = 0.2f;
        public float timer = 0.0f;
        public bool shake = false;
        Vector2 originalPos;
        public Vector2[] iconOriginalPos = new Vector2[6];

        public override void OnCreate()
        {
            CreateIcon(IconType.SHOOTER_ICON);
            CreateIcon(IconType.SPEAR_ICON);
            CreateIcon(IconType.MAGE_ICON);
            CreateIcon(IconType.CANDY_ICON);
            CreateIcon(IconType.HONEY_ICON);
            CreateIcon(IconType.JELLYBEAN_ICON);

            originalPos = this.Position;
        }

        public override void OnUpdate(float dt)
        {
            if (shake)
            {
                timer += dt;
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
                    //CMConsole.Log($"found icon at {i}");

                    for(int j = lastIcon - 1; j > i; j--)
                    {
                        listOfIcons[j].Position = listOfIcons[j - 1].Position;
                    }

                    for (int j = i + 1; j < lastIcon; j++)
                    {
                        //CMConsole.Log($"Swapping position of {j} and {j - 1}");
                       // listOfIcons[j].Position = listOfIcons[j - 1].Position;
                        listOfIcons[j - 1] = listOfIcons[j];
                       // CMConsole.Log($"Position original {listOfIcons[j].Position.x}, {listOfIcons[j].Position.y}");
                       // CMConsole.Log($"Position New {listOfIcons[j]});
                    }
                    lastIcon--;
                    break;
                }
            }

           //bring down by 1
            //CMConsole.Log("Removing base icon");
        }

        public bool CreateIcon(IconType type)
        {

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
                case IconType.SPEAR_ICON:
                    {
                        newIcon = CreateGameObject(SpearIconPrefab);

                        break;
                    }
                case IconType.JELLYBEAN_ICON:
                    {
                        newIcon = CreateGameObject(JellybeanIconPrefab);

                        break;
                    }
            }

            if (newIcon != null)
            {
                //CMConsole.Log("new icon is not null");
                // I dont know if we should just hard code the 7 spot positions
                // because the offset is uneven since the size of sprites are different
                newIcon.Position = new Vector2(Position.x, startingPos - ((yOffset * lastIcon) + (15 * lastIcon)));
                listOfIcons[lastIcon] = newIcon.As<BaseIcon>();
                //CMConsole.Log("HSUDHIASDHIA");
                iconOriginalPos[lastIcon] = new Vector2(Position.x, startingPos - ((yOffset * lastIcon) + (15 * lastIcon)));
                //CMConsole.Log($"Position of icons {iconOriginalPos[lastIcon].x}, {iconOriginalPos[lastIcon].y}");
                lastIcon++;
                // trapIcons.Add(newIcon.As<BaseIcon>());
            }

            //CMConsole.Log($"Num of icons{lastIcon}");

            return true;
        }
    }
}

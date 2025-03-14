using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Parallex : Entity
    {
        // Itself is the floor
        Entity cam;
        Entity sky;                      // Follows by 100%
        
        Entity[] layer2 = new Entity[2]; // Follows by 90%
        Entity[] layer1 = new Entity[2]; // Follows by 50%

        Vector2 lastCamPos;

        public override void OnCreate()
        {
            cam = FindEntityWithName("MainCamera");
            lastCamPos = cam.LocalPosition;
            Entity[] Children = GetAllChildren();
            int i = 0;
            foreach (Entity child in Children)
            {
                switch (i)
                {
                    case 1:
                        sky = child;
                        break;
                    case 5:
                        layer1[0] = child;
                        break;
                    case 7:
                        layer2[0] = child;
                        break;
                    default:
                        break;
                }
                ++i;
            }
        }

        public override void OnUpdate(float dt)
        {
            Vector2 camDiff = cam.LocalPosition - lastCamPos;
            lastCamPos = cam.LocalPosition;

            sky.LocalPosition = lastCamPos;
            {
                Vector2 pos = layer1[0].LocalPosition;
                pos.y += camDiff.y * 0.5f;
                layer1[0].LocalPosition = pos;
            }
            {
                Vector2 pos = layer2[0].LocalPosition;
                pos.y += camDiff.y * 0.5f;
                layer2[0].LocalPosition = pos;
            }
        }
    }
}
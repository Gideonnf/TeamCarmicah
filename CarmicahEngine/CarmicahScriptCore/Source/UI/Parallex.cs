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
        Entity bigClouds;
        Entity cloud0;
        Entity cloud1;
        Entity layer2;
        Entity layer1;
        Entity layer0;

        Vector2 lastCamPos;

        public override void OnCreate()
        {
            cam = FindEntityWithName("MainCamera");
            if (cam != null)
                lastCamPos = cam.LocalPosition;
        }

        public override void OnUpdate(float dt)
        {
            if (cam == null)
            {
                cam = FindEntityWithName("MainCamera");
                lastCamPos = cam.LocalPosition;
            }

            if (sky == null)
                sky = FindEntityWithName("BackgroundSky");
            if (bigClouds == null)
                bigClouds = FindEntityWithName("BackgroundBigClouds");
            if(cloud0 == null)
                cloud0 = FindEntityWithName("BackgroundCloud0");
            if(cloud1 == null)
                cloud1 = FindEntityWithName("BackgroundCloud1");
            if (layer0 == null)
                layer0 = FindEntityWithName("BackgroundP0");
            if (layer1 == null)
                layer1 = FindEntityWithName("BackgroundP1");
            if (layer2 == null)
                layer2 = FindEntityWithName("BackgroundP2");

            Vector2 camDiff = cam.LocalPosition - lastCamPos;
            lastCamPos = cam.LocalPosition;

            if (sky != null)
            {
                sky.LocalPosition = lastCamPos;
            }
            if(bigClouds != null)
            {
                Vector2 pos = bigClouds.LocalPosition;
                pos.y += camDiff.y * 0.5f;
                bigClouds.LocalPosition = pos;
            }

            // -10 ~ 10 y

            // Buildings
            if (layer0 != null && layer1 != null && layer2 != null)
            {
                const float c0 = 1.5f;
                const float c1 = -2.0f;
                const float c2 = -5.3f;
                float camY = lastCamPos.y;
                Vector2 pos = Vector2.Zero;
                if (camY < c2)
                {
                    layer0.LocalPosition = pos;
                    layer1.LocalPosition = pos;
                    layer2.LocalPosition = pos;
                }
                else
                {
                    pos.y = (camY - c2) * 0.1f;
                    layer2.LocalPosition = pos;

                    if (camY > c1)
                        pos.y = pos.y + (camY - c1) * 0.2f;
                    layer1.LocalPosition = pos;

                    if (camY > c0)
                        pos.y = pos.y + (camY - c0) * 0.2f;
                    layer0.LocalPosition = pos;
                }
            }
        }
    }
}
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
            if (layer1[0] == null)
                layer1[0] = FindEntityWithName("BackgroundCakesL2");
            if (layer2[0] == null)
                layer2[0] = FindEntityWithName("BackgroundBigClouds");
            //CMConsole.Log($"cam {cam.mID}");

            if (sky == null || layer1[0] == null || layer2[0] == null)
                return;

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
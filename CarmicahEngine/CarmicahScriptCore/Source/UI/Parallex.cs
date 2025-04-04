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
        bool c0Left = true;
        bool c1Left = false;
        Vector2 cloud0Pos;
        Vector2 cloud1Pos;
        float cloud0Speed;
        float cloud1Speed;

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

        public override void OnFixedUpdate(float dt)
        {
            // Find things
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
            {
                cloud0 = FindEntityWithName("BackgroundCloud0");
                Vector2 temp;
                FunctionCalls.Transform_GetRenderingScale(cloud0.mID, out temp);
                cloud0Pos.x = temp.x / 2.0f;
                cloud0Pos.y = CMRand.Range(-10.0f, 10.0f);
                cloud0Speed = CMRand.Range(1.0f, 20.0f);
            }
            if (cloud1 == null)
            {
                cloud1 = FindEntityWithName("BackgroundCloud1");
                Vector2 temp;
                FunctionCalls.Transform_GetRenderingScale(cloud1.mID, out temp);
                cloud1Pos.x = temp.x / 2.0f;
                cloud1Pos.y = CMRand.Range(-10.0f, 10.0f);
                cloud1Speed = CMRand.Range(1.0f, 20.0f);
            }
            if (layer0 == null)
                layer0 = FindEntityWithName("BackgroundP0");
            if (layer1 == null)
                layer1 = FindEntityWithName("BackgroundP1");
            if (layer2 == null)
                layer2 = FindEntityWithName("BackgroundP2");

            // Do things

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

            if (cloud0 != null)
            {
                bool resetCloud = false;
                Vector2 pos = cloud0.LocalPosition;
                if (!c0Left)
                {
                    pos.x += cloud0Speed * dt;
                    if (pos.x > 20.0f + cloud0Pos.x)
                        resetCloud = true;
                }
                else
                {
                    pos.x -= cloud0Speed * dt;
                    if (pos.x < -20.0f - cloud0Pos.x)
                        resetCloud = true;
                }
                if (resetCloud)
                {
                    float randNum = CMRand.Range(0.0f, 7.0f);
                    if (randNum < 1.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_SmallCloud 0");
                    else if (randNum < 2.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_BigCloud 0");
                    else if (randNum < 3.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TLCloud1 0");
                    else if (randNum < 4.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud1 0");
                    else if (randNum < 5.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TLCloud2 0");
                    else if (randNum < 6.0f)
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud2 0");
                    else
                        cloud0.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud3 0");
                    
                    Vector2 temp;
                    FunctionCalls.Transform_GetRenderingScale(cloud0.mID, out temp);
                    cloud0Pos.x = temp.x / 2.0f;
                    cloud0Pos.y = CMRand.Range(-10.0f, 10.0f);
                    cloud0Speed = CMRand.Range(1.0f, 20.0f);
                    if (CMRand.Range(0.0f, 1.0f) > 0.5f)
                    {
                        c0Left = true;
                        pos.x = 20.0f + cloud0Pos.x;
                    }
                    else
                    {
                        c0Left = false;
                        pos.x = -20.0f - cloud0Pos.x;
                    }
                }
                pos.y = lastCamPos.y + cloud0Pos.y;
                cloud0.LocalPosition = pos;
            }
            if (cloud1 != null)
            {
                bool resetCloud = false;
                Vector2 pos = cloud1.LocalPosition;
                if (!c1Left)
                {
                    pos.x += cloud1Speed * dt;
                    if (pos.x > 20.0f + cloud1Pos.x)
                        resetCloud = true;
                }
                else
                {
                    pos.x -= cloud1Speed * dt;
                    if (pos.x < -20.0f - cloud1Pos.x)
                        resetCloud = true;
                }
                if (resetCloud)
                {
                    float randNum = CMRand.Range(0.0f, 7.0f);
                    if (randNum < 1.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_SmallCloud 0");
                    else if (randNum < 2.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_BigCloud 0");
                    else if (randNum < 3.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TLCloud1 0");
                    else if (randNum < 4.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud1 0");
                    else if (randNum < 5.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TLCloud2 0");
                    else if (randNum < 6.0f)
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud2 0");
                    else
                        cloud1.GetComponent<Renderer>().ChangeTexture("Clouds_SpriteSheet_TRCloud3 0");

                    Vector2 temp;
                    FunctionCalls.Transform_GetRenderingScale(cloud1.mID, out temp);
                    cloud1Pos.x = temp.x / 2.0f;
                    cloud1Pos.y = CMRand.Range(-10.0f, 10.0f);
                    cloud1Speed = CMRand.Range(1.0f, 20.0f);
                    if(CMRand.Range(0.0f, 1.0f) > 0.5f)
                    {
                        c1Left = true;
                        pos.x = 20.0f + cloud1Pos.x;
                    }
                    else
                    {
                        c1Left = false;
                        pos.x = -20.0f - cloud1Pos.x;
                    }
                }
                pos.y = lastCamPos.y + cloud1Pos.y;
                cloud1.LocalPosition = pos;
            }

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
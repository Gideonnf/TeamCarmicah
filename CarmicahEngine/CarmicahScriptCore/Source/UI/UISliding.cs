using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class UISliding : Entity
    {
        public float slideTime = 1.0f;
        public float slideDist = 1000.0f;
        public string slideType = "Up";
        public int slideCurve = 0;

        private bool slidIn = false;
        private float t = 0.0f;
        public Vector2 startPos;
        public Vector2 endPos;

        public override void OnCreate()
        {
            slidIn = false;
            t = 0.0f;
            endPos = startPos = new Vector2(960.0f, 540.0f);
            slideType = slideType.ToLower();
            switch (slideType)
            {
                case "up":
                    startPos.y -= slideDist;
                    break;
                case "down":
                    startPos.y += slideDist;
                    break;
                case "left":
                    startPos.x += slideDist;
                    break;
                case "right":
                    startPos.x -= slideDist;
                    break;
                default:
                    startPos.y -= slideDist;
                    break;
            }
            Position = startPos;
        }

        public void ChangeSlideDetails(int sc , Vector2 s, Vector2 e, float totalTime)
        {
            slidIn = false;
            t = 0.0f;
            slideTime = totalTime;
            slideCurve = sc;
            startPos = s;
            endPos = e;
        }

        public void EndNow()
        {
            t = slideTime + 1.0f;
        }

        public override void OnUpdate(float dt)
        {
            if(!slidIn)
            {
                t += dt;
                if(t > slideTime)
                {
                    Position = endPos;
                    slidIn = true;
                    return;
                }

                Vector2 interpolated = Vector2.Zero;

                switch(slideCurve)
                {
                    case 0:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.LINEAR, startPos, endPos, t / slideTime);
                        break;
                    case 1:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.QUADRATIC, startPos, endPos, t / slideTime);
                        break;
                    case 2:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.CUBIC, startPos, endPos, t / slideTime);
                        break;
                    case 3:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.QUART, startPos, endPos, t / slideTime);
                        break;
                    case 4:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.SINE, startPos, endPos, t / slideTime);
                        break;
                    case 5:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.EXPO, startPos, endPos, t / slideTime);
                        break;
                    case 6:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.CIRC, startPos, endPos, t / slideTime);
                        break;
                    case 7:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.BOUNCE, startPos, endPos, t / slideTime);
                        break;
                    case 8:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.BACK, startPos, endPos, t / slideTime);
                        break;
                    default:
                        interpolated = Easings.GetInterpolate(SLIDE_CURVE.ELASTIC, startPos, endPos, t / slideTime);
                        break;
                }
                Position = interpolated;
            }
        }
    }
}
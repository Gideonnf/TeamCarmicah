using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class DimBG : Entity
    {
        public float targetAlpha = 0.75f;
        public float duration = 1.0f;
        public float timer = 0.0f;
        public bool fadeOut = false;
        public override void OnUpdate(float dt)
        {
            timer += dt;
            if (!fadeOut)
            {
                float alpha = (timer / duration) * targetAlpha;
                if (alpha >= targetAlpha) alpha = targetAlpha;
                GetComponent<Renderer>().SetAlpha(alpha);
            }
            else
            {
                float alpha = targetAlpha - (timer / duration) * targetAlpha;
                if (alpha <= 0.0f) alpha = 0.0f;
                GetComponent<Renderer>().SetAlpha(alpha);

            }
        }

        public void FadeOut()
        {
            fadeOut = true;
            timer = 0.0f;
        }
    }
}

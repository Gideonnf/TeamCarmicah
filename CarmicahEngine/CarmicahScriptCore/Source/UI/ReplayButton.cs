using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ReplayButton : Entity
    {

        public string buttonType;

        public string buttonClick;
        public string buttonHover;
        public string buttonExit;

        
        //string Animation0 = "Button_C_Replay";
        //string Animation1 = "Button_HS_Replay";
        bool hovering = false;
        //void OnCreate()
        //{
        //    ChangeAnim("Button_HE_Back");
        //}
        void OnClick()
        {
            Sound.PlaySFX("SFX_Button", 0.5f);
            //ChangeAnim(Animation0);

            if (hovering)
            {
                hovering = false;
                ChangeAnim(buttonClick);
            }
            //settings.Destroy();
            //Destroy();
        }

        public void OnMouseHover()
        {


            if (!hovering)
            {
                hovering = true;

                ChangeAnim(buttonHover);
            }
        }

        public void OnMouseClick()
        {
            if (hovering)
            {
                hovering = false;
                ChangeAnim(buttonClick);
            }
            //hovering = false;

        }

        public void OnMouseExit()
        {
            if (hovering)
            {
                hovering = false;
                ChangeAnim(buttonExit);

            }
        }
    }
}

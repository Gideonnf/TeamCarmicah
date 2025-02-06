using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class BackButton : Entity
    {

        public string SettingsMenu = "Settings_Menu";
        public string buttonType;
        string Animation0 = "Button_Click_Back";
        string Animation1 = "Button_Back";
        bool hovering = false;
        //public string SettingsCloseButton = "Settings_Close";
        void OnClick()
        {
            Sound.PlaySFX("SFX_Button");
            Entity settings = FindEntityWithName(SettingsMenu);
            SettingButton.IsCreated = false;
            ChangeAnim(Animation0);
            settings.Destroy();
            Destroy();

        }

        public void OnMouseHover()
        {


            if (!hovering)
            {
                hovering = true;
                
                ChangeAnim(Animation1);
            }
        }

        public void OnMouseClick()
        {
            hovering = false;
        }

        public void OnMouseExit()
        {
            hovering = false;
            ChangeAnim("Bear_Climb");
        }
    }
}

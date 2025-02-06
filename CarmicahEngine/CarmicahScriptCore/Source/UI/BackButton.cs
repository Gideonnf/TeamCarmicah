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
        public string CreditsMenu = "Credits_Menu";
        //public string SettingsCloseButton = "Settings_Close";
        void OnClick()
        {
            Entity settings = FindEntityWithName(SettingsMenu);
            SettingButton.IsCreated = false;
            ChangeAnim(Animation0);
            // settings.Destroy();
            // Destroy();
            if (settings != null)
            {
                settings.Destroy();
                Destroy();
            }

            Entity credits = FindEntityWithName(CreditsMenu);
            if (credits != null)
            {
                credits.Destroy();
                Destroy();
            }
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

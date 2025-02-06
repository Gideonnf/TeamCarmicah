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
        //public string SettingsCloseButton = "Settings_Close";
        void OnClick()
        {
            Sound.PlaySFX("SFX_Button");
            Entity settings = FindEntityWithName(SettingsMenu);
            SettingButton.IsCreated = false;
            settings.Destroy();
            Destroy();

        }
    }
}

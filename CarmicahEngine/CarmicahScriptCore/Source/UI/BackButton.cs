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
           Entity settings = FindEntityWithName(SettingsMenu);
            settings.Destroy();
            Destroy();
        }
    }
}

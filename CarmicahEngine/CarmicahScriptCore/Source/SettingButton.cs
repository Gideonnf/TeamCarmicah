using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class SettingButton : Entity
    {
        public string SettingsMenu = "Settings_Menu";
        public string SettingsCloseButton = "Close_Button";
        public bool IsCreated = false;
        void OnClick()
        {
            if (!IsCreated)
            {
                CreateGameObject(SettingsMenu);
                CreateGameObject(SettingsCloseButton);
            }
            Console.WriteLine($"Testing Button {mID}");
        }
    }
}

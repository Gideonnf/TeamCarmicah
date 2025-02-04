using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ResumeButton : Entity
    {

        void OnClick()
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null)
            {
                pauseManager.As<PauseManager>().UnPause();
                Sound.PlaySFX("SFX_Button");

            }

        }
    }
}

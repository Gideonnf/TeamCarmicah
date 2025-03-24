using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class PauseManager : Entity
    {
        public string PauseMenu = "Pause_Menu";
        public string QuitButton = "PauseMenuQuitBtn";
        public string ResumeButton = "PauseMenuResumeBtn";
        public bool IsPaused = false;

        Entity PauseMenuEntity;
        Entity QuitButtonEntity;
        Entity ResumeButtonEntity;
        public override void OnCreate()
        {

        }

        public override void OnUpdate(float dt)
        {
            //Console.WriteLine("ITS IN UPDATE FOR PAUSE");

            if (Input.IsKeyPressed(Keys.KEY_ESC))
            {
               // Console.WriteLine("ITS PRESSING ESC");
                if (IsPaused)
                {
                    UnPause();
                }
                else
                {
                    Pause();
                }
            }
        }

        public void Pause()
        {
            CMConsole.Log("Pausing game");
            IsPaused = true;

            // creating moving to button generic side

            //PauseMenuEntity = CreateGameObject("Settings_Menu");
            //QuitButtonEntity = CreateGameObject("GameClose_Button");
            //ResumeButtonEntity = CreateGameObject(ResumeButton);
        }

        public void UnPause()
        {
            CMConsole.Log("UnPausing game");
            IsPaused = false;
            if (PauseMenuEntity != null)
            {
                PauseMenuEntity.Destroy();
            }
            if (QuitButtonEntity != null)
            {
                QuitButtonEntity.Destroy();
            }
            if (ResumeButtonEntity != null)
            {
                ResumeButtonEntity.Destroy();
            }
        }
    }
}

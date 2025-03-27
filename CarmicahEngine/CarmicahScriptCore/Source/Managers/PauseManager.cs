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
        Vector2 mainPos;

        Entity PauseMenuEntity = null;
        public override void OnCreate()
        {
            mainPos = new Vector2(960.0f, 540.0f);
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
            // dont double pause

            if (IsPaused) return;
            CMConsole.Log("Pausing game");
            IsPaused = true;

            // creating moving to button generic side
            PauseMenuEntity = FindEntityWithName("Pause_Screen");
            if (PauseMenuEntity == null)
            {
                PauseMenuEntity = CreateGameObject("Pause_Screen");
               // mainPos = PauseMenuEntity.Position;
            }
            //else
            //{
            //    mainPos = PauseMenuEntity.Position;
            //}
            //CMConsole.Log($"main Pos: {mainPos.x}, {mainPos.y}");
            //QuitButtonEntity = CreateGameObject("GameClose_Button");
            //ResumeButtonEntity = CreateGameObject(ResumeButton);
        }

        public void UnPause()
        {
            CMConsole.Log("UnPausing game");
            IsPaused = false;
            //Entity settings = FindEntityWithName("Pause_Screen");
            if (PauseMenuEntity != null)
            {
                PauseMenuEntity.As<UISliding>().SlideThenSD();
            }

         
        }

        public void ShiftPause(bool hide)
        {
            if (hide)
            {
                if (PauseMenuEntity != null)
                {
                    PauseMenuEntity.As<UISliding>().ChangeSlideDetails(8, PauseMenuEntity.LocalPosition, new Vector2(3840.0f, 540.0f), 1.5f);
                }
            }
            else
            {
                if (PauseMenuEntity != null)
                {
                    PauseMenuEntity.As<UISliding>().ChangeSlideDetails(8, PauseMenuEntity.LocalPosition, mainPos, 1.5f);
                }

            }
        }

        public bool MenuIsSliding()
        {
            if (PauseMenuEntity != null)
            {
                return PauseMenuEntity.As<UISliding>().IsSliding();
            }

            return false;
        }
    }
}

using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class ButtonGeneric : Entity
    {
        public string buttonType = "";
        public string nextScene  = "";

        bool willChangeScene = false;
        bool willUnpause = false;
        bool willPause = false;

        bool createListCreated = false;
        Dictionary<int, string> createList = new Dictionary<int, string>();
        bool willSelfDestruct = false;
        Dictionary<int, string> destroyList = new Dictionary<int, string>();

        float sceneChangerTimer = -1f;
        const float timeToChangeScene = 0.2f;
        string hoverEnterAnim;
        string hoverExitAnim;
        string clickAnim;

        public override void OnCreate()
        {
            buttonType = buttonType.ToLower();
            switch(buttonType)
            {
                case "play":
                    nextScene       = "Cutscenes";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Play";
                    hoverExitAnim   = "Button_HE_Play";
                    clickAnim       = "Button_C_Play";
                    break;
                case "quit":
                    nextScene       = "quit";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Quit";
                    hoverExitAnim   = "Button_HE_Quit";
                    clickAnim       = "Button_C_Quit";
                    break;
                case "home":
                    nextScene       = "Scene3";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Home";
                    hoverExitAnim   = "Button_HE_Home";
                    clickAnim       = "Button_C_Home";
                    break;
                case "resume":
                    willUnpause     = true;

                    hoverEnterAnim  = "Button_HS_Resume";
                    hoverExitAnim   = "Button_HE_Resume";
                    clickAnim       = "Button_C_Resume";
                    break;
                case "replay":
                    nextScene       = "Scene1";
                    willUnpause     = true;
                    willChangeScene = true;
                    
                    hoverEnterAnim  = "Button_HS_Replay";
                    hoverExitAnim   = "Button_HE_Replay";
                    clickAnim       = "Button_C_Replay";
                    break;
                case "skipsplash":
                    nextScene       = "Scene3";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Skip";
                    hoverExitAnim   = "Button_HE_Skip";
                    clickAnim       = "Button_C_Skip";
                    break;
                case "skipintro":
                    nextScene       = "Scene1";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Skip";
                    hoverExitAnim   = "Button_HE_Skip";
                    clickAnim       = "Button_C_Skip";
                    break;
                case "credits":
                    createList[0]   = "Credits_Menu";
                    createList[1]   = "Close_Button_2";

                    hoverEnterAnim  = "Button_HS_Credits";
                    hoverExitAnim   = "Button_HE_Credits";
                    clickAnim       = "Button_C_Credits";
                    break;
                case "backcredits":
                    destroyList[0]  = "Credits_Menu";
                    willUnpause     = true;
                    willSelfDestruct= true;

                    hoverEnterAnim  = "Button_HS_Back";
                    hoverExitAnim   = "Button_HE_Back";
                    clickAnim       = "Button_C_Back";
                    break;
                case "settings":
                    createList[0]   = "Settings_Menu";
                    createList[1]   = "Close_Button";

                    Sound.PlayBGM("BGM_SetupPhase_Mix1", 0.4f);

                    hoverEnterAnim = "Button_HS_Settings";
                    hoverExitAnim   = "Button_HE_Settings";
                    clickAnim       = "Button_C_Settings";
                    break;
                case "backsettings":
                    destroyList[0] = "Settings_Menu";
                    willUnpause     = true;
                    willSelfDestruct = true;

                    hoverEnterAnim = "Button_HS_Back";
                    hoverExitAnim   = "Button_HE_Back";
                    clickAnim       = "Button_C_Back";
                    break;
                case "nextlevel":
                    destroyList[0] = "Win_Screen";

                    hoverEnterAnim = "Button_HS_Next";
                    hoverExitAnim = "Button_HE_Next";
                    clickAnim = "Button_C_Next";

                    break;
                    /*
                    hoverEnterAnim  = "Button_HS_HowTo";
                    hoverExitAnim   = "Button_HE_HowTo";
                    clickAnim       = "Button_C_HowTo";

                    hoverEnterAnim  = "Button_HS_Menu";
                    hoverExitAnim   = "Button_HE_Menu";
                    clickAnim       = "Button_C_Menu";

                    hoverEnterAnim  = "Button_HS_Next";
                    hoverExitAnim   = "Button_HE_Next";
                    clickAnim       = "Button_C_Next";
                    */
            }

            ChangeAnim(hoverExitAnim);
        }

        public override void OnUpdate(float dt)
        {
            if(sceneChangerTimer > 0)
            {
                sceneChangerTimer -= dt;
                if(sceneChangerTimer < 0)
                {
                    sceneChangerTimer = 0;
                    if(nextScene == "quit")
                    {
                        Scene.CloseGame();
                    }
                    else
                    {
                        Scene.ChangeScene(nextScene);
                    }
                }
            }
            if (createListCreated && FindEntityWithName(createList[0]) == null)
            {
                createListCreated = false;
            }
        }

        public override void OnClick()
        {
            //Sound.PlaySFX("SFX_Button", 0.5f);
            //Sound.PlaySFX("SFX_Button");

            ChangeAnim(clickAnim);

            if (willChangeScene && sceneChangerTimer < 0.0f)
            {
                sceneChangerTimer = timeToChangeScene;
            }
            if (willUnpause)
            {
                Entity pauseManager = FindEntityWithName("PauseManager");
                if (pauseManager != null)
                {
                    pauseManager.As<PauseManager>().UnPause();
                }
            }
            else if (willPause)
            {
                
            }

            if (buttonType == "nextlevel")
            {
                Entity waveSystem = FindEntityWithName("Something");
                waveSystem.As<WaveSystem>().EndOfLevel();
            }

            for(int i = 0; destroyList.ContainsKey(i); ++i)
            {
                Entity toDestroy = FindEntityWithName(destroyList[i]);
                if(toDestroy != null) 
                {
                    toDestroy.Destroy();
                }
            }
            if (!createListCreated)
            {
                for (int i = 0; createList.ContainsKey(i); ++i)
                {
                    CreateGameObject(createList[i]);
                }
                createListCreated = true;
            }
            if (willSelfDestruct)
            {
                Destroy();
            }
        }
        public override void OnMouseEnter()
        {
            ChangeAnim(hoverEnterAnim);
        }
        public override void OnMouseExit()
        {
            ChangeAnim(hoverExitAnim);
        }
    }
}
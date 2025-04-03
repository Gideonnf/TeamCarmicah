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
        bool willColorChange = false;

        bool createListCreated = false;
        Dictionary<int, string> createList = new Dictionary<int, string>();
        Dictionary<int, string> destroyList = new Dictionary<int, string>();

        float sceneChangerTimer = -1f;
        bool buttonColChangeState = false;
        float colChangeTime = 0.0f;
        bool buttonSizeChangeGot = false;
        float sizeChangeTime = 1.0f;
        float sizeBounce = 0.85f;
        int sizeBounceInt = 6;
        Vector2 buttonOriginalSize;
        const float buttonGlowInc = 0.15f;
        const float maxColChangeTime = 0.2f;
        const float maxSizeChangeTime = 0.5f;
        const float timeToChangeScene = 0.5f;
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
                case "tutorial":
                    nextScene = "Tutorial";
                    willChangeScene= true;

                    hoverEnterAnim = "Button_HS_Play";
                    hoverExitAnim = "Button_HE_Play";
                    clickAnim = "Button_C_Play";
                    break;
                case "quit": // TO create the confirmation for quit
                    //nextScene       = "quit";
                    //willChangeScene = false;
                    createList.Add(0, "QuitConfirmationPopup"); 


                    hoverEnterAnim = "Button_HS_Quit";
                    hoverExitAnim   = "Button_HE_Quit";
                    clickAnim       = "Button_C_Quit";
                    break;
                case "tquit": // when they click the quit confirmation
                    nextScene = "quit";
                    willChangeScene = true;


                    hoverEnterAnim = "Button_HS_Quit";
                    hoverExitAnim = "Button_HE_Quit";
                    clickAnim = "Button_C_Quit";
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
                case "skipscut":
                    //nextScene = "Scene3";
                    willChangeScene = true;

                    hoverEnterAnim = "Button_HS_Skip";
                    hoverExitAnim = "Button_HE_Skip";
                    clickAnim = "Button_C_Skip";
                    break;
                case "skipintro":
                    nextScene       = "Scene1";
                    willChangeScene = true;

                    hoverEnterAnim  = "Button_HS_Skip";
                    hoverExitAnim   = "Button_HE_Skip";
                    clickAnim       = "Button_C_Skip";
                    break;
                case "credits":
                    nextScene       = "Credits";
                    willChangeScene = true;

                    //createList[0]   = "Credits_Menu";
                    //createList[1]   = "Close_Button_2";

                    hoverEnterAnim  = "Button_HS_Credits";
                    hoverExitAnim   = "Button_HE_Credits";
                    clickAnim       = "Button_C_Credits";
                    break;
                case "backcredits":
                   
                    nextScene       = "Scene3";
                    willChangeScene = true;

                    //destroyList[0]  = "Credits_Menu";
                    //willUnpause     = true;

                    hoverEnterAnim  = "Button_HS_Back";
                    hoverExitAnim   = "Button_HE_Back";
                    clickAnim       = "Button_C_Back";
                    break;
                case "pause":
                    createList.Add(0, "Pause_Screen");
                    willPause = true;
                    willColorChange = true;
                    
                    // NOTE: Once we add in the actual pause button then change this
                    hoverEnterAnim = "Button_C_Pause";
                    hoverExitAnim = "Button_C_Pause";
                    clickAnim = "Button_C_Pause";
                    break;
                case "settings":
                    createList.Add(0, "Settings_Menu");
                    //willPause = true;

                    hoverEnterAnim  = "Button_HS_Settings";
                    hoverExitAnim   = "Button_HE_Settings";
                    clickAnim       = "Button_C_Settings";
                    break;
                case "menusettings":
                    createList.Add(0, "Settings_MainMenu");
                    //willPause = true;

                    hoverEnterAnim = "Button_HS_Settings";
                    hoverExitAnim = "Button_HE_Settings";
                    clickAnim = "Button_C_Settings";
                    break;
                case "backsettings":
                    //willUnpause     = true;

                    hoverEnterAnim  = "Button_HS_Back";
                    hoverExitAnim   = "Button_HE_Back";
                    clickAnim       = "Button_C_Back";
                    break; 
                case "menubacksettings":
                    //willUnpause     = true;

                    hoverEnterAnim = "Button_HS_Back";
                    hoverExitAnim = "Button_HE_Back";
                    clickAnim = "Button_C_Back";
                    break;
                case "howtonext":
                    hoverEnterAnim  = "Button_HowToNext";
                    hoverExitAnim   = "Button_HowToNext";
                    clickAnim       = "Button_HowToNext";
                    willColorChange = true;
                    break;
                case "howtoback":
                    hoverEnterAnim  = "Button_HowToBack";
                    hoverExitAnim   = "Button_HowToBack";
                    clickAnim       = "Button_HowToBack";
                    willColorChange = true;
                    break;
                case "howtoplay":
                    createList.Add(0, "HowToBG");

                    hoverEnterAnim = "Button_HS_HowTo";
                    hoverExitAnim = "Button_HE_HowTo";
                    clickAnim = "Button_C_HowTo";
                    break;
                case "howtoclose":
                    hoverEnterAnim = "Button_HS_Home";
                    hoverExitAnim = "Button_HE_Home";
                    clickAnim = "Button_C_Home";
                    break;
                case "closehowto":
                    hoverEnterAnim = "Button_C_Cross";
                    hoverExitAnim = "Button_C_Cross";
                    clickAnim = "Button_C_Cross";
                    willColorChange = true;
                    break;
                case "nextlevel":
                    //destroyList.Add(0, "Win_Screen");

                    hoverEnterAnim = "Button_HS_Next";
                    hoverExitAnim = "Button_HE_Next";
                    clickAnim = "Button_C_Next";

                    break;
                case "tback":
                    destroyList.Add(0, "QuitConfirmationPopup");

                    hoverEnterAnim = "Button_HS_Back";
                    hoverExitAnim = "Button_HE_Back";
                    clickAnim = "Button_C_Back";

                    break;
                    /*
                    hoverEnterAnim  = "Button_HS_HowTo";
                    hoverExitAnim   = "Button_HE_HowTo";
                    clickAnim       = "Button_C_HowTo";
                    break;
                    /*

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
            if (!HasComponent<Animation>() || GetComponent<Animation>().IsAnimFinished())
            {
                if (sceneChangerTimer > 0)
                {
                    sceneChangerTimer -= dt;
                    if (sceneChangerTimer < 0)
                    {
                        sceneChangerTimer = -1f;
                        if (nextScene == "quit")
                        {
                            Scene.CloseGame();
                        }
                        else
                        {
                            //CMConsole.Log("Changing scene 2");
                            Scene.ChangeScene(nextScene);
                        }
                    }
                }
            }
            if(willColorChange)
            {
                if(!buttonSizeChangeGot)
                {
                    buttonOriginalSize = Scale;
                    buttonSizeChangeGot = true;
                }

                if(sizeChangeTime < maxSizeChangeTime)
                {
                    sizeChangeTime = sizeChangeTime + dt;
                    if (sizeChangeTime > maxSizeChangeTime)
                        sizeChangeTime = maxSizeChangeTime;
                    float percent = sizeChangeTime / maxSizeChangeTime;
                    if(percent < 0.5f)
                        Scale = Easings.GetInterpolate((SLIDE_CURVE)sizeBounceInt, buttonOriginalSize, buttonOriginalSize * sizeBounce, percent * 2.0f);
                    else
                        Scale = Easings.GetInterpolate((SLIDE_CURVE)sizeBounceInt, buttonOriginalSize * sizeBounce, buttonOriginalSize, percent * 2.0f - 1.0f);
                }
                else if(colChangeTime < maxColChangeTime)
                {
                    colChangeTime = colChangeTime + dt;
                    if(colChangeTime > maxColChangeTime)
                        colChangeTime = maxColChangeTime;
                    if(buttonColChangeState)
                    {
                        float percent = 1.0f + colChangeTime / maxColChangeTime * buttonGlowInc;
                        GetComponent<Renderer>().SetColour(percent, percent, percent);
                    }
                    else
                    {
                        float percent = 1.0f + buttonGlowInc - colChangeTime / maxColChangeTime * buttonGlowInc;
                        GetComponent<Renderer>().SetColour(percent, percent, percent);
                    }
                }
            }

            if (createListCreated && createList.Count > 0 && FindEntityWithName(createList[0]) == null)
            {
                CMConsole.Log($"create list name: {createList[0]}");
                createListCreated = false;
            }
        }

        public override void OnClick()
        {
            Entity pauseManager = FindEntityWithName("PauseManager");


            // unique check for pause
            // cause i dont want to be able to pause when game over
            if (buttonType == "pause")
            {
                Entity gameManager = FindEntityWithName("GameManager");

                if (gameManager != null)
                {
                    if (gameManager.As<GameManager>().GameOver)
                        return;
                }
            }

            // its in the game scene
            if (pauseManager != null)
            {
                // if its in the game scene dont let them click on anything if its still sliding
                if( pauseManager.As<PauseManager>().MenuIsSliding())
                {
                    return;
                }
            }

            //Sound.PlaySFX("SFX_Button", 0.5f);
            //Sound.PlaySFX("SFX_Button");

            ChangeAnim(clickAnim);
            sizeChangeTime = 0.0f;

            if (willChangeScene && sceneChangerTimer < 0.0f)
            {
                //CMConsole.Log("Changing scene");

                sceneChangerTimer = timeToChangeScene;
            }

            if (buttonType == "nextlevel")
            {
                Entity waveSystem = FindEntityWithName("Something");
                if (waveSystem.As<WaveSystem>().levelManager.EndOfGame())
                {
                    Scene.ChangeScene("CutsceneEnding");

                }
                else
                {
                    waveSystem.As<WaveSystem>().EndOfLevel();
                }
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
                    // to not double create
                    if(FindEntityWithName(createList[i]) == null)
                        CreateGameObject(createList[i]);
                }
                createListCreated = true;
            }

            if (willUnpause)
            {
                if (pauseManager != null)
                {
                    pauseManager.As<PauseManager>().UnPause();
                }
            }
            else if (willPause)
            {
                if (pauseManager != null)
                {
                    pauseManager.As<PauseManager>().Pause();
                }

            }


            // Specific other behaviours ig
            switch (buttonType)
            {
                case "nextlevel":
                    Entity winScreen = FindEntityWithName("Win_Screen");
                    if (winScreen != null)
                    {
                        if (winScreen.Has<UISliding>())
                        {
                            winScreen.As<UISliding>().SlideThenSD();
                        }
                    }
                    break;
                case "settings":
                    if (pauseManager != null)
                    {
                        pauseManager.As<PauseManager>().ShiftPause(true);
                    }
                    else
                    {
                        Entity screen = FindEntityWithName("Settings_Menu");
                        if(screen != null)
                        {
                            // if its main menu 
                            screen.As<UISliding>().slideToCenter = true;
                        }
                    }
                    break;
                case "resume":
                    {
                        Entity PauseScreen = FindEntityWithName("Pause_Screen");
                        if (PauseScreen != null)
                        {
                            if (PauseScreen.Has<UISliding>())
                            {
                                PauseScreen.As<UISliding>().SlideThenSD();
                            }
                        }
                    }
                    break;
                case "howtonext":
                    FindEntityWithName("HowToBG").As<HowToPlay>().ProgressScene(1);
                    break;
                case "howtoback":
                    FindEntityWithName("HowToBG").As<HowToPlay>().ProgressScene(-1);
                    break;
                case "backsettings":
                    if (pauseManager != null)
                    {
                        pauseManager.As<PauseManager>().ShiftPause(false);
                    }
                    FindEntityWithName("Settings_Menu").As<UISliding>().SlideThenSD();
                    break;
                case "menubacksettings":
                    if (pauseManager != null)
                    {
                        pauseManager.As<PauseManager>().ShiftPause(false);
                    }
                    FindEntityWithName("Settings_MainMenu").As<UISliding>().SlideThenSD();
                    break;
                case "closehowto":
                case "howtoclose":
                    FindEntityWithName("HowToBG").As<HowToPlay>().DestroyLaterAndJustHide();
                    break;
            }
        }
        public override void OnMouseEnter()
        {
            Sound.PlaySFX("UI_Hover 2", 0.3f);
            ChangeAnim(hoverEnterAnim);
            buttonColChangeState = true;
            colChangeTime = maxColChangeTime - colChangeTime;
        }
        public override void OnMouseExit()
        {
            ChangeAnim(hoverExitAnim);
            buttonColChangeState = false;
            colChangeTime = maxColChangeTime - colChangeTime;
        }
    }
}
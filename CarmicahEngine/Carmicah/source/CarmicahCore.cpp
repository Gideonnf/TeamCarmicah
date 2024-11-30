#include "pch.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include "CarmicahCore.h"
#include <spdlog/spdlog.h>
#include "log.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>   
#include <ImGUI/imgui_impl_opengl3.h>
#include "Systems/GOFactory.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "Editor/Editor.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/RigidBody.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"
#include "Components/Script.h"
#include "Components/Prefab.h"

#include "Systems/GameLogic.h"

#include "Systems/GOFactory.h"
#include "Graphics/AnimationSystem.h"
#include "Graphics/WorldGraphicsSystem.h"
#include "Graphics/UIGraphicsSystem.h"
#include "Graphics/TextGraphicsSystem.h"
#include "Graphics/ColliderGraphicsSystem.h"
#include "Graphics/RigidbodyGraphicsSystem.h"
#include "Graphics/RenderHelper.h"
#include "Systems/CollisionSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/ButtonSystem.h"
#include "Systems/PrefabSystem.h"

#include "Input/InputSystem.h"
#include "Systems/SceneSystem.h"
#include "Systems/SerializerSystem.h"
#include "CarmicahTime.h"
#include "Systems/AssetManager.h"
#include "Editor/SceneToImgui.h"
#include "Scripting/ScriptSystem.h"

namespace Carmicah
{
    bool gToggleGui = false;
    //const char* assetsLoc{ "../Assets" };

    Application::Application() {}
    Application::~Application() {}

    void EnableMemoryLeakChecking(int breakAlloc = -1)
    {
        int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(tmpDbgFlag);
        if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
    }

    void EnableOpenGLErrorCheck(bool extraDetails = false)
    {
        static bool ex = extraDetails;
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(
            [](GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* msg2, const void* uParam)
            {
                std::stringstream ss;
                ss << "GL: ";
                if (ex)
                    ss << " type = 0x" << type << ", Severity = 0x" << severity << ", ";
                ss << msg2 << std::endl;
                if (type == GL_DEBUG_TYPE_ERROR)
                {
                    CM_ERROR(ss.str());
                }
                else
                    //CM_CORE_WARN(ss.str());
                if (uParam || source || length || id) // TODO: It's Just removing warnings idfk how to remove this warnings
                    ex = ex;
            },
            0);
    }

    int Application::run()
    {
       // bool isSoundPlaying = false;

        EnableMemoryLeakChecking();
        AssetManager::GetInstance()->LoadConfig("../Assets/config.json");
        // Serializer.LoadConfig(*this);
        Carmicah::Log::init();
        CM_CORE_INFO("Core Logger Initialized");
        CM_INFO("Client Logger Initialized");

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


#ifdef CM_INSTALLER
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        int Width = mode->width;
        int Height = mode->height;
        glfwCreateWindow(Width, Height, "Carmicah", primaryMonitor, NULL);

#endif



    /*    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int  Width = mode->width;
        int Height = mode->height;
        GLFWwindow* window = glfwCreateWindow(Width, Height, "Carmicah", glfwGetPrimaryMonitor(), NULL);*/
    
        //comment it when using installer
        int Width = AssetManager::GetInstance()->enConfig.Width;
        int Height = AssetManager::GetInstance()->enConfig.Height;
        std::string defaultScene = AssetManager::GetInstance()->enConfig.defaultScene;
        //CM_CORE_INFO("Reached before window creation");
        GLFWwindow* window = glfwCreateWindow(Width, Height, "Carmicah", NULL, NULL);
       // int bufferWidth, bufferHeight;
        //glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
        glfwMakeContextCurrent(window);
       // CM_CORE_INFO("Reached after window creation");

        if (window == NULL)
        {
            CM_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            CM_CORE_ERROR("Failed to initialize GLAD");
            return -1;
        }
        EnableOpenGLErrorCheck();

     /*   GLFWwindow* ImGuiWindow = glfwCreateWindow(WIDTH, HEIGHT, "ImGui", NULL, NULL);

        if (ImGuiWindow == NULL)
        {
            CM_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }*/

        CarmicahTime::GetInstance()->InitTime();
        CarmicahTime::GetInstance()->InitGPUProfiling();

        glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
        REGISTER_COMPONENT(Button);
        REGISTER_COMPONENT(Script);
        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(RigidBody);
        REGISTER_COMPONENT(Renderer);
        REGISTER_COMPONENT(Animation);
        REGISTER_COMPONENT(TextRenderer);
        REGISTER_COMPONENT(UITransform);
        REGISTER_COMPONENT(PrefabData);

        CM_CORE_INFO("Starting system init");

        auto editorSys = REGISTER_SYSTEM(Editor);
        REGISTER_SYSTEM(GOFactory);
        REGISTER_SYSTEM(ScriptSystem);
        auto graSystem = REGISTER_SYSTEM(WorldGraphicsSystem);
        auto uigSystem = REGISTER_SYSTEM(UIGraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextGraphicsSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderGraphicsSystem);
        auto rrsSystem = REGISTER_SYSTEM(RigidbodyGraphicsSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto butSystem = REGISTER_SYSTEM(ButtonSystem);
        auto phySystem = REGISTER_SYSTEM(PhysicsSystem);
        REGISTER_SYSTEM(InputSystem);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);
        auto prefabSystem = REGISTER_SYSTEM(PrefabSystem);
        // auto gameLogic = REGISTER_SYSTEM(GameLogic);
        auto transformSystem = REGISTER_SYSTEM(TransformSystem);
        AssetManager::GetInstance()->Init(prefabSystem);
        AssetManager::GetInstance()->LoadAll(AssetManager::GetInstance()->enConfig.assetLoc.c_str());
        //AssetManager::GetInstance()->fileWatcher.Update();
        // TODO: Shift this all into system constructors to clean up core.cpp
        transformSystem->Init();
        RenderHelper::GetInstance()->InitScreenDimension(static_cast<float>(Width), static_cast<float>(Height));
        graSystem->Init();
        uigSystem->Init();
        txtSystem->Init();
        aniSystem->Init();
        butSystem->Init();
        crsSystem->Init();
        phySystem->Init();
        colSystem->Init();
        rrsSystem->Init();
        souSystem->Init();
        

        // Add goFactory to input system's messaging so that it can send msg to it
        Input.BindSystem(gGOFactory);
        Input.BindSystem(gameSystem);
        
        Input.BindSystem(souSystem);

        gameSystem->BindSystem(editorSys);
        gameSystem->BindSystem(butSystem);
        // Add transform system into gGOFactory's observer so that it can send msg to it
        gGOFactory->BindSystem(transformSystem);
        gGOFactory->BindSystem(prefabSystem);
        // Add Scene system into editor's observer
        editorSys->BindSystem(gameSystem);
        editorSys->BindSystem(prefabSystem);

        butSystem->BindSystem(gScriptSystem);

        //glfwSetWindowUserPointer(window, inputSystem.get());
        gScriptSystem->Init();
        Input.Init(window);
        gameSystem->SetScene("Scene1");
#ifndef CM_INSTALLER
        gameSystem->Init(); // Load all GOs from scene file
        
#endif
        //gGOFactory->CreateSceneObject("Scene1"); // TODO: Shift this so that it isnt here and manually being made
        //gGOFactory->ParentAllGO();

        //GameLogic gameLogic;
        //gameLogic->Init();
        //gGOFactory->BindSystem(std::static_pointer_cast<BaseSystem>(gameLogic).get());
       // graSystem->SetScreenSize((GLuint)Width / 100, (GLuint)Height / 100, gGOFactory->mainCam);

        //colSystem->PrintEntities();
        //int objectCount = 0;
        //phySystem->Update();
        
        CarmicahTime::GetInstance()->SetFixedDT(true);
        double accumulatedTime = 0.0;



        //Editor Editor;
        editorSys->Init(window);
        
        
        static bool gameOnly = false;
#ifdef CM_INSTALLER
        gameOnly = true;
        gameSystem->mNextState = SceneState::INITIALISING;
        gameSystem->mRuntime = true; // set it to run time mode
#endif



        SceneToImgui::GetInstance()->CreateFramebuffer(Width, Height);

        while (!glfwWindowShouldClose(window)) {
            CarmicahTime::GetInstance()->StartLoopTimer();
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTime::GetInstance()->FPS())) + " - Scene : " + gameSystem->GetCurrScene();
            glfwSetWindowTitle(window, title.c_str());
            glfwPollEvents(); // this takes 20% of engine run time


            if (gameSystem->mNextState == SceneState::EXIT)
            {
                break;
            }

            // Reload/initialize the scene here
            if (gameSystem->mNextState == SceneState::INITIALISING || gameSystem->mNextState == SceneState::RELOAD)
            {
               // if (oneTimeInit)
                    gameSystem->Init();

               // oneTimeInit = true;
                // FOR TESTING OF GOs MANUALLY
                    
                //wall.SetParent(mainCharacter);
                //GameObject GameObject2;
                //gGOFactory->FetchGO("GameObject2", GameObject2);

                //LinearDirectionalForce rightForce({ 1.0f,0.0f }, 1.0f, 2.0f);

                //GameObject2.GetComponent<RigidBody>().forcesManager.AddLinearForce(rightForce);


                //GameObject mainCharacter;
                //gGOFactory->FetchGO("mainCharacter", mainCharacter);
                //mainCharacter.AddComponent<Script>();
                //mainCharacter.GetComponent<Script>().scriptName = "Carmicah.Player";

                //gameLogic->Init(); // refetch the objects needed

                // if game only then go straight to onstart and runtime
                if (gameOnly || gameSystem->mRuntime)
                    gameSystem->mNextState = SceneState::ONSTART;
            }
            // If the next state was set to ONSTART, means sceneSystem received a play messag
            if (gameSystem->mNextState == SceneState::ONSTART)
            {
                souSystem->PlaySoundThis("BGM_SetupPhase_Mix1", SoundCategory::BGM, SoundSystem::SOUND_INGAME, 0.4f);
                gScriptSystem->OnStart();
                // go to run time after starting up all script objects
                gameSystem->mNextState = gameSystem->mCurrState = SceneState::RUNTIME;
            }
            else if (gameSystem->mCurrState == gameSystem->mNextState)
            {
                CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                colSystem->Update();
                CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");

                gScriptSystem->UpdateScripts(); // TODO: Add this to profiler

                if (gameSystem->mCurrState == SceneState::RUNTIME && SceneWindow::mIsPaused == false)
                {

                    gScriptSystem->OnUpdate((float)CarmicahTime::GetInstance()->GetDeltaTime()); // TODO: Add this to profiler
                    //gameLogic->Update(window);
                    if (CarmicahTime::GetInstance()->IsFixedDT())
                    {
                        accumulatedTime += CarmicahTime::GetInstance()->GetDeltaTime();

                        while (accumulatedTime >= CarmicahTime::GetInstance()->GetDeltaTime())
                        {
                            //phySystem->Update();
#ifdef CM_DEBUG
                            if (phySystem->mDebugPhysics) {
                                // Handle WASD movement during debugPhysics mode
                                if (phySystem->mToggleUpdate)
                                {
                                    phySystem->mToggleUpdate = false;
                                    CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                                    colSystem->CollisionCheck();
                                    CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");

                                    CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                    phySystem->Update();
                                    CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                                }
                            }
                            else {
                                CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                                colSystem->CollisionCheck();
                                CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
                                CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                phySystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            }
#endif

#ifdef CM_RELEASE || CM_INSTALLER
                            CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                            phySystem->Update();
                            CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                            colSystem->CollisionCheck();
                            CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
#endif
                            accumulatedTime -= CarmicahTime::GetInstance()->GetDeltaTime();
                        }
                    }
                    else
                    {
                        //phySystem->Update();
#ifdef CM_DEBUG
                        if (phySystem->mDebugPhysics) {
                            // Handle WASD movement during debugPhysics mode
                            if (phySystem->mToggleUpdate)
                            {
                                phySystem->mToggleUpdate = false;
                                CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                                colSystem->CollisionCheck();
                                CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");

                                CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                phySystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            }
                        }
                        else {
                            CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                            colSystem->CollisionCheck();
                            CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
                            CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                            phySystem->Update();
                            CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                        }
#endif

#ifdef CM_RELEASE || CM_INSTALLER
                        CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                        phySystem->Update();
                        CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                        CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                        colSystem->CollisionCheck();
                        CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
#endif               
                    }
                }


                CarmicahTime::GetInstance()->StartSystemTimer("AnimationSystem");
                aniSystem->Update();
                CarmicahTime::GetInstance()->StopSystemTimer("AnimationSystem");

                CarmicahTime::GetInstance()->StartSystemTimer("SoundSystem");
                souSystem->Update();
                CarmicahTime::GetInstance()->StopSystemTimer("SoundSystem");
                CarmicahTime::GetInstance()->StartGPUTimer();
                CarmicahTime::GetInstance()->StopGPUTimer();


               // glfwMakeContextCurrent(ImGuiWindow);
                if (!gameOnly)
                {
                    CarmicahTime::GetInstance()->StartSystemTimer("EditorSystem");
                    //GameObject fps;
                   // gGOFactory->FetchGO("FPSText", fps);
                    //fps.GetComponent<TextRenderer>().txt = std::to_string((int)CarmicahTime::GetInstance()->FPS());
                    editorSys->Update();
                    editorSys->Render(window);
                    AssetManager::GetInstance()->fileWatcher.Update();
                    CarmicahTime::GetInstance()->StopSystemTimer("EditorSystem");
                }
                CarmicahTime::GetInstance()->StartSystemTimer("RenderingSystems");
                // Update world and local transforms before rendering
                transformSystem->Update();
                // Pushes data into GPU
                graSystem->Update();
                uigSystem->Update();
                crsSystem->Update();
                rrsSystem->Update();
                txtSystem->Update();
                // Done with pushing transforms, reset updates of transforms
                transformSystem->PostUpdate();

                if (!gameOnly)
                {
                    // Game Cam
                    SceneToImgui::GetInstance()->BindFramebuffer(SceneToImgui::GAME_SCENE);
                    RenderHelper::GetInstance()->Render(gGOFactory->mainCam, true);
                    // Editor Cam
                    SceneToImgui::GetInstance()->BindFramebuffer(SceneToImgui::EDITOR_SCENE);
                    RenderHelper::GetInstance()->UpdateEditorCam();
                    RenderHelper::GetInstance()->Render(&RenderHelper::GetInstance()->mEditorCam, true);
                    SceneToImgui::GetInstance()->UnbindFramebuffer();
                }
                else
                {
                    RenderHelper::GetInstance()->Render(gGOFactory->mainCam);
                    // Editor Cam
                    SceneToImgui::GetInstance()->BindFramebuffer(SceneToImgui::GAME_SCENE);
                    RenderHelper::GetInstance()->Render(gGOFactory->mainCam);
                    SceneToImgui::GetInstance()->UnbindFramebuffer();

                }

                CarmicahTime::GetInstance()->StopSystemTimer("RenderingSystems");
                //SceneToImgui::GetInstance()->IDPick();

                // I WILL UPDAAATEEE BUTTONSYSTEM HERE OKKKKAAYYYY, PLS DONT CRASH CRYING EMOJI
				butSystem->Update();

                Input.Update();

               // glfwMakeContextCurrent(window);

                glfwSwapBuffers(window);
                gGOFactory->UpdateDestroyed();
            }

            Input.UpdatePrevInput();
            // shift this here for now cause moving 
            glfwPollEvents(); // this takes 20% of engine run time


            // Don't exit if we're going into onstart
            // only for anything else but that
            if ((gameSystem->mNextState != gameSystem->mCurrState) && gameSystem->mNextState != SceneState::ONSTART )
            {
                souSystem->StopAllSounds();
                gameSystem->Exit();
            }

            CarmicahTime::GetInstance()->StopLoopTimer();
            CarmicahTime::GetInstance()->CalculateSystemPercentages();
            CarmicahTime::GetInstance()->UpdateTime();

        }

        SceneToImgui::GetInstance()->UnloadFramebuffer();

        editorSys->Exit();
        souSystem->Exit();
        colSystem->Exit();
		butSystem->Exit();
        AssetManager::GetInstance()->UnloadAll();
        Serializer.WriteConfig();
        gScriptSystem->CleanUp();

        glfwTerminate();
        return 0;
    }



}
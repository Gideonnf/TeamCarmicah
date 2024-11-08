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
#include "Systems/GameLogic.h"

#include "Systems/GOFactory.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/UIGraphicsSystem.h"
#include "Graphics/TextSystem.h"
#include "Graphics/AnimationSystem.h"
#include "Graphics/ColliderRenderSystem.h"
#include "Graphics/RigidbodyRendererSystem.h"
#include "Systems/CollisionSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/ButtonSystem.h"

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
        int Width = AssetManager::GetInstance()->enConfig.Width;
        int Height = AssetManager::GetInstance()->enConfig.Height;
        std::string defaultScene = AssetManager::GetInstance()->enConfig.defaultScene;
        //CM_CORE_INFO("Reached before window creation");
        GLFWwindow* window = glfwCreateWindow(Width, Height, "Carmicah", NULL, NULL);
        //int bufferWidth, bufferHeight;
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

        CM_CORE_INFO("Starting system init");

        auto editorSys = REGISTER_SYSTEM(Editor);
        REGISTER_SYSTEM(GOFactory);
        REGISTER_SYSTEM(ScriptSystem);
        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto uigSystem = REGISTER_SYSTEM(UIGraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderRenderSystem);
        auto rrsSystem = REGISTER_SYSTEM(RigidbodyRendererSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto butSystem = REGISTER_SYSTEM(ButtonSystem);
        auto phySystem = REGISTER_SYSTEM(PhysicsSystem);
        REGISTER_SYSTEM(InputSystem);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);
       // auto gameLogic = REGISTER_SYSTEM(GameLogic);
        auto transformSystem = REGISTER_SYSTEM(TransformSystem);
        AssetManager::GetInstance()->LoadAll(AssetManager::GetInstance()->enConfig.assetLoc.c_str());
        // TODO: Shift this all into system constructors to clean up core.cpp
        transformSystem->Init();
        graSystem->Init();
        uigSystem->Init(static_cast<float>(Width), static_cast<float>(Height));
        txtSystem->Init(static_cast<float>(Width), static_cast<float>(Height));
        aniSystem->Init();
        butSystem->Init();
        crsSystem->Init();
        phySystem->Init();
        colSystem->Init(); // Set the signature
        rrsSystem->Init();
        souSystem->Init(true);

        // Add goFactory to input system's messaging so that it can send msg to it
        Input.BindSystem(gGOFactory);
        // TODO: Make this easier to write for people
       Input.BindSystem(std::static_pointer_cast<BaseSystem>(gameSystem).get());

       Input.BindSystem(std::static_pointer_cast<BaseSystem>(souSystem).get());

       gameSystem->BindSystem(std::static_pointer_cast<BaseSystem>(editorSys).get());
       gameSystem->BindSystem(std::static_pointer_cast<BaseSystem>(butSystem).get());
        // Add transform system into gGOFactory's observer so that it can send msg to it
        gGOFactory->BindSystem(std::static_pointer_cast<BaseSystem>(transformSystem).get());
        // Add Scene system into editor's observer
        editorSys->BindSystem(std::static_pointer_cast<BaseSystem>(gameSystem).get());

        //glfwSetWindowUserPointer(window, inputSystem.get());
        Input.Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init(); // Load all GOs from scene file

        //gGOFactory->CreateSceneObject("Scene1"); // TODO: Shift this so that it isnt here and manually being made
        //gGOFactory->ParentAllGO();

        //GameLogic gameLogic;
       // gameLogic->Init();
        //gGOFactory->BindSystem(std::static_pointer_cast<BaseSystem>(gameLogic).get());
        graSystem->SetScreenSize((GLuint)Width / 100, (GLuint)Height / 100, gGOFactory->mainCam);

       // colSystem->PrintEntities();
        //int objectCount = 0;
        //phySystem->Update();
        
        CarmicahTime::GetInstance()->SetFixedDT(true);
        double accumulatedTime = 0.0;

       // Editor Editor;
        editorSys->Init(window);
        gScriptSystem->Init();



        SceneToImgui::GetInstance()->CreateFramebuffer(Width, Height);

        while (!glfwWindowShouldClose(window)) {
            CarmicahTime::GetInstance()->StartLoopTimer();
            glfwPollEvents();
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTime::GetInstance()->FPS())) + " - Scene : " + gameSystem->GetCurrScene();
            glfwSetWindowTitle(window, title.c_str());

            if (gameSystem->mNextState == SceneState::EXIT)
            {
                break;
            }

            // Reload/initialize the scene here
            if (gameSystem->mNextState == SceneState::INITIALISING )
            {
                gameSystem->Init();

                // FOR TESTING OF GOs MANUALLY
                    
                //wall.SetParent(mainCharacter);
                GameObject GameObject2;
                gGOFactory->FetchGO("GameObject2", GameObject2);

                LinearDirectionalForce rightForce({ 1.0f,0.0f }, 1.0f, 2.0f);

                GameObject2.GetComponent<RigidBody>().forcesManager.AddLinearForce(rightForce);


                //GameObject mainCharacter;
                //gGOFactory->FetchGO("mainCharacter", mainCharacter);
                //mainCharacter.AddComponent<Script>();
                //mainCharacter.GetComponent<Script>().scriptName = "Carmicah.Player";

                //gameLogic->Init(); // refetch the objects needed
            }
            // If the next state was set to ONSTART, means sceneSystem received a play messag
            if (gameSystem->mNextState == SceneState::ONSTART)
            {
                gScriptSystem->OnStart();
                // go to run time after starting up all script objects
                gameSystem->mNextState = gameSystem->mCurrState = SceneState::RUNTIME;
            }
            else if (gameSystem->mCurrState == gameSystem->mNextState)
            {
                if (gameSystem->mCurrState == SceneState::RUNTIME)
                {

                    gScriptSystem->OnUpdate((float)CarmicahTime::GetInstance()->GetDeltaTime());
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
                                    colSystem->Update();
                                    CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");

                                    CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                    phySystem->Update();
                                    CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                                }
                            }
                            else {
                                CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                                colSystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
                                CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                phySystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            }
    #endif

    #ifdef CM_RELEASE
                            CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                            phySystem->Update();
                            CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                            colSystem->Update();
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
                                colSystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");

                                CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                                phySystem->Update();
                                CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                            }
                        }
                        else {
                            CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                            colSystem->Update();
                            CarmicahTime::GetInstance()->StopSystemTimer("CollisionSystem");
                            CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                            phySystem->Update();
                            CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                        }
    #endif

    #ifdef CM_RELEASE
                        CarmicahTime::GetInstance()->StartSystemTimer("PhysicsSystem");
                        phySystem->Update();
                        CarmicahTime::GetInstance()->StopSystemTimer("PhysicsSystem");

                        CarmicahTime::GetInstance()->StartSystemTimer("CollisionSystem");
                        colSystem->Update();
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
                CarmicahTime::GetInstance()->StartSystemTimer("EditorSystem");
                GameObject fps;
                gGOFactory->FetchGO("FPSText", fps);
                fps.GetComponent<TextRenderer>().txt = std::to_string((int)CarmicahTime::GetInstance()->FPS());
                editorSys->Update();
                editorSys->Render(window);
                CarmicahTime::GetInstance()->StopSystemTimer("EditorSystem");

                SceneToImgui::GetInstance()->BindFramebuffer();
                CarmicahTime::GetInstance()->StartSystemTimer("RenderingSystems");
                transformSystem->Update(); // Update world and local transforms before rendering
                graSystem->Render(gGOFactory->mainCam);
                uigSystem->Render();
                crsSystem->Render(gGOFactory->mainCam);
                rrsSystem->Render(gGOFactory->mainCam);
                txtSystem->Render();
                CarmicahTime::GetInstance()->StopSystemTimer("RenderingSystems");
                //SceneToImgui::GetInstance()->IDPick();

                // I WILL UPDAAATEEE BUTTONSYSTEM HERE OKKKKAAYYYY, PLS DONT CRASH CRYING EMOJI
				butSystem->Update();
                
                SceneToImgui::GetInstance()->UnbindFramebuffer();
               // glfwMakeContextCurrent(window);

                glfwSwapBuffers(window);
                gGOFactory->UpdateDestroyed();
            }

            // Don't exit if we're going into onstart
            // only for anything else but that
            if ((gameSystem->mNextState != gameSystem->mCurrState) && gameSystem->mNextState != SceneState::ONSTART )
            {
                gameSystem->Exit();
            }

            CarmicahTime::GetInstance()->StopLoopTimer();
            CarmicahTime::GetInstance()->CalculateSystemPercentages();
            CarmicahTime::GetInstance()->UpdateTime();
            Input.UpdatePrevInput();

        }

        SceneToImgui::GetInstance()->UnloadFramebuffer();

        AssetManager::GetInstance()->UnloadAll();
        editorSys->Exit();
        souSystem->Exit();
        colSystem->Exit();
		butSystem->Exit();
        Serializer.WriteConfig();
        gScriptSystem->CleanUp();

        glfwTerminate();
        return 0;
    }



}
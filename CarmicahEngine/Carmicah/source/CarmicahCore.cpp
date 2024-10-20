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
#include "Systems/GameLogic.h"

#include "Systems/GOFactory.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/TextSystem.h"
#include "Graphics/AnimationSystem.h"
#include "Graphics/ColliderRenderSystem.h"
#include "Graphics/RigidbodyRendererSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SoundSystem.h"
#include "Input/InputSystem.h"
#include "Systems/SceneSystem.h"
#include "Systems/SerializerSystem.h"
#include "CarmicahTime.h"
#include "Systems/AssetManager.h"
#include "Editor/SceneToImgui.h"

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

        GLFWwindow* window = glfwCreateWindow(Width, Height, "Carmicah", NULL, NULL);
        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
        glfwMakeContextCurrent(window);

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

     /*   GLFWwindow* ImGuiWindow = glfwCreateWindow(WIDTH, HEIGHT, "ImGui", NULL, NULL);

        if (ImGuiWindow == NULL)
        {
            CM_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }*/

        CarmicahTimer::StartTime();
        CarmicahTimer::InitGPUProfiling();

        glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(RigidBody);
        REGISTER_COMPONENT(Renderer);
        REGISTER_COMPONENT(Animation);
        REGISTER_COMPONENT(TextRenderer);
        REGISTER_COMPONENT(UITransform);

        auto editorSys = REGISTER_SYSTEM(Editor);
        REGISTER_SYSTEM(GOFactory);
        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderRenderSystem);
        auto rrsSystem = REGISTER_SYSTEM(RigidbodyRendererSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto phySystem = REGISTER_SYSTEM(PhysicsSystem);
        auto inputSystem = REGISTER_SYSTEM(InputSystem);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);
        auto gameLogic = REGISTER_SYSTEM(GameLogic);

        AssetManager::GetInstance()->LoadAll(AssetManager::GetInstance()->enConfig.assetLoc.c_str());
        graSystem->Init();
        txtSystem->Init();
        aniSystem->Init();
        crsSystem->Init();
        phySystem->Init();
        colSystem->Init(); // Set the signature
        rrsSystem->Init();
        souSystem->Init(true);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init();

        //GameLogic gameLogic;
        gameLogic->Init();
        gGOFactory->BindSystem(std::static_pointer_cast<BaseSystem>(gameLogic).get());
        graSystem->SetScreenSize((GLuint)Width / 100, (GLuint)Height / 100, gGOFactory->mainCam);

        colSystem->PrintEntities();
        //int objectCount = 0;
        //phySystem->Update();
        

       // Editor Editor;
        editorSys->Init(window);

        SceneToImgui::GetInstance()->create_framebuffer(bufferWidth, bufferHeight);

        while (!glfwWindowShouldClose(window)) {
            CarmicahTimer::StartLoopTimer();
            glfwPollEvents();
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS())) + " - Scene : " + gameSystem->GetCurrScene();
            glfwSetWindowTitle(window, title.c_str());

            if (gameSystem->mNextState == SceneState::EXIT)
            {
                break;
            }

            if (gameSystem->mNextState == SceneState::INITIALISING)
            {
                gameSystem->Init();
                gameLogic->Init(); // refetch the objects needed
            }
            else if (gameSystem->mCurrState == gameSystem->mNextState)
            {
                //phySystem->Update();
                gameLogic->Update(window);
                //gameLogic.Update();
                //phySystem->Update();
                #ifdef CM_DEBUG
                if (phySystem->mDebugPhysics) {
                    // Handle WASD movement during debugPhysics mode
                    if (phySystem->mToggleUpdate)
                    {
                        phySystem->mToggleUpdate = false;
                        CarmicahTimer::StartSystemTimer("CollisionSystem");
                        colSystem->Update();
                        CarmicahTimer::StopSystemTimer("CollisionSystem");

                        CarmicahTimer::StartSystemTimer("PhysicsSystem");
                        phySystem->Update();
                        CarmicahTimer::StopSystemTimer("PhysicsSystem");

                    }
                }
                else {
                    CarmicahTimer::StartSystemTimer("CollisionSystem");
                    colSystem->Update();
                    CarmicahTimer::StopSystemTimer("CollisionSystem");
                    CarmicahTimer::StartSystemTimer("PhysicsSystem");
                    phySystem->Update();
                    CarmicahTimer::StopSystemTimer("PhysicsSystem");

                 }

                #endif

                #ifdef CM_RELEASE
                CarmicahTimer::StartSystemTimer("PhysicsSystem");
                phySystem->Update();
                CarmicahTimer::StopSystemTimer("PhysicsSystem");

                CarmicahTimer::StartSystemTimer("CollisionSystem");
                colSystem->Update();
                CarmicahTimer::StopSystemTimer("CollisionSystem");
                #endif

                CarmicahTimer::StartSystemTimer("AnimationSystem");
                aniSystem->Update();
                CarmicahTimer::StopSystemTimer("AnimationSystem");

                CarmicahTimer::StartSystemTimer("SoundSystem");
                souSystem->Update();
                CarmicahTimer::StopSystemTimer("SoundSystem");
                CarmicahTimer::StartGPUTimer();
                CarmicahTimer::StopGPUTimer();


               // glfwMakeContextCurrent(ImGuiWindow);
                CarmicahTimer::StartSystemTimer("EditorSystem");
                editorSys->Update();
                editorSys->Render(window);
                CarmicahTimer::StopSystemTimer("EditorSystem");

                SceneToImgui::GetInstance()->bind_framebuffer();
                CarmicahTimer::StartSystemTimer("RenderingSystems");
               // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                graSystem->Render(gGOFactory->mainCam);
                crsSystem->Render(gGOFactory->mainCam);
                rrsSystem->Render(gGOFactory->mainCam);
                txtSystem->Render((GLuint)Width, (GLuint)Height);
                CarmicahTimer::StopSystemTimer("RenderingSystems");
                SceneToImgui::GetInstance()->unbind_framebuffer();
               // glfwMakeContextCurrent(window);

                glfwSwapBuffers(window);
                gGOFactory->UpdateDestroyed();
            }

            if (gameSystem->mNextState != gameSystem->mCurrState)
            {
                gameSystem->Exit();
            }

            CarmicahTimer::StopLoopTimer();
            CarmicahTimer::CalculateSystemPercentages();
            CarmicahTimer::UpdateElapsedTime();
            inputSystem->UpdatePrevInput();

        }

        AssetManager::GetInstance()->UnloadAll();
        editorSys->Exit();
        souSystem->Exit();
        colSystem->Exit();
        Serializer.WriteConfig();

        glfwTerminate();
        return 0;
    }



}
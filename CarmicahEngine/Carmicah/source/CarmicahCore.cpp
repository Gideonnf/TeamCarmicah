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
#include "Systems/GraphicsSystem.h"
#include "Systems/TextSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/ColliderRenderSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/SceneSystem.h"
#include "Systems/SerializerSystem.h"
#include "CarmicahTime.h"
#include "AssetManager.h"

namespace Carmicah
{
    const char* sceneName{ "Scene1" };
    const char* scene2Name{ "Scene2" };
    const GLuint WIDTH = 600, HEIGHT = 320;
    const char* assetsLoc{ "../Assets" };

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
        EnableMemoryLeakChecking();
        SerializerSystem::GetInstance()->LoadConfig(*this);
        Carmicah::Log::init();
        CM_CORE_INFO("Core Logger Initialized");
        CM_INFO("Client Logger Initialized");

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow((GLuint)Width, (GLuint)Height, "Carmicah", NULL, NULL);
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

        GLFWwindow* ImGuiWindow = glfwCreateWindow(WIDTH, HEIGHT, "ImGui", NULL, NULL);

        if (ImGuiWindow == NULL)
        {
            CM_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        CarmicahTimer::StartTime();
        CarmicahTimer::InitGPUProfiling();

        glViewport(0, 0, (GLuint)Width, (GLuint)Height);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(RigidBody);
        REGISTER_COMPONENT(Renderer);
        REGISTER_COMPONENT(Animation);
        REGISTER_COMPONENT(TextRenderer);
        REGISTER_COMPONENT(UITransform);

        REGISTER_SYSTEM(GOFactory);
        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderRenderSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto phySystem = REGISTER_SYSTEM(PhysicsSystem);
        auto inputSystem = REGISTER_SYSTEM(InputSystem);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);

        AssetManager::GetInstance()->LoadAll(assetsLoc);
        graSystem->Init();
        txtSystem->Init();
        aniSystem->Init();
        crsSystem->Init();
        colSystem->Init();
        phySystem->Init();
        souSystem->Init(true);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init();

        graSystem->SetScreenSize(WIDTH / 100, HEIGHT / 100, gGOFactory->mainCam);

        GameObject newObj = gGOFactory->CreateGO();
        newObj.AddComponent<Transform>();
        newObj.GetComponent<Transform>().xPos = 1.0f;
        newObj.GetComponent<Transform>().yPos = 2.0f;
        newObj.GetComponent<Transform>().xScale = 1.0f;
        newObj.GetComponent<Transform>().yScale = 1.0f;
        newObj.AddComponent<Collider2D>();
        newObj.GetComponent<Collider2D>().shape = "DebugSquare";
        newObj.AddComponent<RigidBody>();
        newObj.GetComponent<RigidBody>().velocity.x = 2.0f;
        newObj.GetComponent<RigidBody>().velocity.y = 2.0f;
        newObj.GetComponent<RigidBody>().gravity = -5.0f;
        newObj.GetComponent<RigidBody>().objectType = "Dynamic";
        newObj.AddComponent<Renderer>();
        newObj.GetComponent<Renderer>().model = "Square";
        newObj.GetComponent<Renderer>().texture = "Bullet";
        newObj.GetComponent<Renderer>().texureMat = glm::mat3(1);

        GameObject wall = gGOFactory->CreateGO();
        wall.AddComponent<Transform>();
        wall.GetComponent<Transform>().xPos = 4.0f;
        wall.GetComponent<Transform>().xScale = 1.0f;
        wall.GetComponent<Transform>().yScale = 1.0f;
        wall.AddComponent<Collider2D>();
        wall.GetComponent<Collider2D>().shape = "DebugSquare";
        wall.AddComponent<RigidBody>();
        wall.GetComponent<RigidBody>().objectType = "Static";
        wall.AddComponent<Renderer>();
        wall.GetComponent<Renderer>().model = "Square";
        wall.GetComponent<Renderer>().texture = "wall";
        wall.GetComponent<Renderer>().texureMat = glm::mat3(1);

        colSystem->PrintEntities();

        Editor Editor;
        Editor.Init(ImGuiWindow);

        while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(ImGuiWindow)) {
            CarmicahTimer::StartLoopTimer();
            CarmicahTimer::UpdateElapsedTime();
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
            }
            else if (gameSystem->mCurrState == gameSystem->mNextState)
            {
                CarmicahTimer::StartSystemTimer("CollisionSystem");
                colSystem->Update();
                CarmicahTimer::StopSystemTimer("CollisionSystem");

                CarmicahTimer::StartSystemTimer("PhysicsSystem");
                phySystem->Update();
                CarmicahTimer::StopSystemTimer("PhysicsSystem");

                CarmicahTimer::StartSystemTimer("AnimationSystem");
                aniSystem->Update();
                CarmicahTimer::StopSystemTimer("AnimationSystem");

                CarmicahTimer::StartGPUTimer();
                CarmicahTimer::StartSystemTimer("RenderingSystems");
                graSystem->Render(gGOFactory->mainCam);
                crsSystem->Render(gGOFactory->mainCam);
                txtSystem->Render(WIDTH, HEIGHT);
                CarmicahTimer::StopSystemTimer("RenderingSystems");
                CarmicahTimer::StopGPUTimer();

                CarmicahTimer::StartSystemTimer("SoundSystem");
                souSystem->Update();
                CarmicahTimer::StopSystemTimer("SoundSystem");

                glfwSwapBuffers(window);

                glfwMakeContextCurrent(ImGuiWindow);
                CarmicahTimer::StartSystemTimer("EditorSystem");
                Editor.Update();
                Editor.Render(ImGuiWindow);
                CarmicahTimer::StopSystemTimer("EditorSystem");
                glfwMakeContextCurrent(window);

                if (InputSystem::GetInstance()->IsKeyPressed(Keys::KEY_SPACEBAR))
                {
                    gameSystem->ChangeScene(scene2Name);
                }

                if (InputSystem::GetInstance()->IsKeyPressed(Keys::KEY_1))
                {
                    GameObject duckObj = gGOFactory->FetchGO("Duck");
                    duckObj.Destroy();
                }

                gGOFactory->UpdateDestroyed();
            }

            if (gameSystem->mNextState != gameSystem->mCurrState)
            {
                gameSystem->Exit();
            }

            CarmicahTimer::StopLoopTimer();
            CarmicahTimer::CalculateSystemPercentages();
        }

        AssetManager::GetInstance()->UnloadAll();
        Editor.Exit();
        souSystem->Exit();
        colSystem->Exit();
        SerializerSystem::GetInstance()->WriteConfig(*this);

        glfwTerminate();
        return 0;
    }
}
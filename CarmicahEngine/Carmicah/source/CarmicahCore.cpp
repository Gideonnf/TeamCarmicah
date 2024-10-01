#include "pch.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include "CarmicahCore.h"
#include <spdlog/spdlog.h>
#include <log.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>   
#include <ImGUI/imgui_impl_opengl3.h>
#include "Systems/GOFactory.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "Editor/Editor.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"

#include "Systems/GOFactory.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/TextSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/ColliderRenderSystem.h"
#include "Systems/CollisionSystem.h"
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

    Application::Application()
    {
    }

    Application::~Application()
    {
    }

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
        Serializer.LoadConfig(*this);
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

        glViewport(0, 0, (GLuint)Width, (GLuint)Height);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
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
        auto inputSystem = REGISTER_SYSTEM(InputSystem);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);

        AssetManager::GetInstance()->LoadAll(assetsLoc);
        graSystem->Init();
        txtSystem->Init();
        aniSystem->Init();
        crsSystem->Init();
        colSystem->Init();
        souSystem->Init(true);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init();

        graSystem->SetScreenSize(WIDTH / 100, HEIGHT / 100, gGOFactory->mainCam);
        colSystem->PrintEntities();

        Editor Editor;
        Editor.Init(ImGuiWindow);

        double testTime = 0.0;
        while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(ImGuiWindow)) {
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
                CarmicahTimer::StartProfileTimer("Collision System");
                colSystem->Update();
                CarmicahTimer::EndProfileTimer("Collision System");

                CarmicahTimer::StartProfileTimer("Animation System");
                aniSystem->Update();
                CarmicahTimer::EndProfileTimer("Animation System");

                CarmicahTimer::StartProfileTimer("Graphics System");
                graSystem->Render(gGOFactory->mainCam);
                CarmicahTimer::EndProfileTimer("Graphics System");

                CarmicahTimer::StartProfileTimer("Collider Render System");
                crsSystem->Render(gGOFactory->mainCam);
                CarmicahTimer::EndProfileTimer("Collider Render System");

                CarmicahTimer::StartProfileTimer("Text System");
                txtSystem->Render(WIDTH, HEIGHT);
                CarmicahTimer::EndProfileTimer("Text System");

                CarmicahTimer::StartProfileTimer("Sound System");
                souSystem->Update();
                CarmicahTimer::EndProfileTimer("Sound System");

                glfwSwapBuffers(window);

                glfwMakeContextCurrent(ImGuiWindow);
                CarmicahTimer::StartProfileTimer("Editor");
                Editor.Update();
                Editor.Render(ImGuiWindow);
                CarmicahTimer::EndProfileTimer("Editor");
                glfwMakeContextCurrent(window);

                if (Input.IsKeyPressed(Keys::KEY_SPACEBAR))
                {
                    gameSystem->ChangeScene(scene2Name);
                }

                if (Input.IsKeyPressed(Keys::KEY_1))
                {
                    GameObject duckObj = gGOFactory->FetchGO("Duck");
                    duckObj.Destroy();
                }

                gGOFactory->UpdateDestroyed();

                // Reset profiling data at the end of each frame
                CarmicahTimer::ResetProfilingData();
            }

            if (gameSystem->mNextState != gameSystem->mCurrState)
            {
                gameSystem->Exit();
            }
        }

        AssetManager::GetInstance()->UnloadAll();
        Editor.Exit();
        souSystem->Exit();
        colSystem->Exit();
        Serializer.WriteConfig(*this);

        glfwTerminate();
        return 0;
    }
}
#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <log.h>

#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"

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

#include "CarmicahTime.h"
#include "AssetManager.h"


namespace Carmicah
{
    const GLuint WIDTH = 800, HEIGHT = 600;
    const char* sceneName{ "../Assets/Scene/Scene1.json" };
    const char* assetsLoc{ "../Assets" };

    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void EnableMemoryLeakChecking(int breakAlloc = -1)
    {
        //Set the leak checking flag
        int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(tmpDbgFlag);

        //If a valid break alloc provided set the breakAlloc
        if (breakAlloc != -1) _CrtSetBreakAlloc(breakAlloc);
    }

    int Application::run()
    {
        EnableMemoryLeakChecking();

        Carmicah::Log::init();
        CM_CORE_INFO("Core Logger Initialized");
        CM_INFO("Client Logger Initialized");

        glfwInit();

        // Set required options for GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carmicah", NULL, NULL);
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

        //auto fpsCounter = std::make_unique<FPSCounter>();
        //fpsCounter->Init();
        CarmicahTimer::StartTime();

        glViewport(0, 0, WIDTH, HEIGHT);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(Renderer);
        REGISTER_COMPONENT(Animation);
        REGISTER_COMPONENT(TextRenderer);
        REGISTER_COMPONENT(UITransform);

        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderRenderSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto inputSystem = REGISTER_SYSTEM(InputSystem);
        REGISTER_SYSTEM(GOFactory);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);

        AssetManager::GetInstance()->LoadAll(assetsLoc);
        graSystem->Init();
        txtSystem->Init();
        aniSystem->Init();
        crsSystem->Init();
        colSystem->Init(); // Set the signature
        souSystem->Init(false);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->Init(sceneName);
        graSystem->SetScreenSize(WIDTH / 100, HEIGHT / 100, gGOFactory->mainCam);

        //GameObject newObj;
        //Transform playerTrans{ 1, 1, 1 };
        //Collider2D playerCollider{ 1, 2, 3, 4 };
        //newObj.AddComponent<Transform>(playerTrans);
        //newObj.AddComponent<Collider2D>(playerCollider);
        double testTime = 0.0;
        while (!glfwWindowShouldClose(window)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();
            glfwPollEvents();
            testTime += CarmicahTimer::GetDt();
            std::cout << testTime << std::endl;
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS()));
            glfwSetWindowTitle(window, title.c_str());

            gameSystem->Update();
            //newObj.GetComponent<Transform>().xPos += 1;
            colSystem->Update();

            graSystem->Render(gGOFactory->mainCam);
            txtSystem->Render(WIDTH, HEIGHT);
            aniSystem->Update();
            crsSystem->Render(gGOFactory->mainCam);
            souSystem->Update();
            glfwSwapBuffers(window);

            
            
        }

        AssetManager::GetInstance()->UnloadAll();
        //fpsCounter->Exit();
        colSystem->Exit();

        glfwTerminate();
        return 0;
    }
}
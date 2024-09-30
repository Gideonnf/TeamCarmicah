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
#include "Components/RigidBody.h"
#include "Components/Gravity.h"
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
    const GLuint WIDTH = 1920, HEIGHT = 1080;
    const char* assetsLoc{ "../Assets" };
    bool leftPressed = false;
    bool rightPressed = false;

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
        Serializer.LoadConfig(*this);
        //std::cout << Width << Height << std::endl;
        Carmicah::Log::init();
        CM_CORE_INFO("Core Logger Initialized");
        CM_INFO("Client Logger Initialized");

        glfwInit();

        // Set required options for GLFW
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
        //glfwMakeContextCurrent(ImGuiWindow);

        if (ImGuiWindow == NULL)
        {
            CM_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        //auto fpsCounter = std::make_unique<FPSCounter>();
        //fpsCounter->Init();
        CarmicahTimer::StartTime();

        glViewport(0, 0, (GLuint)Width, (GLuint)Height);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(Renderer);
        REGISTER_COMPONENT(Animation);
        REGISTER_COMPONENT(RigidBody);
        REGISTER_COMPONENT(Gravity);
        REGISTER_COMPONENT(TextRenderer);
        REGISTER_COMPONENT(UITransform);

        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto txtSystem = REGISTER_SYSTEM(TextSystem);
        auto aniSystem = REGISTER_SYSTEM(AnimationSystem);
        auto crsSystem = REGISTER_SYSTEM(ColliderRenderSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto phySystem = REGISTER_SYSTEM(PhysicsSystem);
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
        phySystem->Init();
        souSystem->Init(false);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init();

        graSystem->SetScreenSize(WIDTH / 100, HEIGHT / 100, gGOFactory->mainCam);
        colSystem->PrintEntities();

        //GameObject newObj = gGOFactory->CreateGO();
        //newObj.AddComponent<Transform>();
        //newObj.AddComponent<Collider2D>();
        colSystem->PrintEntities();

        //Testing prefab
        //gGOFactory->CreatePrefab("Duck");
        //GameObject newObj = gGOFactory->FetchGO("Duck");
        //newObj.GetComponent<Transform>().xPos = -2.0;

        Editor Editor;
        Editor.Init(ImGuiWindow);

        GameObject newObj = gGOFactory->CreateGO();
        Transform playerTrans{ -5.0f, 0.0f, 1.0f , 0.0f, 1.f, 1.f , true};
        RigidBody playerPhysics{ {0.0f, 0.0f}, {0.0f,0.0f}, {0.0f,0.0f},0.0f, KINEMATIC};
        Collider2D playerCollider{ {0.0f, 0.0f}, {0.0f,0.0f}, "debugSquare"};
        Gravity   playerGravity{ -20.0f };
        Renderer  playerRender{ "Square", "mc_test", glm::mat3(1) };


        newObj.AddComponent<Transform>(playerTrans);
        newObj.AddComponent<RigidBody>(playerPhysics);
        newObj.AddComponent<Collider2D>(playerCollider);
        newObj.AddComponent<Renderer>(playerRender);
        newObj.AddComponent<Gravity>(playerGravity);

        GameObject wall = gGOFactory->CreateGO();
        Transform wallTrans{ 5.0f, 0.0f, 1.0f , 0.0f, 1.f, 1.f , true};
        RigidBody wallPhysics{ {0.0f, 0.0f}, {0.0f,0.0f}, {0.0f,0.0f},0.0f, STATIC};
        Collider2D wallCollider{ {0.0f,0.0f},{0.0f,0.0f}, "debugSquare" };
        Gravity   wallGravity{ 0.0f };
        Renderer  wallRender{ "Square", "wall", glm::mat3(1) };
        

        wall.AddComponent<Transform>(wallTrans);
        wall.AddComponent<RigidBody>(wallPhysics);
        wall.AddComponent<Collider2D>(wallCollider);
        wall.AddComponent<Gravity>(wallGravity);
        wall.AddComponent<Renderer>(wallRender);

        GameObject ball = gGOFactory->CreateGO();
        Transform ballTrans{ -5.0f, 0.0f, 1.0f, 0.0f, 1.f, 1.f, true };
        RigidBody ballPhysics{ {2.0f, 3.0f}, {0.0f,0.0f}, {0.0f,0.0f},0.f, DYNAMIC};
        Gravity ballGravity{ -10.0f };
        Collider2D ballCollider{ {0.0f,0.0f},{0.0f,0.0f}, "debugSquare" };
        Renderer ballRender{ "Square", "bullet", glm::mat3(1) };

        ball.AddComponent<Transform>(ballTrans);
        ball.AddComponent<RigidBody>(ballPhysics);
        ball.AddComponent<Collider2D>(ballCollider);
        ball.AddComponent<Gravity>(ballGravity);
        ball.AddComponent<Renderer>(ballRender);

        double testTime = 0.0;
        while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(ImGuiWindow)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();
            glfwPollEvents();
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS())) + " - Scene : " + gameSystem->GetCurrScene();
            glfwSetWindowTitle(window, title.c_str());
            //GameObject test = 

            if (gameSystem->mNextState == SceneState::EXIT)
            {
                // Closing Engine
                // Dont init a new scene
                // Any exit of systems can be ran here or at the bottom, outside of loop maybe
                break;
            }
            
           // gameSystem->Update();
            if (gameSystem->mNextState == SceneState::INITIALISING)
            {
                gameSystem->Init();
            }
            else if (gameSystem->mCurrState == gameSystem->mNextState)
            {

                colSystem->Update();
                aniSystem->Update();

                graSystem->Render(gGOFactory->mainCam);
                crsSystem->Render(gGOFactory->mainCam);
                txtSystem->Render(WIDTH, HEIGHT);
                souSystem->Update();
                glfwSwapBuffers(window);

                glfwMakeContextCurrent(ImGuiWindow);
                Editor.Update();
                Editor.Render(ImGuiWindow);
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
            }
            
            // Changing of scene/closing of engine
            // run exit to clear objects
            if (gameSystem->mNextState != gameSystem->mCurrState)
            {
                // Clean all entities here
                gameSystem->Exit();
            }


        }
        //Width = 800;
        //Height = 600;

        AssetManager::GetInstance()->UnloadAll();
        //fpsCounter->Exit();
        Editor.Exit();
        colSystem->Exit();
        Serializer.WriteConfig(*this);

        glfwTerminate();
        return 0;
    }
}
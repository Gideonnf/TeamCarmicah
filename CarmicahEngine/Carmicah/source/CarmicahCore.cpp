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
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"

#include "Systems/GOFactory.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/TextSystem.h"
#include "Graphics/AnimationSystem.h"
#include "Graphics/ColliderRenderSystem.h"
#include "Graphics/RigidbodyRendererSystem.h"
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
        auto rrsSystem = REGISTER_SYSTEM(RigidbodyRendererSystem);
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
        rrsSystem->Init();
        colSystem->Init(); // Set the signature
        phySystem->Init();
        souSystem->Init(false);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init(window);
        gameSystem->SetScene("Scene1");
        gameSystem->Init();

        graSystem->SetScreenSize(WIDTH / 100, HEIGHT / 100, gGOFactory->mainCam);

        GameObject newObj = gGOFactory->CreateGO();
        newObj.AddComponent<Transform>();
        newObj.GetComponent<Transform>().xPos = 1.0f;
        newObj.GetComponent<Transform>().yPos = 3.0f;
        newObj.GetComponent<Transform>().xScale = 1.0f;
        newObj.GetComponent<Transform>().yScale = 1.0f;
        newObj.GetComponent<Transform>().notUpdated = false;
        newObj.AddComponent<Collider2D>();
        newObj.GetComponent<Collider2D>().shape = "DebugSquare";
        newObj.AddComponent<RigidBody>();
        newObj.GetComponent<RigidBody>().velocity.x = 1.0f;
        newObj.GetComponent<RigidBody>().velocity.y = 0.0f;
        newObj.GetComponent<RigidBody>().gravity = -0.5f;
        newObj.GetComponent<RigidBody>().objectType = "Dynamic";
        newObj.AddComponent<Renderer>();
        newObj.GetComponent<Renderer>().model = "Square";
        newObj.GetComponent<Renderer>().texture = "Bullet";
        newObj.GetComponent<Renderer>().texureMat = glm::mat3(1);

        GameObject ball = gGOFactory->CreatePrefab("Duck");
        ball.GetComponent<Transform>().xPos = -5.0f;
        ball.GetComponent<Transform>().yPos = 0.0f;
        ball.GetComponent<Transform>().xScale = 0.5f;
        ball.GetComponent<Transform>().yScale = 0.5f;
        ball.GetComponent<Transform>().notUpdated = false;
        ball.AddComponent<Collider2D>();
        ball.GetComponent<Collider2D>().shape = "DebugSquare";
        ball.AddComponent<RigidBody>();
        ball.GetComponent<RigidBody>().velocity.x = 1.0f;
        ball.GetComponent<RigidBody>().velocity.y = 0.0f;
        ball.GetComponent<RigidBody>().gravity = 0.0f;
        ball.GetComponent<RigidBody>().objectType = "Dynamic";
        ball.AddComponent<Renderer>();
        ball.GetComponent<Renderer>().model = "Square";
        ball.GetComponent<Renderer>().texture = "Bullet";
        ball.GetComponent<Renderer>().texureMat = glm::mat3(1);

        GameObject ball2 = gGOFactory->CreatePrefab("Duck");
        ball2.GetComponent<Transform>().xScale = 0.5f;
        ball2.GetComponent<Transform>().yScale = 0.5f;
        ball2.GetComponent<Transform>().notUpdated = false;
        ball2.AddComponent<Collider2D>();
        ball2.GetComponent<Collider2D>().shape = "DebugSquare";
        ball2.AddComponent<RigidBody>();
        ball2.GetComponent<RigidBody>().velocity.x = -1.0f;
        ball2.GetComponent<RigidBody>().velocity.y = 0.0f;
        ball2.GetComponent<RigidBody>().gravity = 0.0f;
        ball2.GetComponent<RigidBody>().objectType = "Dynamic";


        GameObject wall = gGOFactory->CreateGO();
        wall.AddComponent<Transform>();
        wall.GetComponent<Transform>().xPos = 4.0f;
        wall.GetComponent<Transform>().yPos = 0.0f;
        wall.GetComponent<Transform>().xScale = 1.0f;
        wall.GetComponent<Transform>().yScale = 1.0f;
        wall.AddComponent<Collider2D>();
        wall.GetComponent<Collider2D>().shape = "DebugSquare";
        wall.AddComponent<RigidBody>();
        wall.GetComponent<RigidBody>().objectType = "Static";
        wall.AddComponent<Renderer>();
        wall.GetComponent<Renderer>().model = "Square";
        wall.GetComponent<Renderer>().texture = "wall2";
        wall.GetComponent<Renderer>().texureMat = glm::mat3(1);

        colSystem->PrintEntities();

        //Testing prefab
        
        //newObj.GetComponent<Transform>().xPos = -2.0;
        //int objectCount = 0;

        Editor Editor;
        Editor.Init(ImGuiWindow);
        bool is_P_pressed = false;

        double testTime = 0.0;
        while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(ImGuiWindow)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();
            glfwPollEvents();
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS())) + " - Scene : " + gameSystem->GetCurrScene();
            glfwSetWindowTitle(window, title.c_str());

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
                #ifdef CM_DEBUG
                if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
                {
                
                    if (is_P_pressed == false)
                    {
                        is_P_pressed = true;
                        phySystem->Update();
                        colSystem->Update();

                        
                    }

                }
                else 
                {
                    is_P_pressed = false;
                }
                #endif

                #ifdef CM_RELEASE
                phySystem->Update();
                colSystem->Update();
                #endif

                aniSystem->Update();

                graSystem->Render(gGOFactory->mainCam);
                crsSystem->Render(gGOFactory->mainCam);
                rrsSystem->Render(gGOFactory->mainCam);
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
        souSystem->Exit();
        colSystem->Exit();
        Serializer.WriteConfig(*this);

        glfwTerminate();
        return 0;
    }
}
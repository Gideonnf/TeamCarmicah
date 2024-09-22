#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <log.h>
#include "Systems/GOFactory.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/SceneSystem.h"
#include "CarmicahTime.h"


namespace Carmicah
{


    const GLuint WIDTH = 800, HEIGHT = 600;
    const char* sceneName{"../Assets/Scene/Scene1.json"};

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

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    int Application::run()
    {
        EnableMemoryLeakChecking();

        Carmicah::Log::init();
        CM_CORE_INFO("Core Logger Initialized");
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
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        glfwSetKeyCallback(window, key_callback);

        int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (version == 0)
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        glViewport(0, 0, WIDTH, HEIGHT);

        REGISTER_COMPONENT(Transform);
        REGISTER_COMPONENT(Collider2D);
        REGISTER_COMPONENT(Renderer);

        auto graSystem = REGISTER_SYSTEM(GraphicsSystem);
        auto colSystem = REGISTER_SYSTEM(CollisionSystem);
        auto inputSystem = REGISTER_SYSTEM(InputSystem);
        REGISTER_SYSTEM(GOFactory);
        auto souSystem = REGISTER_SYSTEM(SoundSystem);
        auto gameSystem = REGISTER_SYSTEM(SceneSystem);

        graSystem->Init();
        colSystem->Init(); // Set the signature
        souSystem->Init(false);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init();
        gameSystem->Init(sceneName);
       // Import();
        //Entity player = EntityManager::GetInstance()->CreateEntity();
        //Transform playerTrans{ 0.5f, 0.5f, 1.f, 45.f, 1.f, 1.f};
        //Collider2D playerCollider{ 1, 2, 3, 4 };
        //Renderer toRender{};
        //GameObject newObj = gGOFactory->CreateGO();
        //newObj.AddComponent<Transform>(playerTrans);
        //newObj.AddComponent<Collider2D>(playerCollider);
        //newObj.AddComponent<Renderer>(toRender);
        colSystem->PrintEntities();

        // Start timer
        //CarmicahTimer::StartTime();


        while (!glfwWindowShouldClose(window)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();

            glfwPollEvents();

            gameSystem->Update();
            //newObj.GetComponent<Transform>().xPos += 1;
            colSystem->Update();

            souSystem->Update();
            graSystem->Render(window);
        }

        souSystem->Exit();
        graSystem->Exit();

       // Export();

        glfwTerminate();


        return 0;
    }

}
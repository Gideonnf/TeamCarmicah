#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <../log.h>
#include "Systems/GOFactory.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/SoundSystem.h"
#include "CarmicahTime.h"


namespace Carmicah
{


    const GLuint WIDTH = 800, HEIGHT = 600;

    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    int Application::run()
    {
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

        ComponentManager::GetInstance()->RegisterComponent<Transform>();
        ComponentManager::GetInstance()->RegisterComponent<Collider2D>();
        ComponentManager::GetInstance()->RegisterComponent<Renderer>();

        auto graSystem = SystemManager::GetInstance()->RegisterSystem<GraphicsSystem>();
        auto colSystem = SystemManager::GetInstance()->RegisterSystem<CollisionSystem>();
        SystemManager::GetInstance()->RegisterSystem<GOFactory>();
        auto souSystem = SystemManager::GetInstance()->RegisterSystem<SoundSystem>();

        //SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });
        //OR can put it in init
        graSystem->Init();
        colSystem->Init(); // Set the signature
        souSystem->Init(false);

        //Entity player = EntityManager::GetInstance()->CreateEntity();
        Transform playerTrans{ 50, 10, 1, 10, 10};
        Transform testTrans{ 10, 10, 1, 10, 10};

        Collider2D playerCollider{ 1, 2, 3, 4 };
        Collider2D testCollider{ 1, 2, 3, 4 };
        Renderer toRender{};


        GameObject newObj = gGOFactory->CreateGO();
        colSystem->PrintEntities();
        newObj.AddComponent<Transform>(playerTrans);
        colSystem->PrintEntities();
        newObj.AddComponent<Collider2D>(playerCollider);
        colSystem->PrintEntities();
        newObj.AddComponent<Renderer>(toRender);

        /*GameObject newObj2 = gGOFactory->CreateGO();;
        colSystem->PrintEntities();
        newObj2.AddComponent<Transform>(playerTrans2);
        colSystem->PrintEntities();
        newObj2.AddComponent<Renderer>(toRender);*/


        GameObject testObj = gGoFactory->CreateGO();
        colSystem->PrintEntities();
        testObj.AddComponent<Transform>(testTrans);
        colSystem->PrintEntities();
        testObj.AddComponent<Collider2D>(testCollider);
        colSystem->PrintEntities();

        // Start timer
        //CarmicahTimer::StartTime();


        while (!glfwWindowShouldClose(window)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();

            glfwPollEvents();



            //testObj.GetComponent<Transform>().xPos += 1;
            ////testObj.GetComponent<Transform>().yPos += 1;
            //std::cout << "newObj AABB :" << newObj.GetComponent<Collider2D>().minX << " " << newObj.GetComponent<Collider2D>().maxX 
            //    << " " << newObj.GetComponent<Collider2D>().minY << " " << newObj.GetComponent<Collider2D>().maxY << std::endl;
            //std::cout << "testObj AABB :" << testObj.GetComponent<Collider2D>().minX << " " << testObj.GetComponent<Collider2D>().maxX
            //    << " " << testObj.GetComponent<Collider2D>().minY << " " << testObj.GetComponent<Collider2D>().maxY << std::endl;

            //std::cout << "newObj Pos : " << newObj.GetComponent<Transform>().xPos << " " << newObj.GetComponent<Transform>().yPos << std::endl;
            //std::cout << "testObj Pos : " << testObj.GetComponent<Transform>().xPos << " " << testObj.GetComponent<Transform>().yPos << std::endl;
            
            colSystem->Update();

            souSystem->Update();
            graSystem->Render(window);
        }

        souSystem->Exit();
        graSystem->Exit();

        glfwTerminate();

 

        //Carmicah::Log::init();

        //Carmicah::Log::getCoreLogger()->info("Core Logger Initialized");
        //Carmicah::Log::getCoreLogger()->warn("Core Logger Initialized");
        //Carmicah::Log::getCoreLogger()->error("Core Logger Initialized");
        //Carmicah::Log::getCoreLogger()->critical("Core Logger Initialized");
        //Carmicah::Log::getClientLogger()->trace("Client Logger Initialized");
        //          
        //Carmicah::Log::getClientLogger()->info("Client Logger Initialized");
        //Carmicah::Log::getClientLogger()->warn("Client Logger Initialized");
        //Carmicah::Log::getClientLogger()->error("Client Logger Initialized");
        //Carmicah::Log::getClientLogger()->critical("Client Logger Initialized");
        //Carmicah::Log::getClientLogger()->trace("Client Logger Initialized");

        return 0;
    }

}
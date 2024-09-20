#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/GameObject.h"
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
    const char* sceneName{"../Assets/Scene/Scene1.json"};

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
        auto souSystem = SystemManager::GetInstance()->RegisterSystem<SoundSystem>();

        //SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });
        //OR can put it in init
        graSystem->Init();
        colSystem->Init(); // Set the signature
        souSystem->Init(true);

        //Entity player = EntityManager::GetInstance()->CreateEntity();
        Transform playerTrans{ 0.5f, 0.5f, 1.f, 45.f, 1.f, 1.f};
        Transform playerTrans2{ -1.0f, 0.f, 1.f, -125.f, 1.f, 1.f};
        Collider2D playerCollider{ 1, 2, 3, 4 };
        Renderer toRender{};

        GameObject newObj;
        colSystem->PrintEntities();
        newObj.AddComponent<Transform>(playerTrans);
        colSystem->PrintEntities();
        newObj.AddComponent<Collider2D>(playerCollider);
        colSystem->PrintEntities();
        newObj.AddComponent<Renderer>(toRender);

        GameObject newObj2;
        colSystem->PrintEntities();
        newObj2.AddComponent<Transform>(playerTrans2);
        colSystem->PrintEntities();
        newObj2.AddComponent<Renderer>(toRender);


        // Start timer
        //CarmicahTimer::StartTime();


        while (!glfwWindowShouldClose(window)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();

            glfwPollEvents();

            //newObj.GetComponent<Transform>().xPos += 1;
            colSystem->Update();

            souSystem->Update();
            graSystem->Render(window);
        }

        souSystem->Exit();
        graSystem->Exit();

        glfwTerminate();

        return 0;
    }
    void Application::Import()
    {
        std::ifstream ifs{ sceneName, std::ios::binary };
        if (ifs)
        {
            std::string data;
            ifs.seekg(0, std::ios::end);
            data.resize(ifs.tellg());
            ifs.seekg(0, std::ios::beg);
            ifs.read(&data[0], data.size());
            ifs.close();

            rapidjson::Document document;
            document.Parse(data.c_str());

            static const char* kTypeNames[] =
            { "Null", "False", "True", "Object", "Array", "String", "Number" };

            for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin();
                it != document.MemberEnd(); ++it)
            {
                std::cout << "Member:" << it->name.GetString() << " is a:" << kTypeNames[it->value.GetType()] << "\n";
            }
        }

    }
    void Application::Export()
    {
        std::ofstream ofs{ sceneName, std::ios::binary };
        if (ofs)
        {
            rapidjson::Document document;
           
            document.SetObject();
            rapidjson::Value o(GameObject);
            



            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            document.Accept(writer);

            ofs.close();
        }
    }
}
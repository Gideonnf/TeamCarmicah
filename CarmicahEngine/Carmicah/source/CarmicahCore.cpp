#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>
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
        auto inputSystem = SystemManager::GetInstance()->RegisterSystem<InputSystem>();
        SystemManager::GetInstance()->RegisterSystem<GOFactory>();
        auto souSystem = SystemManager::GetInstance()->RegisterSystem<SoundSystem>();

        //SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });
        //OR can put it in init
        graSystem->Init();
        colSystem->Init(); // Set the signature
        souSystem->Init(false);
        inputSystem->BindSystem(gGOFactory);
        inputSystem->Init();

        Import();
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

            //newObj.GetComponent<Transform>().xPos += 1;
            colSystem->Update();

            souSystem->Update();
            graSystem->Render(window);
        }

        souSystem->Exit();
        graSystem->Exit();

        Export();

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

    void Application::Import()
    {
        std::ifstream ifs{ sceneName, std::ios::binary };
        if (ifs)
        {
            rapidjson::IStreamWrapper iws(ifs);
            rapidjson::Document document;
            document.ParseStream(iws);
            ifs.close();
            
            assert(document.IsArray());
            for (rapidjson::SizeType i{}; i < document.Size(); ++i)
            {
                const rapidjson::Value& go = document[i];
                std::string name = std::string(go["GameObject"].GetString());
                int id = go["ID"].GetInt();
                if (id == i)
                {
                    GameObject newObj = gGOFactory->CreateGO();
                    const rapidjson::Value& componentList = go["Components"];
                    for (rapidjson::Value::ConstValueIterator it = componentList.Begin(); it != componentList.End(); ++it)
                    {
                        const std::string& componentName = (*it)["Component Name"].GetString();
                        if (componentName == "struct Carmicah::Transform")
                        {
                            Transform t;
                            t.xPos      = (*it)["xPos"].GetDouble();
                            t.yPos      = (*it)["yPos"].GetDouble();
                            t.zPos      = (*it)["zPos"].GetDouble();
                            t.rot       = (*it)["rot"].GetDouble();
                            t.xScale    = (*it)["xScale"].GetDouble();
                            t.yScale    = (*it)["yScale"].GetDouble();
                            newObj.AddComponent<Transform>(t);
                        }
                        else if (componentName == "struct Carmicah::Collider2D")
                        {
                            Collider2D t;
                            t.minX = (*it)["minX"].GetDouble();
                            t.minY = (*it)["minY"].GetDouble();
                            t.maxX = (*it)["maxX"].GetDouble();
                            t.maxY = (*it)["maxY"].GetDouble();
                            newObj.AddComponent<Collider2D>(t);

                        }
                        else if (componentName == "struct Carmicah::Renderer")
                        {
                            Renderer t;
                            t.primitiveType = static_cast<Renderer::PRIMITIVE>((*it)["primitiveType"].GetInt());
                            newObj.AddComponent<Renderer>(t);
                        }
                    }
                }
            }
        }

    }
    void Application::Export()
    {
        std::ofstream ofs{ sceneName, std::ios::binary };
        if (ofs)
        {
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            
            writer.StartArray();
            gGOFactory->ForAllGO([&](GameObject& o) {
                writer.StartObject();

                writer.String("GameObject");
                writer.String(o.GetName().c_str(), static_cast<rapidjson::SizeType>(o.GetName().length()));

                writer.String("ID");
                writer.Int(o.GetID());

                writer.String("Components");
                writer.StartArray();
                ComponentManager::GetInstance()->ForEachComponent([&](const std::string componentName)
                {
                    writer.StartObject();
                    writer.String("Component Name");
                    writer.String(componentName.c_str(), static_cast<rapidjson::SizeType>(componentName.length()));
                    if (componentName == "struct Carmicah::Transform")
                    {
                        Transform& t = o.GetComponent<Transform>();
                        writer.String("xPos");
                        writer.Double(t.xPos);
                        writer.String("yPos");
                        writer.Double(t.yPos);
                        writer.String("zPos");
                        writer.Double(t.zPos);
                        writer.String("rot");
                        writer.Double(t.rot);
                        writer.String("xScale");
                        writer.Double(t.xScale);
                        writer.String("yScale");
                        writer.Double(t.yScale);
                        
                    }
                    else if (componentName == "struct Carmicah::Collider2D")
                    {
                        Collider2D& t = o.GetComponent<Collider2D>();
                        writer.String("minX");
                        writer.Double(t.minX);
                        writer.String("minY");
                        writer.Double(t.minY);
                        writer.String("maxX");
                        writer.Double(t.maxX);
                        writer.String("maxY");
                        writer.Double(t.maxY);
                    }
                    else if (componentName == "struct Carmicah::Renderer")
                    {
                        Renderer& t = o.GetComponent<Renderer>();
                        writer.String("primitiveType");
                        writer.Int(t.primitiveType);
                    }

                    writer.EndObject();

                }, EntityManager::GetInstance()->GetSignature(o.GetID()));
                writer.EndArray();

                writer.EndObject();
            });
            writer.EndArray();
            ofs.close();
        }
    }
}
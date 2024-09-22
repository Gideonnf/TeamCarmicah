#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <log.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>   
#include <ImGUI/imgui_impl_opengl3.h>
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
        const char* glsl_version = "#version 460";
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
        Transform playerTrans{ 0.5f, 0.5f, 1.f, 45.f, 1.f, 1.f};
        Transform playerTrans2{ -1.0f, 0.f, 1.f, -125.f, 1.f, 1.f};
        Collider2D playerCollider{ 1, 2, 3, 4 };
        Renderer toRender{};


        GameObject newObj = gGOFactory->CreateGO();
        colSystem->PrintEntities();
        newObj.AddComponent<Transform>(playerTrans);
        colSystem->PrintEntities();
        newObj.AddComponent<Collider2D>(playerCollider);
        colSystem->PrintEntities();
        newObj.AddComponent<Renderer>(toRender);

        GameObject newObj2 = gGOFactory->CreateGO();;
        colSystem->PrintEntities();
        newObj2.AddComponent<Transform>(playerTrans2);
        colSystem->PrintEntities();
        newObj2.AddComponent<Renderer>(toRender);


        // Start timer
        //CarmicahTimer::StartTime();


        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


        while (!glfwWindowShouldClose(window)) {

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            if (show_demo_window)
            {
                ImGui::ShowDemoWindow(&show_demo_window);
            }

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
          
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();

            glfwPollEvents();

            //newObj.GetComponent<Transform>().xPos += 1;
            //colSystem->Update();

            //souSystem->Update();
            //graSystem->Render(window);

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        souSystem->Exit();
        graSystem->Exit();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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
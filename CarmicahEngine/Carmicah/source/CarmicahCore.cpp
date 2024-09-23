#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <../log.h>
#include <queue>
#include <chrono>
#include <string>

#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/GameObject.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Systems/CollisionSystem.h"
#include "../FPSCounter.h"

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
        CM_INFO("Client Logger Initialized");

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carmicah", NULL, NULL);
        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, key_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            CM_CORE_ERROR("Failed to initialize GLAD");
            return -1;
        }

        ComponentManager::GetInstance()->RegisterComponent<Transform>();
        ComponentManager::GetInstance()->RegisterComponent<Collider2D>();

        auto colSystem = SystemManager::GetInstance()->RegisterSystem<CollisionSystem>();
        colSystem->Init();

        auto fpsCounter = std::make_unique<FPSCounter>();
        fpsCounter->Init();

        GameObject newObj;
        Transform playerTrans{ 1, 1, 1 };
        Collider2D playerCollider{ 1, 2, 3, 4 };
        newObj.AddComponent<Transform>(playerTrans);
        newObj.AddComponent<Collider2D>(playerCollider);

#ifndef NO_SOUND
        FMOD::System* mpSystem;
        if (FMOD::System_Create(&mpSystem) != FMOD_OK)
            return 0;
        mpSystem->init(32, FMOD_INIT_NORMAL, NULL);
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = NULL;
        if (mpSystem->createSound("../Assets/bouken.mp3", FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
            return 0;
        sound->setMode(FMOD_LOOP_OFF);
        mpSystem->playSound(sound, NULL, false, &channel);
#endif

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

#ifndef NO_SOUND
            mpSystem->update();
#endif

            colSystem->Update();
            fpsCounter->Update();

            glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Update window title with current FPS
            std::string title = "Carmicah - FPS: " + std::to_string(static_cast<int>(fpsCounter->GetFPS()));
            glfwSetWindowTitle(window, title.c_str());

            // Here you would draw your game objects

            glfwSwapBuffers(window);
        }

#ifndef NO_SOUND
        sound->release();
        if (mpSystem != NULL)
            mpSystem->release();
#endif

        fpsCounter->Exit();
        colSystem->Exit();

        glfwTerminate();
        return 0;
    }
}
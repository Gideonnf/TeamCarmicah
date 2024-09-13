#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>

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
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carmicah", NULL, NULL);
        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, key_callback);

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

        int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        printf("GL %d.%d\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

#ifndef NO_SOUND
            mpSystem->update();
#endif

            glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
        }

#ifndef NO_SOUND
        sound->release();
        if (mpSystem != NULL)
            mpSystem->release();
#endif

        glfwTerminate();

        return 0;
    }
}
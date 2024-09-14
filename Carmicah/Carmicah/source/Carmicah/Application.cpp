#include "pch.h"
#include "Application.h"
#include <stdio.h>

#include "ComponentManager.h"
#include "SystemManager.h"
#include "GameObject.h"
#include "CollisionSystem.h"

struct Transform
{
    float xPos;
    float yPos;
    float zPos;
};

struct AABB
{
    float minX;
    float minY;
    float maxX;
    float maxY;
};

struct Collider2D
{
    AABB colliderBox;
};

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

        //int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        //printf("GL %d.%d\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR);

        // Register the components
        ComponentManager::GetInstance()->RegisterComponent<Transform>();
        ComponentManager::GetInstance()->RegisterComponent<Collider2D>();

        auto colSystem = SystemManager::GetInstance()->RegisterSystem<CollisionSystem>();
        SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });

        std::cout << colSystem->mSignature << std::endl;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window);
        }

        glfwTerminate();

        return 0;
	}
}
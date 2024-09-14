#include "pch.h"
#include "Application.h"
#include <stdio.h>

#include "ComponentManager.h"
#include "SystemManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider2D.h"
#include "CollisionSystem.h"





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

        //OR can put it in init
        //SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });
        colSystem->Init(); // Set the signature

        //Entity player = EntityManager::GetInstance()->CreateEntity();
        Transform playerTrans{ 1, 1, 1 };
        Collider2D playerCollider{ 1, 2, 3, 4 };
        //ComponentManager::GetInstance()->AddComponent<Transform>(player, playerTrans);
        //auto entitySignature = EntityManager::GetInstance()->GetSignature(player);
        //// Set the component's signature pos within entity signature to true
        //entitySignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>(), true);
        //// Set the new siganture of hte entity to show that it has this component now
        //EntityManager::GetInstance()->SetSignature(player, entitySignature);
        //// Update the systems that the signature changed
        //SystemManager::GetInstance()->UpdateSignatures(player, entitySignature);

        GameObject newObj;
        colSystem->PrintEntities();
        newObj.AddComponent<Transform>(playerTrans);
        colSystem->PrintEntities();
        newObj.AddComponent<Collider2D>(playerCollider);
        colSystem->PrintEntities();

      
       
        std::cout << colSystem->mSignature << std::endl;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            colSystem->Update();

            glfwSwapBuffers(window);
        }

        glfwTerminate();

        return 0;
	}
}
#include "pch.h"
#include "CarmicahCore.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include <spdlog/spdlog.h>
#include <../log.h>

#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/GameObject.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Systems/CollisionSystem.h"
#include "CarmicahTime.h"
#include "../include/Systems/Events.h" // idk why this aint working
#include "Systems/Input.h"  // this too wtf


bool on_key_pressed(u16 code, void* sender, void* listener, EventData eventdata)
{
    std::cout << "Key Pressed" << eventdata.data.i16[0] << std::endl;

    //std::cout << "Key Pressed" << input_keycode_to_string(eventdata.data.i16[0]) << std::endl;
    //printf("Key Pressed %s \n", input_keycode_to_string(eventdata.data.i16[0]));
    // idk why this isnt working, it should work ^


    return true;
}

bool on_mouse_button_down(u16 code, void* sender, void* listener, EventData eventdata)
{
    // check
    if (eventdata.data.u16[0] == MOUSE_BUTTON_LEFT)
    {
        std::cout << "Mouse Left Button Down" << eventdata.data.i16[0] << std::endl;
    }
    if (eventdata.data.u16[0] == MOUSE_BUTTON_RIGHT)
    {
        std::cout << "Mouse Right Button Down" << eventdata.data.i16[0] << std::endl;
    }
    return true;
}

bool on_mouse_moved(u16 code, void* sender, void* listener, EventData eventdata)
{
    std::cout   << "Mouse Moved! X: " << eventdata.data.i16[0] 
                << "Y: " << eventdata.data.i16[1] << std::endl;
    return true;
}

namespace Carmicah
{

    // IDK WHERE TO RUN IT FROM SO IM GONNA JUST PLUNK MY FUNCTIONS HERE AND HOPE IT WORK
    // it didnt work because i cant even include the header file properly


    event_system_initialise(); // init event system

    input_system_initialise();  // init input system

    // test, create an event where it listens for key is pressed
    event_subscribe(EVENT_CODE_KEY_PRESSED, 0, on_key_pressed);         // by the end i should only need this to test
    // test, create an event where it listens for mouse is moved
    event_subscribe(EVENT_CODE_MOUSE_MOVED, (void*) 1, on_mouse_moved); // by the end i should only need this to test
    // test, create an event where it listens for mouse button is pressed
    event_subscribe(EVENT_CODE_MOUSE_BUTTON_PRESSED, 0, on_key_pressed);  // *

    // create an event to forcefully check if key is pressed
    EventData event_data;               // *
    event_data.eventdata.i16[0] = 13;   // *

    // publish the event that represents the fake event above
    event_publish(EVENT_CODE_KEY_PRESSED, 0, event_data); // *

    // create second event to forcefully check that mouse position of x and y can be returned, not real value atp
    EventData event_data2;
    event_data2.data.i16[0] = 125; // *
    event_data2.data.i16[1] = 350; // *


    // close event system
    void event_system_shutdown();

    // close input system
    void input_system_shutdown();


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
        ComponentManager::GetInstance()->RegisterComponent<Transform>();
        ComponentManager::GetInstance()->RegisterComponent<Collider2D>();

        auto colSystem = SystemManager::GetInstance()->RegisterSystem<CollisionSystem>();

        //SystemManager::GetInstance()->SetSignature<CollisionSystem>({ "Transform", "Collider2D" });
        //OR can put it in init
        colSystem->Init(); // Set the signature

        //Entity player = EntityManager::GetInstance()->CreateEntity();
        Transform playerTrans{ 1, 1, 1 };
        Collider2D playerCollider{ 1, 2, 3, 4 };

        GameObject newObj;
        colSystem->PrintEntities();
        newObj.AddComponent<Transform>(playerTrans);
        colSystem->PrintEntities();
        newObj.AddComponent<Collider2D>(playerCollider);
        colSystem->PrintEntities();

        // Start timer
        //CarmicahTimer::StartTime();


        while (!glfwWindowShouldClose(window)) {
            // Update dt calc
            CarmicahTimer::UpdateElapsedTime();

            std::cout << "dt in CmCore: " << CarmicahTimer::GetDeltaTime() << std::endl;
            glfwPollEvents();

#ifndef NO_SOUND
            mpSystem->update();
#endif

            newObj.GetComponent<Transform>().xPos += 1;
            std::cout << "xPos : " << newObj.GetComponent<Transform>().xPos << std::endl;
            colSystem->Update();
            std::cout << "yPos : " << newObj.GetComponent<Transform>().yPos << std::endl;


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

 

        Carmicah::log::init();
        Carmicah::log::getCoreLogger()->info("Core Logger Initialized");
        Carmicah::log::getCoreLogger()->warn("Core Logger Initialized");
        Carmicah::log::getCoreLogger()->error("Core Logger Initialized");
        Carmicah::log::getCoreLogger()->critical("Core Logger Initialized");
        Carmicah::log::getClientLogger()->trace("Client Logger Initialized");

        Carmicah::log::getClientLogger()->info("Client Logger Initialized");
        Carmicah::log::getClientLogger()->warn("Client Logger Initialized");
        Carmicah::log::getClientLogger()->error("Client Logger Initialized");
        Carmicah::log::getClientLogger()->critical("Client Logger Initialized");
        Carmicah::log::getClientLogger()->trace("Client Logger Initialized");

        return 0;
    }



}
#include "pch.h"
#include "Systems/GameLogic.h"
#include "Input/InputSystem.h"
#include "ECS/SystemManager.h"
#include "Systems/GOFactory.h"
#include "Systems/SoundSystem.h"
#include "Components/TextRenderer.h"
#include "Components/UITransform.h"
#include "CarmicahTime.h"
#include "Systems/AssetManager.h"


namespace Carmicah
{
    bool pKeyWasPressed = false;
    bool moveKeyWasPressed = false;
    bool tKeyWasPressed = false;
    bool debugPhysics = false; // This will toggle the physics debug mode

	void GameLogic::Init()
	{
        soundSystemRef = SystemManager::GetInstance()->GetSystem<SoundSystem>();
        physicsRef = SystemManager::GetInstance()->GetSystem<PhysicsSystem>();
        physicsRef->mDebugPhysics = false;
        physicsRef->mToggleUpdate = false;

        mainCharacter = gGOFactory->FetchGO("mainCharacter");

        wall = gGOFactory->FetchGO("wall");

        FPSText = gGOFactory->FetchGO("FPSText");
        FPSText.GetComponent<UITransform>().yPos = AssetManager::GetInstance()->enConfig.Height;
    }

	void GameLogic::Update(GLFWwindow* window)
	{
        if (Input.IsKeyPressed(Keys::KEY_SPACEBAR))
        {
            SystemManager::GetInstance()->ChangeScene("Scene2");
        }

        if (Input.IsKeyPressed(Keys::KEY_1))
        {
            GameObject duckObj = gGOFactory->FetchGO("Duck");
            duckObj.Destroy();
        }

        if (Input.IsKeyPressed(Keys::KEY_B))
        {
            soundSystemRef->PauseResumeSound(soundSystemRef->defaultBGM);
           // SystemManager::GetInstance()->GetSystem<SoundSystem>()->PauseResumeSound
        }

        // TODO: Input key press is broken atm. Pressed will only work for the very first function call cause its a global map
        // I need to make an event listener 
        /*
        // Trigger physics debug
        if (Input.IsKeyPressed(Keys::KEY_P))
        {
            physicsRef->mDebugPhysics = true;
        }

        // Anytime a movement key is pressed or T key 
        if (Input.IsKeyPressed(Keys::KEY_W) || Input.IsKeyPressed(Keys::KEY_A) || Input.IsKeyPressed(Keys::KEY_S) || Input.IsKeyPressed(Keys::KEY_D) || Input.IsKeyPressed(Keys::KEY_T))
        {
            physicsRef->mToggleUpdate = true;
        }

        if (Input.IsKeyPressed(Keys::KEY_D))
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = 5.0f;
        }
        else if (Input.IsKeyPressed(Keys::KEY_A))
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = -5.0f;
        }
        else if (Input.IsKeyPressed(Keys::KEY_W))
        {
            mainCharacter.GetComponent<RigidBody>().velocity.y = 5.0f;
        }
        else if (Input.IsKeyPressed(Keys::KEY_S))
        {
            mainCharacter.GetComponent<RigidBody>().velocity.y = -5.0f;
        }
        else
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = 0.0f;
            mainCharacter.GetComponent<RigidBody>().velocity.y = 0.0f;
        }
       */
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pKeyWasPressed) {
            physicsRef->mDebugPhysics = !physicsRef->mDebugPhysics;
            pKeyWasPressed = true;  // Mark the key as pressed
        }
        else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            pKeyWasPressed = false;  // Reset the key press flag when the P key is released
        }

        if (physicsRef->mDebugPhysics) {
            // Handle WASD movement during debugPhysics mode
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !tKeyWasPressed)
            {
                physicsRef->mToggleUpdate = true;
                tKeyWasPressed = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
            {
                tKeyWasPressed = false;
            }

            // Check if a movement key was pressed
            bool movementKeyPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

            if (movementKeyPressed && !moveKeyWasPressed) {
                physicsRef->mToggleUpdate = true;
                moveKeyWasPressed = true;  // Mark movement key as pressed
            }
            else if (!movementKeyPressed) {
                moveKeyWasPressed = false;  // Reset when no key is pressed
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = 5.0f;
        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = -5.0f;
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            mainCharacter.GetComponent<RigidBody>().velocity.y = 5.0f;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            mainCharacter.GetComponent<RigidBody>().velocity.y = -5.0f;
        }
        else
        {
            mainCharacter.GetComponent<RigidBody>().velocity.x = 0.0f;
            mainCharacter.GetComponent<RigidBody>().velocity.y = 0.0f;
        }


        
        FPSText.GetComponent<TextRenderer>().txt = "FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS()));
        //if (Input.IsKeyPressed(Keys::KEY_SPACEBAR))
        //{
        //    sceneSys->ChangeScene(scene2Name);
        //}

        //if (Input.IsKeyPressed(Keys::KEY_1))
        //{
        //    GameObject duckObj = gGOFactory->FetchGO("Duck");
        //    duckObj.Destroy();
        //}

        //if (Input.IsKeyPressed(Keys::KEY_P))
        //{
        //    souSystem->PauseResumeSound(souSystem->defaultBGM);
        //}
	}
}
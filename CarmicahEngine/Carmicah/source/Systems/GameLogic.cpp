/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        GameLogic.cpp

 author:   Gideon Francis(70%)
 co-author: Lee Yong Yee (30%)

 emails:       g.francis@digipen.edu, l.yongyee@digipen.edu

 brief:       A system to handle game logic coding. Controling of players and any game play related examples to show in engine

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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

        gGOFactory->FetchGO("mainCharacter", mainCharacter);

        //wall = gGOFactory->FetchGO("wall");

        gGOFactory->FetchGO("FPSText", FPSText);
        //FPSText.GetComponent<UITransform>().yPos = (float)AssetManager::GetInstance()->enConfig.Height;
    }

	void GameLogic::Update(GLFWwindow* window)
	{
        UNUSED(window);
        if (Input.IsKeyPressed(Keys::KEY_Z))
        {
            SystemManager::GetInstance()->ChangeScene("Scene1");
        }
        if (Input.IsKeyPressed(Keys::KEY_X))
        {
            SystemManager::GetInstance()->ChangeScene("Scene2");
        }
        if (Input.IsKeyPressed(Keys::KEY_C) && mainCharacter.IsActive())
        {
            mainCharacter.GetComponent<Transform>().xScale += 2.0f * (float)CarmicahTimer::GetDt();
            mainCharacter.GetComponent<Transform>().yScale += 2.0f * (float)CarmicahTimer::GetDt();

            //SystemManager::GetInstance()->ChangeScene("Scene2");
        }
        if (Input.IsKeyPressed(Keys::KEY_B))
        {
            soundSystemRef->PauseResumeSound(soundSystemRef->defaultBGM);
          //  soundSystemRef->PauseResumeSound(soundSystemRef->buttonBGM);

           // SystemManager::GetInstance()->GetSystem<SoundSystem>()->PauseResumeSound
        }
        if (Input.IsKeyPressed(Keys::KEY_G))
        {
            soundSystemRef->PauseResumeSound(soundSystemRef->buttonBGM);
            // SystemManager::GetInstance()->GetSystem<SoundSystem>()->PauseResumeSound
        }

        if (Input.IsKeyPressed(Keys::KEY_P))
        {
            physicsRef->mDebugPhysics = !physicsRef->mDebugPhysics;
        }

        // Anytime a movement key is pressed or T key 
        if (Input.IsKeyPressed(Keys::KEY_W) || Input.IsKeyPressed(Keys::KEY_A) || Input.IsKeyPressed(Keys::KEY_S) || Input.IsKeyPressed(Keys::KEY_D) || Input.IsKeyPressed(Keys::KEY_T))
        {
            physicsRef->mToggleUpdate = true;
        }

        if (mainCharacter.IsActive())
        {
            if (physicsRef->mDebugPhysics ? Input.IsKeyPressed(Keys::KEY_D) : Input.IsKeyHold(Keys::KEY_D))
            {
                mainCharacter.GetComponent<RigidBody>().velocity.x = 5.0f;
            }
            else if (physicsRef->mDebugPhysics ? Input.IsKeyPressed(Keys::KEY_A) : Input.IsKeyHold(Keys::KEY_A))
            {
                mainCharacter.GetComponent<RigidBody>().velocity.x = -5.0f;
            }
            else if (physicsRef->mDebugPhysics ? Input.IsKeyPressed(Keys::KEY_W) : Input.IsKeyHold(Keys::KEY_W))
            {
                mainCharacter.GetComponent<RigidBody>().velocity.y = 5.0f;
            }
            else if (physicsRef->mDebugPhysics ? Input.IsKeyPressed(Keys::KEY_S) : Input.IsKeyHold(Keys::KEY_S))
            {
                mainCharacter.GetComponent<RigidBody>().velocity.y = -5.0f;
            }
            else
            {
                mainCharacter.GetComponent<RigidBody>().velocity.x = 0.0f;
                mainCharacter.GetComponent<RigidBody>().velocity.y = 0.0f;
            }

        }

        FPSText.GetComponent<TextRenderer>().txt = "FPS: " + std::to_string(static_cast<int>(CarmicahTimer::GetFPS()));
	}

    void GameLogic::EntityDestroyed(Entity id)
    {
        UNUSED(id);
        //if (id == mainCharacter.GetID())
        //{
        //    mainCharacter.SetActive(false);
        //}
    }

    void GameLogic::ReceiveMessage(Message* msg)
    {
        if (msg->mMsgType == MSG_ENTITYKILLED)
        {
            Entity entityKilled = dynamic_cast<EntityKilledMessage*>(msg)->mEntityID;

            if (entityKilled == mainCharacter.GetID())
            {
                mainCharacter.SetActive(false);
            }
        }
    }
}
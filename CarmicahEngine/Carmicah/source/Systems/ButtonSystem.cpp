/* file documentation -----------------------------------------------------------------------------
\file       ButtonSystem.cpp
\author     Micah Lim (100%)
\course     CSD 2400
\date       071124

\brief      Implementation of the ButtonSystem class, which manages button components in the game
			engine, including detecting button presses and releases.

\functions  - ButtonSystem::Init: Initializes the button system and sets up entity signatures.
			- ButtonSystem::Update: Updates button states based on input detection.
			- ButtonSystem::Exit: Clears registered entities and releases resources.
			- ButtonSystem::OnPress: Handles logic for button press events.
			- ButtonSystem::OnRelease: Handles logic for button release events.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <ECS/ECSTypes.h>
#include <ECS/GameObject.h>
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/RigidBody.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"
#include "../Input/InputSystem.h"
#include "ButtonSystem.h"
#include "SoundSystem.h"
#include <algorithm>

namespace Carmicah
{
	/* function documentation--------------------------------------------------------------------------
	\brief      Initializes the ButtonSystem, setting up necessary entity signatures for button
			components.

	\param      [in, out] Init
			Initializes the button system components and registers necessary signatures.

	\return     void
	-------------------------------------------------------------------------------------------------*/
	void ButtonSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Button>());

		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ButtonSystem>(mSignature);

		//buttons.clear(); // clear any existing button registrations
		std::cout << "Button System Initialized" << std::endl; // test
	}


	/* function documentation--------------------------------------------------------------------------
	\brief      Updates the ButtonSystem, detecting button states and processing input events.

	\param      [in, out] Update
				Checks for button press and release events and updates button states accordingly.

	\return     void
	-------------------------------------------------------------------------------------------------*/
	void ButtonSystem::Update()
	{
		//auto* componentManager = ComponentManager::GetInstance();

		//for (const auto& entity : mEntitiesSet)
		//{
		//	// retrieve the Button component for the entity
		//	auto& button = componentManager->GetComponent<Button>(entity);
		//	auto& buttonRenderer = componentManager->GetComponent<Renderer>(entity);
		//	if (Input.IsKeyHold(Keys::KEY_B))
		//	{
		//		buttonRenderer.texture = button.ButtonImagePressed;
		//	}
		//	else
		//	{
		//		buttonRenderer.texture = button.ButtonImage;
		//	}

		//	// check if mouse is over button	
		//	//bool isMouseOverButton = Input.IsMouseOver(button.GetPosition(), button.GetSize());
		//	//bool isMousePressed = Input.IsMousePressed(MOUSE_BUTTON_LEFT);
		//	//bool isMouseReleased = Input.IsMouseReleased(MOUSE_BUTTON_LEFT);

		//	//if (isMouseOverButton && isMousePressed && !button.isPressed)
		//	//{
		//	//	OnPress(button.ButtonOG);  // pass the button name or identifier
		//	//}
		//	//else if (isMouseOverButton && isMouseReleased && button.isPressed)
		//	//{
		//	//	OnRelease(button.ButtonOG);
		//	//}
		//}
	}


	/* function documentation--------------------------------------------------------------------------
	\brief      Cleans up and clears the registered entities in the ButtonSystem.

	\param      [in, out] Exit
				Releases resources and clears entities registered within the button system.

	\return     void
	-------------------------------------------------------------------------------------------------*/
	void ButtonSystem::Exit()
	{
		mEntitiesSet.clear(); // clear all buttons
	}



	void ButtonSystem::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_BUTTONENTITY)
		{
			auto castedMsg = dynamic_cast<ButtonClicked*>(msg);
			for (auto it : mEntitiesSet)
			{
				if (it == castedMsg->mEntityID)
				{
					auto& button = ComponentManager::GetInstance()->GetComponent<Button>(castedMsg->mEntityID);
					auto& buttonRenderer = ComponentManager::GetInstance()->GetComponent<Renderer>(castedMsg->mEntityID);
					if (button.isPressed)
					{
						buttonRenderer.Texture(button.ButtonImagePressed);
						OnPress(button, castedMsg->mEntityID);
					}
					else
					{
						buttonRenderer.Texture(button.ButtonImage);
						OnRelease(button);
					}

					button.isPressed = !button.isPressed;
				}
			}
		}
	}



	/* function documentation--------------------------------------------------------------------------
	\brief      Handles logic for when a specific button is pressed.

	\param      [in] name
				The name of the button that was pressed.

	\return     void
	-------------------------------------------------------------------------------------------------*/
	void ButtonSystem::OnPress(Button& buttonComponent, Entity entityID)
	{
		UNUSED(buttonComponent);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySound("buttonclick", SoundCategory::UI, 0.5f);

		OnClickMsg newMsg(entityID);
		SendSysMessage(&newMsg);

		//auto* componentManager = ComponentManager::GetInstance();

		//for (const auto& entity : mEntitiesSet)
		//{
		//	auto& button = componentManager->GetComponent<Button>(entity);
		//	if (button.ButtonOG == name)
		//	{
		//		button.isPressed = true;
		//		std::cout << "button pressed" << std::endl;
		//		//button.SetTexture(button.ButtonPress); // need to create the SetTexture function
		//		////std::cout << "Button " << name << " pressed" << std::endl;
		//	}
		//}
	}


	/* function documentation--------------------------------------------------------------------------
	\brief      Handles logic for when a specific button is released.

	\param      [in] name
				The name of the button that was pressed.

	\return     void
	-------------------------------------------------------------------------------------------------*/
	void ButtonSystem::OnRelease(Button& buttonComponent)
	{
		UNUSED(buttonComponent);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySound("pop", SoundCategory::UI, 0.5f);

		//auto* componentManager = ComponentManager::GetInstance();

		//for (const auto& entity : mEntitiesSet)
		//{
		//	auto& button = componentManager->GetComponent<Button>(entity);
		//	if (button.ButtonOG == name)
		//	{
		//		button.isPressed = false;
		//		std::cout << "button released" << std::endl;
		//		//button.SetTexture(button.ButtonOG); // need to create the SetTexture function
		//		//std::cout << "Button " << name << " released" << std::endl;
		//	}
		//}
	}

}

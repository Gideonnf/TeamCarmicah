
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
						buttonRenderer.texture = button.ButtonImagePressed;
						OnPress(button);
					}
					else
					{
						buttonRenderer.texture = button.ButtonImage;
						OnRelease(button);
					}

					button.isPressed = !button.isPressed;
				}
			}
		}
	}

	// OnPress and OnRelease takes in the name of the current button being pressed 
	// and then handles the logic for when the button is pressed
	void ButtonSystem::OnPress(Button& buttonComponent)
	{
		UNUSED(buttonComponent);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySound("buttonclick", SoundCategory::UI, 0.5f);
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

	// call this function when button is released
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


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
		auto* componentManager = ComponentManager::GetInstance();

		for (const auto& entity : mEntitiesSet)
		{
			// retrieve the Button component for the entity
			auto& button = componentManager->GetComponent<Button>(entity);

			// check if mouse is over button	
			bool isMouseOverButton = Input.IsMouseOver(button.GetPosition(), button.GetSize());
			bool isMousePressed = Input.IsMousePressed(MOUSE_BUTTON_LEFT);
			bool isMouseReleased = Input.IsMouseReleased(MOUSE_BUTTON_LEFT);

			if (isMouseOverButton && isMousePressed && !button.isPressed)
			{
				OnPress(button.ButtonOG);  // pass the button name or identifier
			}
			else if (isMouseOverButton && isMouseReleased && button.isPressed)
			{
				OnRelease(button.ButtonOG);
			}
		}

		//// loop through container of buttons 
		//for (auto& [name, buttonAny] : buttons)
		//{
		//	// cast stored button to actual Button type
		//	Button* button = std::any_cast<Button>(&buttonAny);
		//	std::cout << "heLOOOOOOOOOO" << std::endl;
		//	if (button)
		//	{
		//		// check if mouse is over button	
		//		bool isMouseOverButton = Input.IsMouseOver(button->GetPosition(), button->GetSize());
		//		std::cout << isMouseOverButton << std::endl;
		//		bool isMousePressed = Input.IsMousePressed(MOUSE_BUTTON_LEFT);
		//		bool isMouseReleased = Input.IsMouseReleased(MOUSE_BUTTON_LEFT);
		//		// check if mouse is over button and if the button is pressed
		//		if (isMouseOverButton && isMousePressed && !button->isPressed)
		//		{
		//			OnPress(name);
		//		}
		//		// check if mouse is over button and if the button is released
		//		else if (isMouseOverButton && isMouseReleased && button->isPressed)
		//		{
		//			OnRelease(name);
		//		}
		//	}
		//}
	}


	void ButtonSystem::Exit()
	{
		mEntitiesSet.clear(); // clear all buttons
	}

	// OnPress and OnRelease takes in the name of the current button being pressed 
	// and then handles the logic for when the button is pressed
	void ButtonSystem::OnPress(std::string name)
	{
		auto* componentManager = ComponentManager::GetInstance();

		for (const auto& entity : mEntitiesSet)
		{
			auto& button = componentManager->GetComponent<Button>(entity);
			if (button.ButtonOG == name)
			{
				button.isPressed = true;
				std::cout << "button pressed" << std::endl;
				//button.SetTexture(button.ButtonPress); // need to create the SetTexture function
				//std::cout << "Button " << name << " pressed" << std::endl;
			}
		}
	}

	// call this function when button is released
	void ButtonSystem::OnRelease(std::string name)
	{
		auto* componentManager = ComponentManager::GetInstance();

		for (const auto& entity : mEntitiesSet)
		{
			auto& button = componentManager->GetComponent<Button>(entity);
			if (button.ButtonOG == name)
			{
				button.isPressed = false;
				std::cout << "button released" << std::endl;
				//button.SetTexture(button.ButtonOG); // need to create the SetTexture function
				//std::cout << "Button " << name << " released" << std::endl;
			}
		}
	}

}

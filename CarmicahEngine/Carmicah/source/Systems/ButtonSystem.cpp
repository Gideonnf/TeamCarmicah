
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

		buttons.clear(); // clear any existing button registrations
		std::cout << "Button System Initialized" << std::endl; // test
	}


	void ButtonSystem::Update()
	{
		// loop through container of buttons 
		for (auto& [name, buttonAny] : buttons)
		{
			// cast stored button to actual Button type
			Button* button = std::any_cast<Button>(&buttonAny);
			std::cout << "heLOOOOOOOOOO" << std::endl;
			if (button)
			{
				// check if mouse is over button	
				bool isMouseOverButton = Input.IsMouseOver(button->GetPosition(), button->GetSize());
				std::cout << isMouseOverButton << std::endl;
				bool isMousePressed = Input.IsMousePressed(MOUSE_BUTTON_LEFT);
				bool isMouseReleased = Input.IsMouseReleased(MOUSE_BUTTON_LEFT);

				// check if mouse is over button and if the button is pressed
				if (isMouseOverButton && isMousePressed && !button->isPressed)
				{
					OnPress(name);
				}
				// check if mouse is over button and if the button is released
				else if (isMouseOverButton && isMouseReleased && button->isPressed)
				{
					OnRelease(name);
				}
			}
		}
	}


	void ButtonSystem::Exit()
	{
		buttons.clear(); // clear all buttons
	}

	// OnPress and OnRelease takes in the name of the current button being pressed 
	// and then handles the logic for when the button is pressed
	void ButtonSystem::OnPress(std::string name)
	{
		Button* button = std::any_cast<Button>(&buttons[name]);
		button->isPressed = true; // set to pressed
		std::cout << "Button is pressed" << std::endl; // test


		// logic to switch texture here
		//Renderer::texture = ButtonPress;
	}

	// call this function when button is released
	void ButtonSystem::OnRelease(std::string name)
	{
		Button* button = std::any_cast<Button>(&buttons[name]);
		button->isPressed = false; // set to released
		std::cout << "Button is released" << std::endl; // test

		// revert to og texture here
		//Renderer::texture = ButtonOG;
	}

	void ButtonSystem::AddButton(std::string name, std::any button)
	{
		buttons[name] = button; // add a new button to the container of the button components
	}

	void ButtonSystem::RemoveButton(std::string name)
	{
		buttons.erase(name); // remove a button from the container of the button components
	}
}

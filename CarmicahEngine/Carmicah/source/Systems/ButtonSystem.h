/* file documentation -----------------------------------------------------------------------------
\file       ButtonSystem.h
\author     Micah Lim (100%)
\course     CSD 2400
\date       071124

\brief      Header file for the ButtonSystem class, which manages the logic for button components
			in the game engine, including press and release detection and state management.

\functions  - Init: Initializes the button system, setting up necessary components and systems.
			- Update: Updates the button states based on input events.
			- Exit: Cleans up and clears button resources when no longer needed.
			- OnPress: Processes actions when a button is pressed.
			- OnRelease: Processes actions when a button is released.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/


#ifndef BUTTON_SYSTEM_H
#define BUTTON_SYSTEM_H

#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include "ECS/BaseSystem.h"
#include "../Components/Button.h"
#include <string>
#include <unordered_map>
#include <any>

namespace Carmicah
{
	class ButtonSystem : public BaseSystem
	{
	public:
		// running the button system
		void Init();
		void Update();
		void Exit();

		void ReceiveMessage(Message* msg) override;

		// button functions
		void OnPress(Button& buttonComponent, Entity entityID);
		void OnRelease(Button&);

		#pragma region Potential Button Functions
		//void OnHover(std::string name);
		//void OnUnhover(std::string name);
		//void OnClick(std::string name);
		//void OnDoubleClick(std::string name);
		//void OnLongPress(std::string name);
		//void OnDrag(std::string name);
		//void OnDrop(std::string name);
		//void OnScroll(std::string name);
		//void OnRightClick(std::string name);
		//void OnMiddleClick(std::string name);
		//void OnRightDoubleClick(std::string name);
		//void OnMiddleDoubleClick(std::string name);
		//void OnRightLongPress(std::string name);
		//void OnMiddleLongPress(std::string name);
		//void OnRightDrag(std::string name);
		//void OnMiddleDrag(std::string name);
		//void OnRightDrop(std::string name);
		//void OnMiddleDrop(std::string name);
		//void OnRightScroll(std::string name);
		//void OnMiddleScroll(std::string name);
		//void OnRightHover(std::string name);
		//void OnMiddleHover(std::string name);
		//void OnRightUnhover(std::string name);
		//void OnMiddleUnhover(std::string name);
		//void OnRightPress(std::string name);
		//void OnMiddlePress(std::string name);
		//void OnRightRelease(std::string name);
		//void OnMiddleRelease(std::string name);
		//void OnRightClick(std::string name);
		//void OnMiddleClick(std::string name);
		//void OnRightDoubleClick(std::string name);
		//void OnMiddleDoubleClick(std::string name);
		//void OnRightLongPress(std::string name);
		//void OnMiddleLongPress(std::string name);
		//void OnRightDrag(std::string name);
		//void OnMiddleDrag(std::string name);
		//void OnRightDrop(std::string name);
		//void OnMiddleDrop(std::string name);
		//void OnRightScroll(std::string name);
		//void OnMiddleScroll(std::string name);
		//void OnRightHover(std::string name);
		//void OnMiddleHover(std::string name);
		//void OnRightUnhover(std::string name);
		#pragma endregion
	};
}

#endif
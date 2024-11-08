/* file documentation -----------------------------------------------------------------------------
\file		InputSystem.hpp
\author     Micah Lim, (micahshengyao.lim)
\course     CSD 2400
\date		240924

\brief      This file declares the InputSystem class, which manages the input handling for keyboard
			and mouse events in a game engine. It includes functionality for tracking key and mouse
			states, handling drag events, and converting keycodes to string representations.

\functions  - InputSystem::Init: Initializes the input system with a reference to the GLFW window.
			- InputSystem::Update: Updates the input system's state.
			- InputSystem::UpdatePrevInput: Updates the previous key state for tracking.
			- InputSystem::IsKeyPressed/IsKeyReleased/IsKeyHold: Checks the current key states.
			- InputSystem::WasKeyPressed/WasKeyReleased/WasKeyHold: Placeholder methods for tracking previous key states.
			- InputSystem::IsMousePressed/IsMouseReleased/IsMouseHold: Checks the current mouse button states.
			- InputSystem::GetMousePosition/GetMouseX/GetMouseY: Retrieves the current mouse position.
			- InputSystem::SetMousePosition: Sets the mouse position.
			- InputSystem::UpdateKeyMap/UpdateMouseMap: Updates the key and mouse button state maps.
			- InputSystem::IsDragging/SetDragging: Manages drag state.
			- InputSystem::GetDragStartPos/GetDragEndPos/GetDragCurrentPos: Retrieves the drag positions.
			- InputSystem::SetDragStartPos/SetDragEndPos/SetDragCurrentPos: Sets the drag positions.
			- InputSystem::KeycodeToString: Converts keycodes to string names.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "GLFW/glfw3.h"
#include <array>
#include "Input/InputTypes.h"
#include "Singleton.h"
#include "Math/Vec2.h"

namespace Carmicah
{
	class InputSystem : public BaseSystem
	{
	private:
		GLFWwindow* windowRef;
		std::unordered_map<int, KeyStates> mKeyMap;
		std::unordered_map<int, KeyStates> mMouseMap;

		// Keep track of keyPressed since its meant to be a one time return
		//std::unordered_map<int, bool> mKeyPressedMap;
		//// Keep track if key was used in an event
		//std::unordered_map<int, bool> mKeyPreviousMap;


		Vector2D<double> mMousePos;
		bool mMousePressed;
		float mMouseTick;

		// dragging state and position tracking variables
		bool isDragging;
		Vector2D<double> dragStartPos;
		Vector2D<double> dragEndPos;
		Vector2D<double> dragCurrentPos;
		

	public:
		InputSystem() : windowRef(nullptr), mMousePressed(false), mMouseTick(0) {}
		~InputSystem() {};

		// Not using the inherited singleton template class cause we want the base system inheritance
		static InputSystem* GetInstance() {
			static InputSystem instance;
			return &instance;
		}
		
		/* function documentation--------------------------------------------------------------------------

		\brief      Initializes the input system with a reference to the GLFW window

					This function is responsible for initializing the input system by setting up the GLFW window
					reference. It ensures that the system is prepared to handle keyboard and mouse input events.
					Pre-condition: A valid GLFWwindow reference must be provided.
					Post-condition: The input system is ready to track input events.

		\param      [in] ref
					Reference to the GLFW window.

		\return     void

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		void Init(GLFWwindow* ref);

		/* function documentation--------------------------------------------------------------------------

		\brief      Updates the input system's state

					This function updates the current state of the input system. It checks for new input events
					and processes them, updating internal maps to reflect the current status of keys and mouse buttons.
					Pre-condition: The input system must be initialized.
					Post-condition: The input system's key and mouse maps are updated.

		\param      void

		\return     void

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		void Update();

		/* function documentation--------------------------------------------------------------------------

		\brief      Updates the previous key state for tracking

					This function stores the current key states into the previous state maps, allowing the system
					to track changes between input states across frames.
					Pre-condition: The input system must be initialized.
					Post-condition: Previous input states are updated for comparison in the next frame.

		\param      void

		\return     void

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		void UpdatePrevInput();

		/* function documentation--------------------------------------------------------------------------

		\brief      Checks if a key is currently pressed

					This function checks if the specified key is currently pressed down. It only returns true once
					when the key is initially pressed.
					Pre-condition: The input system must be initialized.
					Post-condition: Returns whether the key is pressed.

		\param      [in] key
					The key to check the press status of.

		\return     bool - True if the key is pressed, false otherwise.

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		bool IsKeyPressed(Keys key);

		/* function documentation--------------------------------------------------------------------------

		\brief      Checks if a key is currently released

					This function checks if the specified key is currently released. It returns true when the key
					has been let go.
					Pre-condition: The input system must be initialized.
					Post-condition: Returns whether the key is released.

		\param      [in] key
					The key to check the release status of.

		\return     bool - True if the key is released, false otherwise.

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		bool IsKeyReleased(Keys key);

		/* function documentation--------------------------------------------------------------------------

		\brief      Checks if a key is being held down

					This function checks if the specified key is being held down continuously.
					Pre-condition: The input system must be initialized.
					Post-condition: Returns whether the key is held down.

		\param      [in] key
					The key to check the hold status of.

		\return     bool - True if the key is held down, false otherwise.

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		bool IsKeyHold(Keys key);

		bool WasKeyPressed(Keys key);
		bool WasKeyReleased(Keys key);
		bool WasKeyHold(Keys key);

		bool IsMousePressed(MouseButtons button);
		bool IsMouseReleased(MouseButtons button);
		bool IsMouseHold(MouseButtons button);
		bool IsMouseOver(Vec2d& position, Vec2d& size);


		/* function documentation--------------------------------------------------------------------------

		\brief      Retrieves the current mouse position

					This function retrieves the current position of the mouse cursor within the window.
					Pre-condition: The input system must be initialized.
					Post-condition: Returns the current mouse position.

		\param      void

		\return     Vector2D<double> - The current mouse position.

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		Vector2D<double> GetMousePosition();
		double GetMouseX();
		double GetMouseY();

		void SetMousePosition(double xPos, double yPos);

		void UpdateKeyMap(int key, KeyStates state);
		void UpdateMouseMap(int key, KeyStates state);

		// new methods for drag tracking
		bool IsDragging() const;
		Vector2D<double> GetDragStartPos() const;
		Vector2D<double> GetDragEndPos() const;
		Vector2D<double> GetDragCurrentPos() const;

		// setter methods for dragging state and position tracking
		void SetDragging(bool dragging);
		void SetDragStartPos(const Vector2D<double>& pos);
		void SetDragEndPos(const Vector2D<double>& pos);
		void SetDragCurrentPos(const Vector2D<double>& pos);


		/* function documentation--------------------------------------------------------------------------

		\brief      Converts keycodes to string representations

					This function takes a keycode and returns the corresponding string name for that key.
					Pre-condition: A valid key must be provided.
					Post-condition: Returns the string representation of the key.

		\param      [in] key
					The keycode to be converted.

		\return     const char* - The string representation of the keycode.

		\exceptions No exceptions thrown.
		-------------------------------------------------------------------------------------------------*/
		const char* KeycodeToString(Keys key);

		void ProxySend(Message*);
	};

	// Just so people can call "Input.IsKeyPressed" 
	static InputSystem& Input = *InputSystem::GetInstance();
}
#endif

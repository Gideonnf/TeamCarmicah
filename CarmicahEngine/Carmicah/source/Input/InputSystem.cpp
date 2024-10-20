/* file documentation -----------------------------------------------------------------------------
\file		InputSystem.cpp
\author     Micah Lim, (micahshengyao.lim)
\course     CSD 2400
\date		240924

\brief      This file implements the input system for handling keyboard and mouse input in a game engine.
			It includes methods for tracking key and mouse states, detecting key presses/releases,
			and managing drag events with the mouse. Additionally, it integrates callbacks for input
			handling using the GLFW library.

\functions  - KeyCallback: Handles keyboard input, including key press and release detection.
			- MouseCallback: Handles mouse button input, including drag events.
			- CursorPosCallback: Tracks the mouse cursor's position and manages dragging logic.
			- ScrollCallback: Handles mouse scroll input (currently unused).
			- InputSystem::Init: Initializes the input system with a reference to the GLFW window.
			- InputSystem::Update: Updates the input system's state.
			- InputSystem::UpdatePrevInput: Updates the previous key state for tracking.
			- InputSystem::IsKeyPressed/IsKeyReleased/IsKeyHold: Checks the current key states.
			- InputSystem::IsMousePressed/IsMouseReleased/IsMouseHold: Checks the current mouse button states.
			- InputSystem::SetMousePosition/GetMousePosition: Sets or retrieves the current mouse position.
			- InputSystem::SetDragging/IsDragging: Manages drag state and tracking of start, current, and end positions.
			- InputSystem::UpdateKeyMap: Updates the key state map.
			- InputSystem::UpdateMouseMap: Updates the mouse button state map.
			- InputSystem::KeycodeToString: Converts keycodes to string names.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/


#include "pch.h"
#include "Input/InputSystem.h"
#include "Messaging/InputMessage.h"
#include "log.h"
#include "Math/Vec2.h"
#include "CarmicahTime.h"


namespace Carmicah
{
	std::array<bool, (int)GLFW_KEY_LAST> mKeyCurrentState{};
	std::array<bool, (int)GLFW_KEY_LAST> mKeyPreviousState{};

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		UNUSED(scancode);
		UNUSED(mods);
		UNUSED(window);
		Input.UpdateKeyMap(key, (KeyStates)action);

		// close window if Esc is pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// cout whatever key that was pressed
		if (action == GLFW_PRESS)
		{
			// use KeycodeToString function to print the key name
			const char* keyName = Input.KeycodeToString((Keys)key);
			std::cout << "Key Pressed: " << keyName << std::endl;

		}
	}

	void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		UNUSED(mods);
		UNUSED(window);
		Input.UpdateMouseMap(button, (KeyStates)action);

		// start dragging if mouse button is pressed, left button only as of now
		if (action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_LEFT /*|| button == GLFW_MOUSE_BUTTON_RIGHT*/))
		{
			// mouse is now dragging
			Input.SetDragging(true);

			// get and set where it started dragging from
			Input.SetDragStartPos(Input.GetMousePosition());
		}
		// stop dragging when button is released
		else if (action == GLFW_RELEASE && Input.IsDragging() 
				/*&& (Input.GetMousePosition().x != Input.GetDragCurrentPos().x || Input.GetMousePosition().y != Input.GetDragCurrentPos().y)*/)
		{
			// set bool to false and get dragEndPos
			Input.SetDragging(false);
			Input.SetDragEndPos(Input.GetMousePosition());

			// see where drag started and ended
			std::cout << "Drag started and ended at: (" << Input.GetDragStartPos().x << ", " << Input.GetDragStartPos().y << ") to ("
				<< Input.GetDragEndPos().x << ", " << Input.GetDragEndPos().y << ")" << std::endl;
		}
	}

	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		UNUSED(window);
		Input.SetMousePosition(xPos, yPos);
		//std::cout << "Mouse current pos: " << xPos << ", " << yPos << std::endl;

		// if mouse is dragging
		if (Input.IsDragging())
		{
			// get current pos of cursor
			Input.SetDragCurrentPos( {xPos, yPos} );

			// cout the current mouse pos
			std::cout << "Dragging to: (" << xPos << ", " << yPos << ")" << std::endl;
		}
	}

	// Scroll back stuff
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		UNUSED(window);
		UNUSED(xOffset);
		UNUSED(yOffset);
	}

	void InputSystem::Init(GLFWwindow* ref)
	{
		windowRef = ref;

		// example of sending messages
		int keyCode = 5;
		KeyMessage msg(keyCode);
		SendMessage(&msg);

		// Set up the call backs
		glfwSetKeyCallback(windowRef, KeyCallback);
		glfwSetMouseButtonCallback(windowRef, MouseCallback);
		glfwSetCursorPosCallback(windowRef, CursorPosCallback);
		glfwSetScrollCallback(windowRef, ScrollCallback);
		//GLFW_MOUSE_BUTTON_LEFT;
	}

	void InputSystem::Update()
	{
		if (windowRef == nullptr)
		{
			CM_CORE_ERROR("Error: Input system not initalized.");
		}
		
	}

	void InputSystem::UpdatePrevInput()
	{
		mKeyPreviousState = mKeyCurrentState;
	}

	// key press should only return true once until its released and pressed again
	// so a map is used to keep track of that
	bool InputSystem::IsKeyPressed(Keys key)
	{
		return mKeyCurrentState[(int)key] && !mKeyPreviousState[(int)key];
	}

	bool InputSystem::IsKeyReleased(Keys key)
	{
		return !mKeyCurrentState[(int)key] && mKeyPreviousState[(int)key];
	}

	bool InputSystem::IsKeyHold(Keys key)
	{
		return mKeyCurrentState[(int)key];
	}

	bool InputSystem::WasKeyPressed(Keys key)
	{
		UNUSED(key);
		// do later
		return false;
	}

	bool  InputSystem::WasKeyReleased(Keys key)
	{
		UNUSED(key);
		// do later
		return false;
	}

	bool  InputSystem::WasKeyHold(Keys key)
	{
		UNUSED(key);
		// do later
		return false;
	}

	bool InputSystem::IsMousePressed(MouseButtons button)
	{
		return KeyStates::PRESSED == mMouseMap[(int)button];
	}

	bool InputSystem::IsMouseReleased(MouseButtons button)
	{
		return KeyStates::RELEASE == mMouseMap[(int)button];
	}

	bool InputSystem::IsMouseHold(MouseButtons button)
	{
		return KeyStates::HOLD == mMouseMap[(int)button];
	}

	Vector2D<double> InputSystem::GetMousePosition()
	{
		return mMousePos;
	}

	double InputSystem::GetMouseX()
	{
		return mMousePos.x;
	}

	double InputSystem::GetMouseY()
	{
		return mMousePos.y;
	}

	void InputSystem::SetMousePosition(double xPos, double yPos)
	{
		mMousePos.x = xPos;
		mMousePos.y = yPos;
	}

	// new methods for drag tracking
	bool InputSystem::IsDragging() const
	{
		return isDragging;
	}

	Vector2D<double> InputSystem::GetDragStartPos() const
	{
		return dragStartPos;
	}

	Vector2D<double> InputSystem::GetDragEndPos() const
	{
		return dragEndPos;
	}

	Vector2D<double> InputSystem::GetDragCurrentPos() const
	{
		return dragCurrentPos;
	}

	// setter methods for drag tracking
	void InputSystem::SetDragging(bool dragging)
	{
		isDragging = dragging;
	}

	void InputSystem::SetDragStartPos(const Vector2D<double>& pos)
	{
		dragStartPos = pos;
	}

	void InputSystem::SetDragEndPos(const Vector2D<double>& pos)
	{
		dragEndPos = pos;
	}

	void InputSystem::SetDragCurrentPos(const Vector2D<double>& pos)
	{
		dragCurrentPos = pos;
	}

	void InputSystem::UpdateKeyMap(int key, KeyStates state)
	{
		mKeyMap[key] = state;

		if (state == KeyStates::PRESSED)
		{
			mKeyCurrentState[(int)key] = true;
		}
		else if (state == KeyStates::RELEASE)
		{
			mKeyCurrentState[(int)key] = false;
		}
	}

	void InputSystem::UpdateMouseMap(int key, KeyStates state)
	{
		mMouseMap[key] = state;
		if (state == KeyStates::PRESSED)
		{
			mMousePressed = true;
			mMouseTick = 0.0f;
		}
		else
		{
			mMousePressed = false;
			mMouseTick = 0.0f;
		}
		//std::cout << "in update mouse map" << mMousePressed << std::endl;
		//std::cout << "Mouse State : " << state << " For : " << key << std::endl;
	}

	// Convert keycode to string (if-version)
	const char* InputSystem::KeycodeToString(Keys key)
	{
		if (key == KEY_BACKSPACE)
		{
			return "backspace";
		}
		if (key == KEY_TAB)
		{
			return "tab";
		}
		if (key == KEY_ENTER)
		{
			return "enter";
		}
		if (key == KEY_CTRL)
		{
			return "ctrl";
		}
		if (key == KEY_CAPSLOCK)
		{
			return "capslock";
		}
		if (key == KEY_ESC)
		{
			return "esc";
		}
		if (key == KEY_SPACEBAR)
		{
			return "spacebar";
		}
		if (key == KEY_PLUS)
		{
			return "plus";
		}
		if (key == KEY_COMMA)
		{
			return "comma";
		}
		if (key == KEY_DASH)
		{
			return "dash";
		}
		if (key == KEY_FULLSTOP)
		{
			return "fullstop";
		}
		if (key == KEY_QUESTIONMARK)
		{
			return "questionmark";
		}
		if (key == KEY_CURLYDASH)
		{
			return "curlydash";
		}
		if (key == KEY_CURLYBRACKET_OPEN)
		{
			return "curlybracket_open";
		}
		if (key == KEY_BACKDASH)
		{
			return "backdash";
		}
		if (key == KEY_CURLYBRACKET_CLOSE)
		{
			return "curlybracket_close";
		}
		if (key == KEY_APOSTROPHE)
		{
			return "apostrophe";
		}
		if (key == KEY_WINDOWS_KEY)
		{
			return "windows_key";
		}

		// Arrow Keys
		if (key == KEY_ARROW_LEFT)
		{
			return "arrow_left";
		}
		if (key == KEY_ARROW_UP)
		{
			return "arrow_up";
		}
		if (key == KEY_ARROW_RIGHT)
		{
			return "arrow_right";
		}
		if (key == KEY_ARROW_DOWN)
		{
			return "arrow_down";
		}

		// Volume Keys
		if (key == KEY_VOLUME_MUTE)
		{
			return "volume_mute";
		}
		if (key == KEY_VOLUME_DOWN)
		{
			return "volume_down";
		}
		if (key == KEY_VOLUME_UP)
		{
			return "volume_up";
		}

		// Track Keys
		if (key == KEY_TRACK_NEXT)
		{
			return "track_next";
		}
		if (key == KEY_TRACK_PREVIOUS)
		{
			return "track_previous";
		}
		if (key == KEY_TRACK_STOP)
		{
			return "track_stop";
		}
		if (key == KEY_TRACK_PLAYORPAUSE)
		{
			return "track_playorpause";
		}

		// Number Keys
		if (key == KEY_0)
		{
			return "0";
		}
		if (key == KEY_1)
		{
			return "1";
		}
		if (key == KEY_2)
		{
			return "2";
		}
		if (key == KEY_3)
		{
			return "3";
		}
		if (key == KEY_4)
		{
			return "4";
		}
		if (key == KEY_5)
		{
			return "5";
		}
		if (key == KEY_6)
		{
			return "6";
		}
		if (key == KEY_7)
		{
			return "7";
		}
		if (key == KEY_8)
		{
			return "8";
		}
		if (key == KEY_9)
		{
			return "9";
		}

		// Alphabet Keys
		if (key == KEY_A)
		{
			return "A";
		}
		if (key == KEY_B)
		{
			return "B";
		}
		if (key == KEY_C)
		{
			return "C";
		}
		if (key == KEY_D)
		{
			return "D";
		}
		if (key == KEY_E)
		{
			return "E";
		}
		if (key == KEY_F)
		{
			return "F";
		}
		if (key == KEY_G)
		{
			return "G";
		}
		if (key == KEY_H)
		{
			return "H";
		}
		if (key == KEY_I)
		{
			return "I";
		}
		if (key == KEY_J)
		{
			return "J";
		}
		if (key == KEY_K)
		{
			return "K";
		}
		if (key == KEY_L)
		{
			return "L";
		}
		if (key == KEY_M)
		{
			return "M";
		}
		if (key == KEY_N)
		{
			return "N";
		}
		if (key == KEY_O)
		{
			return "O";
		}
		if (key == KEY_P)
		{
			return "P";
		}
		if (key == KEY_Q)
		{
			return "Q";
		}
		if (key == KEY_R)
		{
			return "R";
		}
		if (key == KEY_S)
		{
			return "S";
		}
		if (key == KEY_T)
		{
			return "T";
		}
		if (key == KEY_U)
		{
			return "U";
		}
		if (key == KEY_V)
		{
			return "V";
		}
		if (key == KEY_W)
		{
			return "W";
		}
		if (key == KEY_X)
		{
			return "X";
		}
		if (key == KEY_Y)
		{
			return "Y";
		}
		if (key == KEY_Z)
		{
			return "Z";
		}

		// Function Keys
		if (key == KEY_F01)
		{
			return "F1";
		}
		if (key == KEY_F02)
		{
			return "F2";
		}
		if (key == KEY_F03)
		{
			return "F3";
		}
		if (key == KEY_F04)
		{
			return "F4";
		}
		if (key == KEY_F05)
		{
			return "F5";
		}
		if (key == KEY_F06)
		{
			return "F6";
		}
		if (key == KEY_F07)
		{
			return "F7";
		}
		if (key == KEY_F08)
		{
			return "F8";
		}
		if (key == KEY_F09)
		{
			return "F9";
		}
		if (key == KEY_F10)
		{
			return "F10";
		}
		if (key == KEY_F11)
		{
			return "F11";
		}
		if (key == KEY_F12)
		{
			return "F12";
		}

		return "undefined";
	}

	// convert keycode to string (switch version)
	/*// Convert keycode to string
	const char* InputSystem::KeycodeToString(Keys key)
	{
		switch (key)
		{
		// Misc Keys
		case KEY_BACKSPACE:
			return "backspace";
		case KEY_TAB:
			return "tab";
		case KEY_ENTER:
			return "enter";
		case KEY_CTRL:
			return "ctrl";
		case KEY_CAPSLOCK:
			return "capslock";
		case KEY_ESC:
			return "esc";
		case KEY_SPACEBAR:
			return "spacebar";
		case KEY_PLUS:
			return "plus";
		case KEY_COMMA:
			return "comma";
		case KEY_DASH:
			return "dash";
		case KEY_FULLSTOP:
			return "fullstop";
		case KEY_QUESTIONMARK:
			return "questionmark";
		case KEY_CURLYDASH:
			return "curlydash";
		case KEY_CURLYBRACKET_OPEN:
			return "curlybracket_open";
		case KEY_BACKDASH:
			return "backdash";
		case KEY_CURLYBRACKET_CLOSE:
			return "curlybracket_close";
		case KEY_APOSTROPHE:
			return "apostrophe";
		case KEY_WINDOWS_KEY:
			return "windows_key";

		// Arrow Keys
		case KEY_ARROW_LEFT:
			return "arrow_left";
		case KEY_ARROW_UP:
			return "arrow_up";
		case KEY_ARROW_RIGHT:
			return "arrow_right";
		case KEY_ARROW_DOWN:
			return "arrow_down";

		// Volume Keys
		case KEY_VOLUME_MUTE:
			return "volume_mute";
		case KEY_VOLUME_DOWN:
			return "volume_down";
		case KEY_VOLUME_UP:
			return "volume_up";

		// Track Keys
		case KEY_TRACK_NEXT:
			return "track_next";
		case KEY_TRACK_PREVIOUS:
			return "track_previous";
		case KEY_TRACK_STOP:
			return "track_stop";
		case KEY_TRACK_PLAYORPAUSE:
			return "track_playorpause";

		// Number Keys
		case KEY_0:
			return "0";
		case KEY_1:
			return "1";
		case KEY_2:
			return "2";
		case KEY_3:
			return "3";
		case KEY_4:
			return "4";
		case KEY_5:
			return "5";
		case KEY_6:
			return "6";
		case KEY_7:
			return "7";
		case KEY_8:
			return "8";
		case KEY_9:
			return "9";

		// Alphabet Keys
		case KEY_A:
			return "A";
		case KEY_B:
			return "B";
		case KEY_C:
			return "C";
		case KEY_D:
			return "D";
		case KEY_E:
			return "E";
		case KEY_F:
			return "F";
		case KEY_G:
			return "G";
		case KEY_H:
			return "H";
		case KEY_I:
			return "I";
		case KEY_J:
			return "J";
		case KEY_K:
			return "K";
		case KEY_L:
			return "L";
		case KEY_M:
			return "M";
		case KEY_N:
			return "N";
		case KEY_O:
			return "O";
		case KEY_P:
			return "P";
		case KEY_Q:
			return "Q";
		case KEY_R:
			return "R";
		case KEY_S:
			return "S";
		case KEY_T:
			return "T";
		case KEY_U:
			return "U";
		case KEY_V:
			return "V";
		case KEY_W:
			return "W";
		case KEY_X:
			return "X";
		case KEY_Y:
			return "Y";
		case KEY_Z:
			return "Z";

		// Function Keys
		case KEY_F01:
			return "F1";
		case KEY_F02:
			return "F2";
		case KEY_F03:
			return "F3";
		case KEY_F04:
			return "F4";
		case KEY_F05:
			return "F5";
		case KEY_F06:
			return "F6";
		case KEY_F07:
			return "F7";
		case KEY_F08:
			return "F8";
		case KEY_F09:
			return "F9";
		case KEY_F10:
			return "F10";
		case KEY_F11:
			return "F11";
		case KEY_F12:
			return "F12";

		default:
			return "undefined";
		}
	}*/


}
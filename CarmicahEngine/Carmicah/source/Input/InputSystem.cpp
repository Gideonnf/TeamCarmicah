/* File Documentation -----------------------------------------------------------------------------
file:           InputSystem.cpp

\author			Micah Lim (80%), Gideon Francis (20%)

email:          micahshengyao.lim@digipen.edu

brief:          This file implements the InputSystem class, managing input handling for keyboard
				and mouse events. It provides methods for tracking key presses, mouse clicks, drag
				events, and other input states. Integrates GLFW for input callbacks and utilizes
				functionality to synchronize input states for real-time applications.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/



#include "pch.h"
#include "Input/InputSystem.h"
#include "Messaging/InputMessage.h"
#include <chrono>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "Editor/SceneToImgui.h"
#include "log.h"
#include "Math/Vec2.h"
#include "CarmicahTime.h"
#include "ECS/SystemManager.h"
#include "Systems/AssetManager.h"
#include "Systems/SoundSystem.h"
#include "Systems/GOFactory.h"
#include "ECS/ComponentManager.h"
#include "Editor/SceneWindow.h"


namespace Carmicah
{
	#pragma region Callback Functions

	/* function documentation--------------------------------------------------------------------------
	\brief      Callback function to handle keyboard input events, such as key presses and releases.

				Handles the following events:
					In-Game:
						Alt Enter	- Toggles Full Screen
					Editor Mode:
						Escape		- Closes the Window
					Both:
						Alt Tab		- Iconify
						Task Manager- Iconify
	-------------------------------------------------------------------------------------------------*/
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		UNUSED(scancode);
		UNUSED(mods);

		if (key < 0)
		{
			std::stringstream ss;
			ss << "Invalid key code received: " << key << "\n";
			CM_WARN(ss.str());
			return;
		}


		// close window if Esc is pressed	
		if (action == GLFW_PRESS)
		{
			Input.UpdateKeyMap(key, PRESS);

			#ifndef CM_INSTALLER
			if (key == GLFW_KEY_ESCAPE)
			{
				//glfwSetWindowShouldClose(window, GL_TRUE);
			}
			#endif

			// CTRL-ALT-DEL
			bool ctrlPressed = Input.IsKeyDown(KEY_LEFT_CONTROL) || Input.IsKeyDown(KEY_RIGHT_CONTROL);
			bool altPressed = Input.IsKeyDown(KEY_LEFT_ALT) || Input.IsKeyDown(KEY_RIGHT_ALT);
			bool deletePressed = Input.IsKeyDown(KEY_DELETE);
			if (ctrlPressed && altPressed && deletePressed)
			{
				// If the key is pressed this frame
				if ((key == GLFW_KEY_DELETE || key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT || key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) &&
					!Input.mNotFullScreen)
				// minimise window
				glfwIconifyWindow(window);
			}

			// ALT-TAB
			bool tabPressed = Input.IsKeyDown(KEY_TAB);
			if (altPressed && tabPressed)
			{
				if ((key == GLFW_KEY_TAB || key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) &&
					!Input.mNotFullScreen)
					glfwIconifyWindow(window);
			}

#ifdef CM_INSTALLER
			bool enterPressed = Input.IsKeyDown(KEY_ENTER);
			if (altPressed && enterPressed &&
				(key == GLFW_KEY_ENTER || key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT))
			{
				Input.ToggleFullScreen();
			}
#endif

			// cout whatever key that was pressed
			//use KeycodeToString function to print the key name
			//{
			//	std::stringstream ss;
			//	const char* keyName = Input.KeycodeToString((Keys)key);
			//	ss << "Key Pressed: " << keyName << std::endl;
			//	CM_INFO(ss.str());
			//}
		}
		else if (action == GLFW_RELEASE)
		{
			Input.UpdateKeyMap(key, RELEASE);
		}
	}


	/* function documentation--------------------------------------------------------------------------
	\brief      Callback function to handle mouse button input events, such as press and release.
	-------------------------------------------------------------------------------------------------*/
	void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		UNUSED(mods);
		UNUSED(window);

		// update the current mouse state map
		if (action == GLFW_PRESS)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				Input.UpdateMouseMap(button, KeyStates::PRESS);
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				Input.UpdateMouseMap(button, KeyStates::PRESS);
				Vec2f a = Input.GetMouseWorldPosition();
				//CM_CORE_INFO("{}. {}", a.x, a.y);

			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				Input.UpdateMouseMap(button, KeyStates::RELEASE);
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				Input.UpdateMouseMap(button, KeyStates::RELEASE);
			}
		}

		// DRAG CHECK
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		{
#ifndef CM_INSTALLER
			if (SceneToImgui::GetInstance()->GetHovering() != SceneToImgui::NO_SCENE)
			{
#endif
				EntityPickedMessage msg(SceneToImgui::GetInstance()->GetIDObjPick());

				Input.ProxySend(&msg);
#ifndef CM_INSTALLER
			}
#endif
	}
		else if (action == GLFW_RELEASE) // stop dragging when button is released
		{
			//Vec2d mousePosD = Input.GetMousePosition();
			// TODO: Hard coded
			//if (mousePosD.x >= 0 && mousePosD.x <= 1920 && mousePosD.y >= 0 && mousePosD.x <= 1080)
			//{
			//	Vec2i mousePosI = { std::clamp(static_cast<int>(mousePosD.x), 0, 1920), 1080 - std::clamp(static_cast<int>(mousePosD.y) - 1, 0, 1080) };
			//}
		}
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Callback function to handle cursor input events, such as press and release.
	-------------------------------------------------------------------------------------------------*/
	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		UNUSED(window);
		// NOTE: SetMousePos is being used in SceneWindow for wrapping position of the cursor so that the mouse pos can pick accurately 
		// since IMGUI makes the whole window 1920 by 1080 but we have to treat the scene as 1920 by 1080
#ifndef CM_INSTALLER
		if (SceneToImgui::GetInstance()->GetHovering() == SceneToImgui::NO_SCENE)
		{
#endif
			Input.SetMousePosition(xPos, yPos);
#ifndef CM_INSTALLER
		}
#endif
	}

	/* function documentation--------------------------------------------------------------------------
	\brief	Callback Function to handle mouse scroll events.
	-------------------------------------------------------------------------------------------------*/
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		// notified when the user scrolls, whether with a mouse wheel or touchpad gesture, set a scroll callback
		UNUSED(window);
		UNUSED(xOffset);

		// adjust zoom level based on scroll direction
		if (yOffset > 0)
		{
			//std::cout << "scroll up (zoom in)" << std::endl;
			Input.SetScrollOffset(yOffset);
		}
		else if (yOffset < 0)
		{
			//std::cout << "scroll down (zoom out)" << std::endl;
			Input.SetScrollOffset(yOffset);
		}
		
		// or is this function for scrolling up and down the scene? 
	}

	/* function documentation-------------------------------------------------------------------------
	\brief 	Callback function to handle window iconify events.
	------------------------------------------------------------------------------------------------*/
	void WindowIconifyCallback(GLFWwindow* window, int iconified)
	{
		UNUSED(iconified);

		// check if window is iconified
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
		{
			// pause all audio
			auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
			souSystem->PauseAllSounds();

			#ifdef CM_INSTALLER
			SceneWindow::mIsPaused = true;
			#endif
		}
		else
		{
			// resume all audio
			auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
			souSystem->ResumeAllSounds();

			#ifdef CM_INSTALLER
			SceneWindow::mIsPaused = false;
			#endif
		}
	}

	/* function documentation-------------------------------------------------------------------------
	\brief 	Callback function to handle window focus events.
	------------------------------------------------------------------------------------------------*/
	void WindowFocusCallback(GLFWwindow* window, int focused)
	{
		UNUSED(window);
		UNUSED(focused);

		#ifdef CM_INSTALLER
		int isFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

		if (!isFocused && !Input.mNotFullScreen)
		{

			glfwIconifyWindow(window);
		}
		#endif
	}

	#pragma endregion


	#pragma region Init & Update
	
	/* function documentation--------------------------------------------------------------------------
	\brief      Initializes the InputSystem for a specific GLFW window.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::Init(GLFWwindow* ref)
	{
		windowRef = ref;

		// example of sending messages
		int keyCode = 5;
		KeyMessage msg(keyCode);
		SendSysMessage(&msg);

		glfwSetInputMode(ref, GLFW_STICKY_KEYS, GLFW_TRUE);

		// Set up the call backs
		glfwSetKeyCallback			 (windowRef, KeyCallback);
		glfwSetMouseButtonCallback	 (windowRef, MouseCallback);
		glfwSetScrollCallback		 (windowRef, ScrollCallback);
		glfwSetCursorPosCallback	 (windowRef, CursorPosCallback);
		//glfwSetWindowFocusCallback	 (windowRef, WindowFocusCallback);
		glfwSetWindowIconifyCallback (windowRef, WindowIconifyCallback);
	}

	void InputSystem::CloseGame()
	{
		glfwSetWindowShouldClose(windowRef, GL_TRUE);
	}

	void InputSystem::ToggleFullScreen()
	{
		if (!Input.mNotFullScreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

			glfwSetWindowMonitor(windowRef, primaryMonitor, 0, 0,
				AssetManager::GetInstance()->enConfig.Width, AssetManager::GetInstance()->enConfig.Height, 0);
			glfwSetInputMode(windowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

			glfwSetWindowMonitor(windowRef, primaryMonitor, 0, 25,
				Input.mWindowScale.x, Input.mWindowScale.y, 0);

			glfwSetWindowMonitor(windowRef, nullptr, 0, 25,
				AssetManager::GetInstance()->enConfig.Width, AssetManager::GetInstance()->enConfig.Height, 0);
			glfwSetInputMode(windowRef, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		Input.mNotFullScreen = !Input.mNotFullScreen;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Updates the state of the input system.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::Update()
	{
		if (windowRef == nullptr)
		{
			CM_CORE_ERROR("Error: Input system not initalized.");
		}

		// handle transitions automatically
		for (auto& mouseState : mMouseMap)
		{
			if (mouseState.second == KeyStates::PRESSED)
			{
				mouseState.second = KeyStates::HOLD;
			}
		}
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Updates the previous key state for tracking purposes.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::UpdatePrevInput()
	{
		decltype(mChangedKeyQueue) mTempQueue;
		while(!mChangedKeyQueue.empty())
		{
			auto i = mChangedKeyQueue.front();
			mChangedKeyQueue.pop();
			KeyStates newKeyState = NONE;
			switch (std::get<2>(i))
			{
				case PRESS:
					newKeyState = PRESSED;
				break;
				case PRESSED:
					newKeyState = HOLD;
				break;
				case RELEASE:
					newKeyState = RELEASED;
				break;
				case RELEASED:
					newKeyState = NONE;
				break;
			}

			if (std::get<0>(i))
			{
				mKeyMap[std::get<1>(i)] = newKeyState;
			}
			else
			{
				mMouseMap[std::get<1>(i)] = newKeyState;
			}
			if (newKeyState == PRESSED || newKeyState == RELEASED)
			{
				mTempQueue.push(std::make_tuple(std::get<0>(i), std::get<1>(i), newKeyState));
			}
		}
		mChangedKeyQueue = mTempQueue;
		mScrollAmt = 0.f;
	}

	#pragma endregion


	#pragma region Key & Mouse State Methods

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key is currently pressed.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsKeyPressed(Keys key)
	{
		// key press should only return true once until its released and pressed again
		// so a map is used to keep track of that
		return mKeyMap[key] == PRESS;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key is currently released.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsKeyReleased(Keys key)
	{
		return mKeyMap[key] == RELEASE;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key is being held down.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsKeyHold(Keys key)
	{
		return mKeyMap[key] == HOLD || mKeyMap[key] == PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key is down.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsKeyDown(Keys key)
	{
		return mKeyMap[key] == HOLD || mKeyMap[key] == PRESS || mKeyMap[key] == PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key was pressed in a previous state (currently unused).
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasKeyPressed(Keys key)
	{
		return mKeyMap[key] == PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key was released in a previous state (currently unused).
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasKeyReleased(Keys key)
	{
		return mKeyMap[key] == RELEASED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified key was held down in a previous state
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasKeyHold(Keys key)
	{
		return mKeyMap[key] == RELEASE;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified mouse button is currently pressed.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsMousePressed(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::PRESS;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified mouse button is currently released.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsMouseReleased(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::RELEASE;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified mouse button is being held down.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsMouseHold(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::HOLD || mMouseMap[button] == KeyStates::PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if the mouse is currently positioned over a given area.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsMouseOver(Vec2d& position, Vec2d& size)	{
		// get current mouse position
		const Vec2d& mousePos = mCurrMousePos;

		// define button boundaries
		double left = position.x - (size.x * 0.5);
		double right = position.x + (size.x * 0.5);
		double bottom = position.y - (size.y * 0.5);
		double top = position.y + (size.y * 0.5);

		// check if mouse position is within button boundaries
		return (mousePos.x >= left && mousePos.x <= right &&
			mousePos.y >= bottom && mousePos.y <= top);
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified mouse button was pressed in a previous state.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasMousePressed(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if a specified mouse button was relesed in a previous state.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasMouseReleased(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::RELEASED;
	}

	/* function documentation--------------------------------------------------------------------------
	* \brief      Checks if a specified mouse button was held down in a previous state.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::WasMouseHold(MouseButtons button)
	{
		return mMouseMap[button] == KeyStates::HOLD || mMouseMap[button] == KeyStates::PRESSED;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Checks if the mouse is currently in a dragging state.
	-------------------------------------------------------------------------------------------------*/
	bool InputSystem::IsDragging(MouseButtons button)
	{
		return IsMouseHold(button) && Vector2DIsSimilar(mCurrMousePos, mPrevMousePos);
	}

	#pragma endregion


	#pragma region Mouse Position Getters & Setters

	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the current X-coordinate of the mouse position.
	-------------------------------------------------------------------------------------------------*/
	double InputSystem::GetMouseX()
	{
		return mCurrMousePos.x;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the current Y-coordinate of the mouse position.
	-------------------------------------------------------------------------------------------------*/
	double InputSystem::GetMouseY()
	{
		return mCurrMousePos.y;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the current mouse position.
	-------------------------------------------------------------------------------------------------*/
	Vector2D<double> InputSystem::GetMousePosition()
	{
		return mCurrMousePos;
	}

	// Camera scale means 2.f/num
	Vector2D<float> InputSystem::GetMouseWorldPosition()
	{
		const Transform& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(gGOFactory->mainCam);
		Vec2f ret{ mCurrMousePos };
		ret.x = (ret.x / static_cast<float>(AssetManager::GetInstance()->enConfig.Width) - 0.5f);
		ret.y = -(ret.y / static_cast<float>(AssetManager::GetInstance()->enConfig.Height) - 0.5f);
		Mtx3x3f tmpMtx;
		tmpMtx.rotDegThis(camTrans.Rot());
		tmpMtx.m[0] *= 2.f / camTrans.Scale().x;
		tmpMtx.m[3] *= 2.f / camTrans.Scale().y;
		tmpMtx.m[1] *= 2.f / camTrans.Scale().x;
		tmpMtx.m[4] *= 2.f / camTrans.Scale().y;
		tmpMtx.m[6] = camTrans.Pos().x;
		tmpMtx.m[7] = camTrans.Pos().y;

		return tmpMtx * ret;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Sets the mouse position to specific X and Y coordinates.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::SetMousePosition(double xPos, double yPos)
	{
		mCurrMousePos.x = xPos;
		mCurrMousePos.y = yPos;
	}

	void InputSystem::SetMousePosition(Vector2D<double> pos)
	{
		SetMousePosition(pos.x, pos.y);
	}

	#pragma endregion


	#pragma region Drag Tracking, Getters & Setters
	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the starting position of a drag event.
	-------------------------------------------------------------------------------------------------*/
	Vector2D<double> InputSystem::GetDragStartPos() const
	{
		return mPrevMousePos;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the ending position of a drag event.
	-------------------------------------------------------------------------------------------------*/
	Vector2D<double> InputSystem::GetDragEndPos() const
	{
		return mCurrMousePos;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Retrieves the current position during a drag event.
	-------------------------------------------------------------------------------------------------*/
	Vector2D<double> InputSystem::GetDragCurrentPos() const
	{
		return mCurrMousePos;
	}

	void InputSystem::SetScrollOffset(const double& in)
	{
		mScrollAmt = static_cast<float>(in) * 2.f;
	}

	const float& InputSystem::GetScrollOffset() const
	{
		return mScrollAmt;
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Sets the starting position of a drag event.
	-------------------------------------------------------------------------------------------------*/
	//void InputSystem::SetDragStartPos(const Vector2D<double>& pos)
	//{
	//	dragStartPos = pos;
	//}

	/* function documentation--------------------------------------------------------------------------
	\brief      Sets the ending position of a drag event.
	-------------------------------------------------------------------------------------------------*/
	//void InputSystem::SetDragEndPos(const Vector2D<double>& pos)
	//{
	//	dragEndPos = pos;
	//}

	/* function documentation--------------------------------------------------------------------------
	\brief      Sets the current position during a drag event.
	-------------------------------------------------------------------------------------------------*/
	//void InputSystem::SetDragCurrentPos(const Vector2D<double>& pos)
	//{
	//	dragCurrentPos = pos;
	//}

	/* function documentation--------------------------------------------------------------------------
	\brief      Sets the dragging state to true or false.
	-------------------------------------------------------------------------------------------------*/
	//void InputSystem::SetDragging(bool dragging)
	//{
	//	isDragging = dragging;
	//}

	#pragma endregion


	#pragma region Key & Mouse Map Update Methods
	/* function documentation--------------------------------------------------------------------------
	\brief      Updates the key state map for tracking key presses and releases.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::UpdateKeyMap(int key, KeyStates state)
	{
		mKeyMap[key] = state;
		mChangedKeyQueue.push(std::make_tuple(true, key, state));
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Updates the mouse button state map for tracking button presses and releases.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::UpdateMouseMap(int button, KeyStates state)
	{
		mMouseMap[button] = state;
		mChangedKeyQueue.push(std::make_tuple(false, button, state));
	}

	#pragma endregion


	#pragma region ProxySend & Keycode Functions	
	/* function documentation--------------------------------------------------------------------------
	\brief      Sends a message through the system proxy.
	-------------------------------------------------------------------------------------------------*/
	void InputSystem::ProxySend(Message* msg)
	{
		SendSysMessage(msg);
	}

	/* function documentation--------------------------------------------------------------------------
	\brief      Converts a keycode to a human-readable string representation.
	-------------------------------------------------------------------------------------------------*/
	const char* InputSystem::KeycodeToString(int key)
	{
		switch (key)
		{
			// Printable keys
		case KEY_SPACEBAR:	 return "Spacebar";
		case KEY_APOSTROPHE: return "Apostrophe";
		case KEY_COMMA:		 return "Comma";
		case KEY_MINUS:		 return "Minus";
		case KEY_PERIOD:	 return "Period";
		case KEY_SLASH:		 return "Slash";
		case KEY_SEMICOLON:	 return "Semicolon";
		case KEY_EQUAL:		 return "Equal";

			// Alphabet keys
		case KEY_0: return "0";
		case KEY_1: return "1";
		case KEY_2: return "2";
		case KEY_3: return "3";
		case KEY_4: return "4";
		case KEY_5: return "5";
		case KEY_6: return "6";
		case KEY_7: return "7";
		case KEY_8: return "8";
		case KEY_9: return "9";
		case KEY_A: return "A";
		case KEY_B: return "B";
		case KEY_C: return "C";
		case KEY_D: return "D";
		case KEY_E: return "E";
		case KEY_F: return "F";
		case KEY_G: return "G";
		case KEY_H: return "H";
		case KEY_I: return "I";
		case KEY_J: return "J";
		case KEY_K: return "K";
		case KEY_L: return "L";
		case KEY_M: return "M";
		case KEY_N: return "N";
		case KEY_O: return "O";
		case KEY_P: return "P";
		case KEY_Q: return "Q";
		case KEY_R: return "R";
		case KEY_S: return "S";
		case KEY_T: return "T";
		case KEY_U: return "U";
		case KEY_V: return "V";
		case KEY_W: return "W";
		case KEY_X: return "X";
		case KEY_Y: return "Y";
		case KEY_Z: return "Z";

			// Function keys
		case KEY_F01: return "F1";
		case KEY_F02: return "F2";
		case KEY_F03: return "F3";
		case KEY_F04: return "F4";
		case KEY_F05: return "F5";
		case KEY_F06: return "F6";
		case KEY_F07: return "F7";
		case KEY_F08: return "F8";
		case KEY_F09: return "F9";
		case KEY_F10: return "F10";
		case KEY_F11: return "F11";
		case KEY_F12: return "F12";

			// Arrow keys
		case KEY_UP: return "Arrow Up";
		case KEY_DOWN: return "Arrow Down";
		case KEY_LEFT: return "Arrow Left";
		case KEY_RIGHT: return "Arrow Right";

			// Modifier keys
		case KEY_LEFT_SHIFT: return "Left Shift";
		case KEY_LEFT_CONTROL: return "Left Control";
		case KEY_LEFT_ALT: return "Left Alt";
		case KEY_LEFT_SUPER: return "Left Super";
		case KEY_RIGHT_SHIFT: return "Right Shift";
		case KEY_RIGHT_CONTROL: return "Right Control";
		case KEY_RIGHT_ALT: return "Right Alt";
		case KEY_RIGHT_SUPER: return "Right Super";
		case KEY_MENU: return "Menu";

			// Misc keys
		case KEY_ESCAPE: return "Escape";
		case KEY_ENTER: return "Enter";
		case KEY_TAB: return "Tab";
		case KEY_BACKSPACE: return "Backspace";
		case KEY_INSERT: return "Insert";
		case KEY_DELETE: return "Delete";
		case KEY_PAGE_UP: return "Page Up";
		case KEY_PAGE_DOWN: return "Page Down";
		case KEY_HOME: return "Home";
		case KEY_END: return "End";
		case KEY_CAPS_LOCK: return "Caps Lock";
		case KEY_SCROLL_LOCK: return "Scroll Lock";
		case KEY_NUM_LOCK: return "Num Lock";
		case KEY_PRINT_SCREEN: return "Print Screen";
		case KEY_PAUSE: return "Pause";

			// Special keys
		case KEY_GRAVE_ACCENT: return "Grave Accent";
		case KEY_WORLD_1: return "World 1";
		case KEY_WORLD_2: return "World 2";

			// Mouse buttons (if applicable in the same function)
		case MOUSE_BUTTON_LEFT: return "Mouse Button Left";
		case MOUSE_BUTTON_RIGHT: return "Mouse Button Right";
		case MOUSE_BUTTON_MIDDLE: return "Mouse Button Middle";

		default: return "Unknown Key";
		}
	}

	#pragma endregion

}
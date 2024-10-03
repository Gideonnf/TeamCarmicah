#include "pch.h"
#include "Input/InputSystem.h"
#include "Messaging/InputMessage.h"
#include "log.h"
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
		//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		//	glfwSetWindowShouldClose(window, GL_TRUE);
	}

	void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		UNUSED(mods);
		UNUSED(window);
		Input.UpdateMouseMap(button, (KeyStates)action);
	}

	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		UNUSED(window);
		Input.SetMousePosition(xPos, yPos);
		//std::cout << "Mouse pos: " << xPos << ", " << yPos << std::endl;
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
		std::cout << "in update mouse map" << mMousePressed << std::endl;
		std::cout << "Mouse State : " << state << " For : " << key << std::endl;
	}
}
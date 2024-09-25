#include "pch.h"
#include "Systems/InputSystem.h"
#include "Messaging/InputMessage.h"
namespace Carmicah
{
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Input.UpdateMap(key, (KeyStates)action);
		//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		//	glfwSetWindowShouldClose(window, GL_TRUE);
	}

	void InputSystem::Init(GLFWwindow* ref)
	{
		windowRef = ref;
		int keyCode = 5;
		KeyMessage msg(keyCode);
		SendMessage(&msg);

		glfwSetKeyCallback(windowRef, key_callback);
		
		//GLFW_MOUSE_BUTTON_LEFT;
	}

	void InputSystem::Update()
	{
		//key_callback
	}

	bool InputSystem::IsKeyPressed(Keys key)
	{
		return false;
	}

	bool InputSystem::IsKeyReleased(Keys key)
	{
		return false;

	}

	bool InputSystem::IsKeyHold(Keys key)
	{
		return false;

	}

	bool InputSystem::IsMousePressed(MouseButtons button)
	{
		return false;

	}

	bool InputSystem::IsMouseReleased(MouseButtons button)
	{
		return false;

	}

	bool InputSystem::IsMouseHold(MouseButtons button)
	{
		return false;

	}

	Vector2D<float> InputSystem::GetMousePosition()
	{
		return Vector2D<float>();
	}

	float InputSystem::GetMouseX()
	{
		return float{};
	}

	float InputSystem::GetMouseY()
	{
		return float{};
	}

	void InputSystem::UpdateMap(int key, KeyStates state)
	{
		mKeyMap[key] = state;
		std::cout << "State : " << state << " For : " << key << std::endl;
	}

}
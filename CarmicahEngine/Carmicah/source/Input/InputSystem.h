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

	public:
		InputSystem() : windowRef(nullptr), mMousePressed(false), mMouseTick(0) {}
		~InputSystem() {};

		// Not using the inherited singleton template class cause we want the base system inheritance
		static InputSystem* GetInstance() {
			static InputSystem instance;
			return &instance;
		}

		void Init(GLFWwindow* ref);

		void Update();

		void UpdatePrevInput();

		bool IsKeyPressed(Keys key);
		bool IsKeyReleased(Keys key);
		bool IsKeyHold(Keys key);

		bool IsMousePressed(MouseButtons button);
		bool IsMouseReleased(MouseButtons button);
		bool IsMouseHold(MouseButtons button);

		Vector2D<double> GetMousePosition();
		double GetMouseX();
		double GetMouseY();

		void SetMousePosition(double xPos, double yPos);

		void UpdateKeyMap(int key, KeyStates state);
		void UpdateMouseMap(int key, KeyStates state);
	};

	// Just so people can call "Input.IsKeyPressed" 
	static InputSystem& Input = *InputSystem::GetInstance();


}
#endif

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "GLFW/glfw3.h"
#include <unordered_map>
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


	public:
		InputSystem(){}
		~InputSystem() {};

		static InputSystem* GetInstance() {
			static InputSystem instance;
			return &instance;
		}

		void Init(GLFWwindow* ref);

		void Update();

		bool IsKeyPressed(Keys key);
		bool IsKeyReleased(Keys key);
		bool IsKeyHold(Keys key);

		bool IsMousePressed(MouseButtons button);
		bool IsMouseReleased(MouseButtons button);
		bool IsMouseHold(MouseButtons button);

		Vector2D<float> GetMousePosition();
		float GetMouseX();
		float GetMouseY();

		void UpdateMap(int key, KeyStates state);
	};
	static InputSystem& Input = *InputSystem::GetInstance();


}
#endif

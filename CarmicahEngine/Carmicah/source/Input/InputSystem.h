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

		void Init(GLFWwindow* ref);

		void Update();

		void UpdatePrevInput();

		bool IsKeyPressed(Keys key);
		bool IsKeyReleased(Keys key);
		bool IsKeyHold(Keys key);

		bool WasKeyPressed(Keys key);
		bool WasKeyReleased(Keys key);
		bool WasKeyHold(Keys key);

		bool IsMousePressed(MouseButtons button);
		bool IsMouseReleased(MouseButtons button);
		bool IsMouseHold(MouseButtons button);

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

		const char* KeycodeToString(Keys key);
	};

	// Just so people can call "Input.IsKeyPressed" 
	static InputSystem& Input = *InputSystem::GetInstance();
}
#endif

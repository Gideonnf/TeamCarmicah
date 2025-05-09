/* File Documentation -----------------------------------------------------------------------------
file:           InputSystem.h

\author			Micah Lim (80%), Gideon Francis (20%)

email:          micahshengyao.lim@digipen.edu

brief:          This file declares the InputSystem class, which manages the input handling for
				keyboard and mouse events in a game engine. It includes functionality for tracking
				key and mouse states, handling drag events, and converting keycodes to string
				representations. The InputSystem class integrates GLFW for callback handling.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


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
		std::queue<std::tuple<bool, int, KeyStates>> mChangedKeyQueue;// true is key, false is mouse

		Vector2D<double> mCurrMousePos;
		Vector2D<double> mPrevMousePos;
		float mScrollAmt;

		const double sScreenHeight = 1080;
	public:
		bool mNotFullScreen{};
		Vec2i mWindowScale{};


		InputSystem() : windowRef(nullptr), mScrollAmt(0.0f) {}
		~InputSystem() {};

		// Not using the inherited singleton template class cause we want the base system inheritance
		static InputSystem* GetInstance()
		{
			static InputSystem instance;
			return &instance;
		}

		void CloseGame();

		void ToggleFullScreen();


#pragma region Init & Update

		void Init(GLFWwindow* ref);
		void Update();
		void UpdatePrevInput();

#pragma endregion


#pragma region Key & Mouse State Methods
		bool IsDragging(MouseButtons button);
		bool IsMouseOver(Vec2d& position, Vec2d& size);

		bool IsKeyPressed(Keys key);
		bool IsKeyReleased(Keys key);
		bool IsKeyHold(Keys key);
		bool IsKeyDown(Keys key);

		bool IsMousePressed(MouseButtons button);
		bool IsMouseReleased(MouseButtons button);
		bool IsMouseHold(MouseButtons button);

		bool WasKeyPressed(Keys key);
		bool WasKeyReleased(Keys key);
		bool WasKeyHold(Keys key);

		bool WasMousePressed(MouseButtons button);
		bool WasMouseReleased(MouseButtons button);
		bool WasMouseHold(MouseButtons button);

#pragma endregion


#pragma region Key & Mouse Map Update Methods

		void UpdateKeyMap(int key, KeyStates state);
		void UpdateMouseMap(int key, KeyStates state);

#pragma endregion


#pragma region Mouse Position Getters

		double GetMouseX();
		double GetMouseY();
		Vector2D<double> GetMousePosition();
		Vector2D<float> GetMouseWorldPosition();
		void SetMousePosition(double xPos, double yPos);
		void SetMousePosition(Vector2D<double> pos);

#pragma endregion


#pragma region Drag & Held Tracking, Getters & Setters

		//void SetDragging(bool dragging);

		//void SetDragStartPos(const Vector2D<double>& pos);
		//void SetDragEndPos(const Vector2D<double>& pos);
		//void SetDragCurrentPos(const Vector2D<double>& pos);

		Vector2D<double> GetDragStartPos() const;
		Vector2D<double> GetDragEndPos() const;
		Vector2D<double> GetDragCurrentPos() const;
		void SetScrollOffset(const double& in);
		const float& GetScrollOffset() const;
#pragma endregion


		void ProxySend(Message*);
		const char* KeycodeToString(int key);
	};

	static InputSystem& Input = *InputSystem::GetInstance(); // so people can call "Input.IsKeyPressed" 
}
#endif
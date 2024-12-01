/* File Documentation -----------------------------------------------------------------------------
file:           CameraWindow.h

author:			Won Yu Xuan Rainne(100%)

email:			won.m@digipen.edu

brief:          This file defines the CameraWindow class, which extends the EditorWindow class.
				It provides a graphical user interface (GUI) component for interacting with and
				manipulating camera properties within the editor. The class includes fields for
				tracking object positions, initial click positions, and the original angle of the
				camera during transformations. It overrides the Update method to integrate custom
				behavior specific to the camera window.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#ifndef CAMERAWINDOW_H_
#define CAMERAWINDOW_H_


#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "Editor.h"
#include "log.h"
#include "../ECS/BaseSystem.h"
#include "Math/Vec2.h"

namespace Carmicah
{
	class CameraWindow : public EditorWindow
	{
	private:
		Vec2f mSelectedObjPos;
		Vec2f mStartClickPos;
		float mOriginalAngle;
	public:

		/**
		 * @brief Construct a new Scene Window object
		 *
		 */
		CameraWindow();
		/**
		 * @brief SceneWindow Update override from EditorWindow
		 *
		 */
		void Update() override;


	};
}

#endif
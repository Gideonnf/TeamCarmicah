/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:          CameraWindow.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:         Handles getting input from imGUI side when in the editor window

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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

		/*!*************************************************************************
		brief
			Constructs the Camera window (editor mode)
		***************************************************************************/
		CameraWindow();

		/*!*************************************************************************
		brief
			Takes over input calculations when mouse is hovering over the imGUI window
			Also handles moving game objects when selected using the gizmos
		***************************************************************************/
		void Update() override;


	};
}

#endif
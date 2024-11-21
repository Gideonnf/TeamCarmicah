/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SceneWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This SceneWindow class is a derived class.
				It provides an interactive view where users can visualize and manipulate game objects within the scene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef SCENEWINDOW_H_
#define SCENEWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "Editor.h"
#include "log.h"
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class SceneWindow : public EditorWindow
	{
	private:


	public:

		static bool mIsPlaying;
		static bool mIsPaused;
		static bool mChangeState;

		/**
		 * @brief Construct a new Scene Window object
		 * 
		 */
		SceneWindow();
		/**
		 * @brief SceneWindow Update override from EditorWindow
		 * 
		 */
		void Update() override;


	};
}

#endif
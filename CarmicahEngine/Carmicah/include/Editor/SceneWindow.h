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
#include "Editor/EditorWindow.h"
#include "log.h"

namespace Carmicah
{
	class SceneWindow : public EditorWindow
	{
	private:


	public:

		SceneWindow();

		void Update() override;


	};
}

#endif
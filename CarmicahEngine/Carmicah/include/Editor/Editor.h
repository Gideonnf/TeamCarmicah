/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Editor.h

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:			This editor class manages and coordinates the various editor windows.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef EDITOR_H_
#define EDITOR_H_
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "Editor/DebugWindow.h"
#include "Editor/EditorWindow.h"
#include "Editor/HierarchyWindow.h"
#include "Editor/SceneWindow.h"

namespace Carmicah
{
	class Editor
	{
	public:

		Editor();

		~Editor();

		void Init(GLFWwindow* window);

		void Update();

		void Render(GLFWwindow* window);

		void Exit();

	private:

		std::vector<std::unique_ptr<EditorWindow>> mWindows;
	};

}

#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			EditorWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This EditorWindow class serves as a base class for various editor window types, containing basic functionality.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"

namespace Carmicah
{

	EditorWindow::EditorWindow(const char* title, ImVec2 size, ImVec2 pos) : mTitle(title), mSize(size), mPos(pos), mIsVisible(true) {}

	void EditorWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
		}
		ImGui::End();
	}
}
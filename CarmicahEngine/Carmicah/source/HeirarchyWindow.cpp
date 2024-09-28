#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/HeirarchyWindow.h"

namespace Carmicah
{
	HeirarchyWindow::HeirarchyWindow() : EditorWindow("Heirarchy", ImVec2(900, 300), ImVec2(0, 0)) { isVisible = true; }

	void HeirarchyWindow::Update()
	{

	}
}
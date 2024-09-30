#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"

namespace Carmicah
{

	EditorWindow::EditorWindow(const char* title, ImVec2 size, ImVec2 pos) : title(title), size(size), pos(pos), isVisible(true) {}

	void EditorWindow::Update()
	{
		if (ImGui::Begin(title))
		{
			ImGui::Text("Default");
		}
		ImGui::End();
	}
}
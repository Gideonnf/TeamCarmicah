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
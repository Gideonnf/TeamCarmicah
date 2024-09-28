#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/DebugWindow.h"

namespace Carmicah
{
	DebugWindow::DebugWindow() : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0)), showFPS(false) { isVisible = true; }

	void DebugWindow::Update()
	{
		static int clicked;
		static int fps; //TODO:: Replace with Actual FPS system later
		fps++;

		if(ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar))
		{
			//Draw Menu Bar
			if (ImGui::BeginMenuBar())
			{
				//Menu for Debugging Tools
				if (ImGui::BeginMenu("Debugging Tools"))
				{
					if (ImGui::MenuItem("FPS", nullptr, showFPS))
					{
						showFPS = !showFPS;
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();


			if (showFPS)
			{
				ImGui::Text("FPS: %d", fps);
			}

			if (ImGui::Button("Counter Clicker"))
			{
				clicked++;
			}

			ImGui::Text("Clicked: %d", clicked);

		}
		ImGui::End();

	}
}
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/DebugWindow.h"

namespace Carmicah
{
	DebugWindow::DebugWindow() : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0)), showFPS(false), showLogger(true) { isVisible = true; }

	void DebugWindow::Update()
	{
		static int clicked;
		static int counter; //TODO:: Replace with Actual FPS system later
		static ImGuiTextBuffer logBuffer;
		static bool autoScrollLog = true;
		counter++;

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

			if(ImGui::BeginTabBar("Debug Tabs"))
			{
				if (showFPS)
				{
					if (ImGui::BeginTabItem("FPS Info"))
					{
						ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
						ImGui::EndTabItem();
					}
				}
				if (showLogger)
				{
					if (ImGui::BeginTabItem("Logger"))
					{
						//Append logs here
						logBuffer.append(std::to_string(counter).c_str());
						logBuffer.append("\n");

						ImGui::BeginChild("Logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
						ImGui::TextUnformatted(logBuffer.begin(), logBuffer.end());
						ImGui::EndChild();
						ImGui::EndTabItem();
					}
				}
				ImGui::EndTabBar();
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
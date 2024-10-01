#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/DebugWindow.h"
#include "CarmicahTime.h"

namespace Carmicah
{
	DebugWindow::DebugWindow() : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0)), mShowFPS(false), mShowLogger(true) { mIsVisible = true; }

	void DebugWindow::Update()
	{
		static int clicked;
		static int counter; //TODO:: Replace with Actual FPS system later
		static ImGuiTextBuffer logBuffer;
		static bool autoScrollLog = true;
		counter++;

		if(ImGui::Begin(mTitle, nullptr, ImGuiWindowFlags_MenuBar))
		{
			//Draw Menu Bar
			if (ImGui::BeginMenuBar())
			{
				//Menu for Debugging Tools
				if (ImGui::BeginMenu("Debugging Tools"))
				{
					if (ImGui::MenuItem("FPS", nullptr, mShowFPS))
					{
						mShowFPS = !mShowFPS;
					}

					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();

			if(ImGui::BeginTabBar("Debug Tabs"))
			{
				if (mShowFPS)
				{
					if (ImGui::BeginTabItem("FPS Info"))
					{
						ImGui::Text("FPS: %f", (static_cast<float>(CarmicahTimer::GetFPS())));
						ImGui::EndTabItem();
					}
				}
				if (mShowLogger)
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
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
						//Debug button press thing
						if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_D)))
						{
							Log::logMessage("Log Test Button Pressed");
						}

						static bool sAutoScroll = true;
						const auto& logMessages = Carmicah::Log::getLogs();

						ImGui::BeginChild("Logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
						for(const auto& msg : logMessages)
						{
							ImGui::TextUnformatted(msg.c_str());
						}
						if (sAutoScroll)
						{
							ImGui::SetScrollHereY(1.0f);
						}
						if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
						{
							sAutoScroll = false;
						}

						ImGui::EndChild();
						ImGui::EndTabItem();
					}
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

	}
}
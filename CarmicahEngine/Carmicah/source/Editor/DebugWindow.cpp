/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			DebugWindow.cpp

 author:		Nicholas Lai (60%)
 co-author(s):	Yang Yujie (40%)

 email:			n.lai@digipen.edu

 brief:			This DebugWindow class is a derived class from EditorWindow.
                It currently provides an FPS tracker, a console log and profiler for tracking purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "DebugWindow.h"
#include "CarmicahTime.h"

namespace Carmicah
{
    DebugWindow::DebugWindow() : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0)),
        mShowFPS(true), mShowLogger(true), mShowProfiling(true), mShowGPUProfiling(true) {
        mIsVisible = true;
    }

    void DebugWindow::Update()
    {
        static int clicked;
        static int counter;
        static ImGuiTextBuffer logBuffer;
        static bool autoScrollLog = true;
        counter++;
        
        if (ImGui::Begin(mTitle, nullptr, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Debugging Tools"))
                {
                    ImGui::MenuItem("FPS", nullptr, &mShowFPS);
                    ImGui::MenuItem("Logger", nullptr, &mShowLogger);
                    ImGui::MenuItem("Profiling", nullptr, &mShowProfiling);
                    ImGui::MenuItem("GPU Profiling", nullptr, &mShowGPUProfiling);
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenuBar();

            //Debugging tabs for FPS, Logger, Profiling, GPU Profiling
			if(ImGui::BeginTabBar("Debug Tabs"))
			{
				if (mShowFPS && ImGui::BeginTabItem("FPS Info"))
				{
					ImGui::Text("FPS: %f", (static_cast<float>(CarmicahTimer::GetFPS())));
					ImGui::EndTabItem();
				}
                if (mShowLogger && ImGui::BeginTabItem("Logger"))
                {
                    //Debug button press thing
                    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_D)))
                    {
                        Log::logMessage("Log Test Button Pressed");

                    }

                    static bool sAutoScroll = true;
                    const auto& logMessages = Carmicah::Log::getLogs();

                    //ImGui::Text("Log Messages");
                    ImGui::BeginChild("Logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
                    for (const auto& msg : logMessages)
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
                //Render the profiling tab for the Debug Window to display the profiling data
                if (mShowProfiling && ImGui::BeginTabItem("Profiling-Game"))
                {
                    RenderProfilingTab();
                    ImGui::EndTabItem();
                }
                //  Render the GPU profiling tab for the Debug Window to display the GPU profiling data
                if (mShowGPUProfiling && ImGui::BeginTabItem("GPU Profiling"))
                {
                    RenderGPUProfilingTab();
                    ImGui::EndTabItem();
                }
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
    }

    //Render the profiling tab for the Debug Window to display the profiling data
    void DebugWindow::RenderProfilingTab()
    {
        const auto& systemPercentages = CarmicahTimer::GetSystemPercentages();
        double totalLoopTime = CarmicahTimer::GetTotalLoopTime();

        ImGui::Text("Total Loop Time: %.3f ms", totalLoopTime * 1000.0);
        ImGui::Separator();

        if (totalLoopTime > 0) {
            ImGui::Text("Total Loop Time: %.3f ms", totalLoopTime * 1000.0);
            ImGui::Separator();

            for (const auto& [systemName, percentage] : systemPercentages)
            {
                if (!systemName.empty()) {
                    ImGui::Text("%s: %.2f%%", systemName.c_str(), percentage);
                    ImGui::ProgressBar(static_cast<float>(percentage) / 100.0f);
                }
            }
        }
        else {
            ImGui::Text("No profiling data available yet.");
        }
    }

    //Render the GPU profiling tab for the Debug Window to display the GPU profiling data  
    void DebugWindow::RenderGPUProfilingTab()
    {
        double gpuTime = CarmicahTimer::GetGPUTime();
        double totalLoopTime = CarmicahTimer::GetTotalLoopTime();

        if (totalLoopTime > 0) {
            ImGui::Text("GPU Time: %.3f ms", gpuTime);
            ImGui::ProgressBar(static_cast<float>(gpuTime / (totalLoopTime * 1000.0)));
        }
        else {
            ImGui::Text("No GPU profiling data available yet.");
        }
    }
}
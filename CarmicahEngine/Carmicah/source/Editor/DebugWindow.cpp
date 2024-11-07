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
        mShowPerformance(true), mShowLogger(true){
        mIsVisible = true;
    }

    ImU32 GetSystemColor(const std::string& systemName) {
        // Simple string hash function
        std::size_t hash = 0;
        for (char c : systemName) {
            hash = (hash * 31) + c;
        }

        // Generate RGB colors from hash
        unsigned char r = (hash & 0xFF) | 0x40;        // Ensure some red
        unsigned char g = ((hash >> 8) & 0xFF) | 0x40; // Ensure some green
        unsigned char b = ((hash >> 16) & 0xFF) | 0x40; // Ensure some blue

        return IM_COL32(r, g, b, 255);
    }

    void DebugWindow::RenderProfilingTab()
    {
        const auto& systemPercentages = CarmicahTimer::GetSystemPercentages();
        double totalLoopTime = CarmicahTimer::GetTotalLoopTime();

        // Performance Overview Section
        ImGui::BeginChild("Performance Overview", ImVec2(0, 60), true);
        {
            ImGui::Text("Frame Time: %.2f ms", CarmicahTimer::GetDt() * 1000.0);
            ImGui::SameLine(200);
            ImGui::Text("FPS: %.1f", CarmicahTimer::GetFPS());

            ImGui::Text("GPU Time: %.2f ms", CarmicahTimer::GetGPUTime());
            ImGui::SameLine(200);
            ImGui::Text("Total Loop Time: %.2f ms", totalLoopTime * 1000.0);
        }
        ImGui::EndChild();

        // System Timeline
        ImGui::Text("System Timeline");
        ImGui::BeginChild("Timeline", ImVec2(0, 100), true);
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            float xPos = canvas_pos.x;
            const float height = 20.0f;

            // Draw timeline background
            draw_list->AddRectFilled(
                canvas_pos,
                ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + height),
                IM_COL32(30, 30, 30, 255)
            );

            // Draw each system's time slice
            for (const auto& pair : systemPercentages) {
                float width = (static_cast<float>(pair.second) / 100.0f) * canvas_size.x;
                ImU32 color = GetSystemColor(pair.first);

                draw_list->AddRectFilled(
                    ImVec2(xPos, canvas_pos.y),
                    ImVec2(xPos + width, canvas_pos.y + height),
                    color
                );

                // System label if there's enough space
                if (width > 50.0f) {
                    draw_list->AddText(
                        ImVec2(xPos + 2, canvas_pos.y + 2),
                        IM_COL32(255, 255, 255, 255),
                        pair.first.c_str()
                    );
                }

                // Tooltip on hover
                if (ImGui::IsMouseHoveringRect(
                    ImVec2(xPos, canvas_pos.y),
                    ImVec2(xPos + width, canvas_pos.y + height)))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s\nTime: %.3f ms\nPercentage: %.1f%%",
                        pair.first.c_str(),
                        totalLoopTime * (pair.second / 100.0) * 1000.0,
                        pair.second);
                    ImGui::EndTooltip();
                }

                xPos += width;
            }
        }
        ImGui::EndChild();

        // System Stats Table
        ImGui::Text("System Statistics");
        if (ImGui::BeginTable("System Stats", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("System");
            ImGui::TableSetupColumn("Time (ms)");
            ImGui::TableSetupColumn("% of Frame");
            ImGui::TableHeadersRow();

            for (const auto& pair : systemPercentages) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%s", pair.first.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%.3f", totalLoopTime * (pair.second / 100.0) * 1000.0);
                ImGui::TableNextColumn();
                ImGui::Text("%.1f%%", pair.second);
            }
            ImGui::EndTable();
        }
    }

    void DebugWindow::Update()
    {
        if (ImGui::Begin(mTitle, nullptr, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Debugging Tools"))
                {
                    ImGui::MenuItem("Performance", nullptr, &mShowPerformance);
                    ImGui::MenuItem("Logger", nullptr, &mShowLogger);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginTabBar("Debug Tabs"))
            {
                if (mShowPerformance && ImGui::BeginTabItem("Performance"))
                {
                    RenderProfilingTab();
                    ImGui::EndTabItem();
                }

                if (mShowLogger && ImGui::BeginTabItem("Logger"))
                {
                    // Keep the auto-scroll behavior
                    static bool autoScroll = true;
                    const auto& logMessages = Carmicah::Log::getLogs();

                    ImGui::BeginChild("Logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
                    for (const auto& msg : logMessages)
                    {
                        ImGui::TextUnformatted(msg.c_str());
                    }
                    if (autoScroll)
                    {
                        ImGui::SetScrollHereY(1.0f);
                    }
                    if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
                    {
                        autoScroll = false;
                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}
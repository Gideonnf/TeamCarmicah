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
    std::unordered_map<std::string, DebugWindow::DebugStatistics> DebugWindow::debugStats;

    DebugWindow::DebugWindow()
        : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0))
        , mShowPerformance(true)
        , mShowLogger(true)
    {
        mIsVisible = true;
    }


    void DebugWindow::UpdateDebugStatistics()
    {
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        float xPos = canvas_pos.x;
        UNUSED(xPos);
        const auto& systemPercentages = CarmicahTime::GetInstance()->GetSystemPercentages();
        double totalLoopTime = CarmicahTime::GetInstance()->GetTotalLoopTime();

        for (const auto& pair : systemPercentages)
        {
            DebugStatistics stats;
            stats.width = (static_cast<float>(pair.second) / 100.0f) * canvas_size.x;
            stats.timeTaken = (float)(totalLoopTime * (pair.second / 100.0) * 1000.0);
            stats.loadPercentage = (float)pair.second;
            debugStats.insert_or_assign(pair.first, stats);
        }
    }

    /*
    @beief: This function is used to get the color of the system based on the system name.
    @param: const std::string& systemName
    @return: ImU32
    The function returns the color of the system based on the system name.
    */
    ImU32 DebugWindow::GetSystemColor(const std::string& systemName)
    {
        // Simple string hash function for consistent colors
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

    // Render the performance overview section
    void DebugWindow::RenderPerformanceOverview()
    {
        ImGui::BeginChild("Performance Overview", ImVec2(0, 60), true);
        {
            static float accumulatedTime = 0.0f;
            float dt = (float)CarmicahTime::GetInstance()->ForceDeltaTime();
            static float frameTime = static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime() * 1000.0);
            static float fps = static_cast<float>(CarmicahTime::GetInstance()->FPS());
            static float gpuTime = static_cast<float>(CarmicahTime::GetInstance()->GetGPUTime());
            static float totalLoopTime = static_cast<float>(CarmicahTime::GetInstance()->GetTotalLoopTime() * 1000.0);
            accumulatedTime += dt;

            if(accumulatedTime >= 0.5f)
            {
                frameTime = static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime() * 1000.0);
                fps = static_cast<float>(CarmicahTime::GetInstance()->FPS());
                gpuTime = static_cast<float>(CarmicahTime::GetInstance()->GetGPUTime());
                totalLoopTime = static_cast<float>(CarmicahTime::GetInstance()->GetTotalLoopTime() * 1000.0);
                accumulatedTime = 0.0f;
            }

            ImGui::Text("Frame Time: %.2f ms", frameTime);
            ImGui::SameLine(200);
            ImGui::Text("FPS: %.1f", fps);

            ImGui::Text("GPU Time: %.2f ms", gpuTime);
            ImGui::SameLine(200);
            ImGui::Text("Total Loop Time: %.2f ms", totalLoopTime);
        }
        ImGui::EndChild();
    }

    // Render the system timeline section
    void DebugWindow::RenderSystemTimeline()
    {
        const auto& systemPercentages = CarmicahTime::GetInstance()->GetSystemPercentages();
        double totalLoopTime = CarmicahTime::GetInstance()->GetTotalLoopTime();
        UNUSED(totalLoopTime);
        static float accumulatedTime = 0.0f;
        float dt = (float)CarmicahTime::GetInstance()->ForceDeltaTime();
        accumulatedTime += dt;

        ImGui::Text("System Timeline");
        ImGui::BeginChild("Timeline", ImVec2(0, 30), ImGuiChildFlags_AutoResizeY);
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
                ImU32 color = GetSystemColor(pair.first);

                draw_list->AddRectFilled(
                    ImVec2(xPos, canvas_pos.y),
                    ImVec2(xPos + debugStats[pair.first].width, canvas_pos.y + height),
                    color
                );

                // System label if there's enough space
                if (debugStats[pair.first].width > 50.0f) {
                    draw_list->AddText(
                        ImVec2(xPos + 2, canvas_pos.y + 2),
                        IM_COL32(255, 255, 255, 255),
                        pair.first.c_str()
                    );
                }

                // Tooltip on hover
                if (ImGui::IsMouseHoveringRect(
                    ImVec2(xPos, canvas_pos.y),
                    ImVec2(xPos + debugStats[pair.first].width, canvas_pos.y + height)))
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s\nTime: %.3f ms\nPercentage: %.1f%%",
                        pair.first.c_str(),
                        debugStats[pair.first].timeTaken,
                        debugStats[pair.first].loadPercentage);
                    ImGui::EndTooltip();
                }

                xPos += debugStats[pair.first].width;
            }
        }
        ImGui::EndChild();
    }

    // Render the system statistics section
    void DebugWindow::RenderSystemStatistics()
    {
        const auto& systemPercentages = CarmicahTime::GetInstance()->GetSystemPercentages();
        double totalLoopTime = CarmicahTime::GetInstance()->GetTotalLoopTime();
        UNUSED(totalLoopTime);
        ImGui::Text("System Statistics");
        ImGui::BeginChild("System Stats", ImVec2(0, 0), true);
        {
            const float columnWidth = ImGui::GetContentRegionAvail().x - 20;
            UNUSED(columnWidth);
            for (const auto& pair : systemPercentages) {
                // Create a bordered box for each system
                ImGui::BeginChild(pair.first.c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
                {
                    // System Name with colored indicator

                    ImU32 color = GetSystemColor(pair.first);
                    ImGui::ColorButton("##color", ImColor(color), ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16));
                    ImGui::SameLine();
                    ImGui::Text("%s", pair.first.c_str());

                    // Performance metrics in two columns
                    ImGui::Columns(2, nullptr, false);

                    ImGui::Text("Time:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%.3f ms",
                        debugStats[pair.first].timeTaken);

                    ImGui::NextColumn();

                    ImGui::Text("Load:");
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%.1f%%",
                        debugStats[pair.first].loadPercentage);

                    ImGui::Columns(1);

                    // Progress bar showing system load
                    ImGui::ProgressBar((float)debugStats[pair.first].loadPercentage / 100.0f, ImVec2(-1, 4), "");
                }
                ImGui::EndChild();
                ImGui::Spacing();
            }
        }
        ImGui::EndChild();
    }

    void DebugWindow::RenderLoggerTab()
    {
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
        else
        {
            autoScroll = true;
        }
        ImGui::EndChild();
    }



    void DebugWindow::RenderProfilingTab()
    {
        static float accumulatedTime = 0.0f;
        static bool firstTime = true;
        float dt = (float)CarmicahTime::GetInstance()->ForceDeltaTime();
        accumulatedTime += dt;

        if (firstTime)
        {
            UpdateDebugStatistics();
            firstTime = false;
        }

        if (accumulatedTime >= 0.5f)
        {
            UpdateDebugStatistics();
            accumulatedTime = 0.0f;
        }

        RenderPerformanceOverview();
        RenderSystemTimeline();
        RenderSystemStatistics();
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
                if (mShowLogger && ImGui::BeginTabItem("Logger"))
                {
                    RenderLoggerTab();
                    ImGui::EndTabItem();
                }
                if (mShowPerformance && ImGui::BeginTabItem("Performance"))
                {
                    RenderProfilingTab();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}
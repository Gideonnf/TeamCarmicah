#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/DebugWindow.h"
#include "CarmicahTime.h"

namespace Carmicah
{
    DebugWindow::DebugWindow() : EditorWindow("Debug", ImVec2(900, 300), ImVec2(0, 0)),
        showFPS(false), showLogger(true), showProfiling(true), showGPUProfiling(true) {
        isVisible = true;
    }

    void DebugWindow::Update()
    {
        static int clicked;
        static int counter;
        static ImGuiTextBuffer logBuffer;
        static bool autoScrollLog = true;
        counter++;

        if (ImGui::Begin(title, nullptr, ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Debugging Tools"))
                {
                    ImGui::MenuItem("FPS", nullptr, &showFPS);
                    ImGui::MenuItem("Logger", nullptr, &showLogger);
                    ImGui::MenuItem("Profiling", nullptr, &showProfiling);
                    ImGui::MenuItem("GPU Profiling", nullptr, &showGPUProfiling);
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenuBar();

            if (ImGui::BeginTabBar("Debug Tabs"))
            {
                if (showFPS && ImGui::BeginTabItem("FPS Info"))
                {
                    ImGui::Text("FPS: %f", (static_cast<float>(CarmicahTimer::GetFPS())));
                    ImGui::EndTabItem();
                }
                if (showLogger && ImGui::BeginTabItem("Logger"))
                {
                    logBuffer.append(std::to_string(counter).c_str());
                    logBuffer.append("\n");

                    ImGui::BeginChild("Logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
                    ImGui::TextUnformatted(logBuffer.begin(), logBuffer.end());
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                if (showProfiling && ImGui::BeginTabItem("Profiling-Game"))
                {
                    RenderProfilingTab();
                    ImGui::EndTabItem();
                }
                if (showGPUProfiling && ImGui::BeginTabItem("GPU Profiling"))
                {
                    RenderGPUProfilingTab();
                    ImGui::EndTabItem();
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
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			DebugWindow.h

 author:		Nicholas Lai (60%)
 co-author(s):	Yang Yujie (40%)

 email:			n.lai@digipen.edu

 brief:			This DebugWindow class is a derived class from EditorWindow.
                It currently provides an FPS tracker, a console log and profiler for tracking purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DEBUGWINDOW_H_
#define DEBUGWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "log.h"

/*
* @brief DebugWindow class is a derived class from EditorWindow.
* It currently provides an FPS tracker, a console log and profiler for tracking purposes.
*/
namespace Carmicah
{
    class DebugWindow : public EditorWindow
    {
    private:
        bool mShowPerformance;
        bool mShowLogger;

        // Helper functions for rendering different sections
        void RenderProfilingTab();
        void RenderPerformanceOverview();
        void RenderSystemTimeline();
        void RenderSystemStatistics();
        void RenderLoggerTab();

        // Helper function to generate consistent colors for systems
        ImU32 GetSystemColor(const std::string& systemName);

    public:
        DebugWindow();
        void Update() override;
    };
}

#endif
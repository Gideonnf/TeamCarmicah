
#include "pch.h"
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "../ECS/BaseSystem.h"
#include "EditorWindow.h"
#include "CameraWindow.h"


namespace Carmicah
{

    CameraWindow::CameraWindow() : EditorWindow("Editor Camera", ImVec2(0, 0), ImVec2(0, 0)) { mIsVisible = true; }

    void CameraWindow::Update()
    {

        if (ImGui::Begin(mTitle))
        {
        }
        ImGui::End();
    }
}
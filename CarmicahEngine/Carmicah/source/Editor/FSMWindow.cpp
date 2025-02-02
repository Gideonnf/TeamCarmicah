/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			FSMWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:			This FSMWindow class is a derived class from EditorWindow.
                It currently provides the implementation of the FSMWindow to manage the FSM.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "FSMWindow.h"

namespace Carmicah
{
    FSMWindow::FSMWindow() : EditorWindow("FSM", ImVec2(900, 300), ImVec2(0, 0))
    {
        mIsVisible = true;
    }

    void FSMWindow::Update()
    {
        if (ImGui::Begin(mTitle))
        {




        }
        ImGui::End();
    }

}
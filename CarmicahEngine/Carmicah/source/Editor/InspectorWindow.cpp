/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "InspectorWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"

namespace Carmicah
{
	InspectorWindow::InspectorWindow() : EditorWindow("Inspector", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void InspectorWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
		}
		ImGui::End();
	}
}
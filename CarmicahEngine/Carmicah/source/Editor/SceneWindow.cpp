/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SceneWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This SceneWindow class is a derived class.
				It provides an interactive view where users can visualize and manipulate game objects within the scene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "SceneWindow.h"
#include "SceneToImgui.h"

namespace Carmicah
{
	SceneWindow::SceneWindow() : EditorWindow("Scene", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void SceneWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
			const float windowWidth = ImGui::GetContentRegionAvail().x;
			const float windowHeight = ImGui::GetContentRegionAvail().y;

			SceneToImgui::GetInstance()->rescale_framebuffer(windowWidth, windowHeight);
			glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->texture_id,
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + windowWidth, pos.y + windowHeight),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
		}
		ImGui::End();
	}
}
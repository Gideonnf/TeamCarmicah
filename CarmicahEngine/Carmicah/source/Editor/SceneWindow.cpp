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
#include "Input/InputSystem.h"

namespace Carmicah
{
	SceneWindow::SceneWindow() : EditorWindow("Scene", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void SceneWindow::Update()
	{
        // Render scene content here if needed
        if (ImGui::Begin(mTitle))
        {
            const float windowWidth = ImGui::GetContentRegionAvail().x;
            const float windowHeight = ImGui::GetContentRegionAvail().y;

            SceneToImgui::GetInstance()->RescaleFramebuffer(windowWidth, windowHeight);
            glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

            // Get the screen position of the Scene window's content area
            ImVec2 sceneWindowPos = ImGui::GetWindowPos();

            ImVec2 pos = ImGui::GetCursorScreenPos();

            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->texture_id,
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + windowWidth, pos.y + windowHeight),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

            //if (ImGui::Begin("Scene"))
            //{
                // Check if the mouse is hovering over the Scene window
                if (ImGui::IsWindowHovered())
                {
                    // Get the global mouse position
                    ImVec2 mousePos = ImGui::GetMousePos();

                    // Calculate mouse position relative to the Scene window's content area
                    ImVec2 relativeMousePos = { mousePos.x - pos.x, mousePos.y - pos.y };

                    // Ensure the mouse is within the bounds of the Scene content area
                    if (relativeMousePos.x >= 0 && relativeMousePos.x <= windowWidth &&
                        relativeMousePos.y >= 0 && relativeMousePos.y <= windowHeight)
                    {
                        // Scale the coordinates to 1920x1080
                        float scaledX = (relativeMousePos.x / windowWidth) * 1920.0f;
                        float scaledY = (relativeMousePos.y / windowHeight) * 1080.0f;

                        // Update the InputSystem with the relative mouse position
                        Input.SetMousePosition(scaledX, scaledY);

                        // If dragging, update the drag position within the Scene window
                        if (Input.IsDragging())
                        {
                            Input.SetDragCurrentPos({ scaledX, scaledY });
                            std::cout << "Dragging in Scene to: (" << scaledX << ", " << scaledY << ")" << std::endl;
                        }
                    }
                }
            //}
        }
 
		ImGui::End();
	}
}
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
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "../ECS/BaseSystem.h"
#include "EditorWindow.h"
#include "SceneWindow.h"
#include "../Components/Button.h"
#include "SceneToImgui.h"
#include "../Systems/SceneSystem.h"
#include "../Messaging/Message.h"
#include "Input/InputSystem.h"


namespace Carmicah
{

	bool SceneWindow::mIsPlaying = false;
	bool SceneWindow::mChangeState = false;
    bool SceneWindow::mIsPaused = false;

	SceneWindow::SceneWindow() : EditorWindow("Scene", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void SceneWindow::Update()
	{
        
		if (ImGui::Begin(mTitle))
		{
			if (!mIsPlaying)
			{
				if (ImGui::Button("Play"))
				{
					mIsPlaying = !mIsPlaying;
                    mIsPaused = false;
					mChangeState = true;
				}
			}
			else
			{
				if (ImGui::Button("Stop"))
				{
					mIsPlaying = !mIsPlaying;
					mChangeState = true;
				}
			}
            ImGui::SameLine();
            if (ImGui::Button("Pause"))
            {
                mIsPaused = !mIsPaused;
            }

			const float windowWidth =   std::clamp(ImGui::GetContentRegionAvail().x, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Width));
			const float windowHeight =  std::clamp(ImGui::GetContentRegionAvail().y, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Height));

            SceneToImgui::GetInstance()->RescaleFramebuffer(windowWidth, windowHeight);
            glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

            // get screen position of the Scene window's content area
            ImVec2 pos = ImGui::GetCursorScreenPos();

            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->texture_id,
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + windowWidth, pos.y + windowHeight),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
            
            SceneToImgui::GetInstance()->IsHovering = ImGui::IsWindowHovered();
            // check if the mouse is hovering over the Scene window
            if (ImGui::IsWindowHovered())
            {
                // get global mouse position
                ImVec2 mousePos = ImGui::GetMousePos();

                // calc mouse position relative to the Scene window's content area
                ImVec2 relativeMousePos = { mousePos.x - pos.x, mousePos.y - pos.y };

                // make sure mouse is within the bounds of the Scene content area
                if (relativeMousePos.x >= 0 && relativeMousePos.x <= windowWidth &&
                    relativeMousePos.y >= 0 && relativeMousePos.y <= windowHeight)
                {
                    // scale the coordinates to 1920x1080
                    float scaledX = (relativeMousePos.x / windowWidth) * 1920.0f;
                    float scaledY = (relativeMousePos.y / windowHeight) * 1080.0f;

                    // update InputSystem with the relative mouse position
                    Input.SetMousePosition(scaledX, scaledY);

                    // if dragging, update the drag position within the Scene window
                    if (Input.IsDragging())
                    {
                        Input.SetDragCurrentPos({ scaledX, scaledY });
                       // std::cout << "Dragging in Scene to: (" << scaledX << ", " << scaledY << ")" << std::endl;
                    }

      //              // BUTTON CHECKING, if button pressed, tell button its pressed
      //              // check if mouse click is within button bounds
      //              if (Input.IsMousePressed(MOUSE_BUTTON_LEFT))
      //              {
						//// assuming button's position and size are set (example values), need to make this dynamic
      //                  ImVec2 buttonPosition = { 960.0f, 540.0f }; // Button center
      //                  ImVec2 buttonSize = { 200.0f, 200.0f };     // Button dimensions

      //                  if (scaledX >= buttonPosition.x - buttonSize.x / 2 &&
      //                      scaledX <= buttonPosition.x + buttonSize.x / 2 &&
      //                      scaledY >= buttonPosition.y - buttonSize.y / 2 &&
      //                      scaledY <= buttonPosition.y + buttonSize.y / 2)
      //                  {
      //                      // Trigger button press/release logic
						//	Button* button = new Button();
						//	button->onPress();
      //                  }
      //              }
      //              else if (Input.IsMouseReleased(MOUSE_BUTTON_LEFT))
      //              {
						//Button* button = new Button();
						//button->onRelease();
      //              }
                }
            }
        }
 
		ImGui::End();
	}
}
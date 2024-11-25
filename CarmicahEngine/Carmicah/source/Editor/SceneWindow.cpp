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
#include "Components/Transform.h"
#include "../Components/Button.h"
#include "SceneToImgui.h"
#include "Systems/GOFactory.h"
#include "../Systems/SceneSystem.h"
#include "../Messaging/Message.h"
#include "Input/InputSystem.h"


namespace Carmicah
{

	bool SceneWindow::mIsPlaying = false;
	bool SceneWindow::mChangeState = false;
    bool SceneWindow::mIsPaused = false;
    bool SceneWindow::mIsDebug = false;

	SceneWindow::SceneWindow() : EditorWindow("Scene", ImVec2(0, 0), ImVec2(0, 0)) { mIsVisible = true; }

	void SceneWindow::Update()
	{
        
		if (ImGui::Begin(mTitle))
		{
            //ImVec2 windowPos = ImGui::GetWindowPos();
            //ImVec2 windowSize = ImGui::GetWindowSize();
            //ImVec2 windowBottomRight(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

            //// Draw the border around the window (including title bar)
            //ImColor borderColor = ImColor(255, 0, 0); // Red border (customize as needed)
            //float borderThickness = 2.0f;
            ////ImGui::InvisibleButton("Window Area",windowBottomRight);

            if (ImGui::BeginDragDropTarget())
            {
                //Area to handle anything that's dropped into the SceneWindow
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB_PAYLOAD"))
                {
                    if (payload->DataSize > 0)
                    {
                        std::string textureName = *(const std::string*)payload->Data;
                        std::cout << "Dropped Prefab: " << textureName << std::endl;

                        // Use the payload data (textureName in this case) to create prefab
                        gGOFactory->CreatePrefab(textureName);
                    }
                    else
                    {
                        std::cout << "Empty Payload" << std::endl;
                    }
                }
                else
                {
                    std::cout << "No payload accepted!" << std::endl;
                }
                ImGui::EndDragDropTarget();
            }
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
            

            if (Input.IsKeyPressed(KEY_W))
            {
                mIsDebug = !mIsDebug;
            }

			const float windowWidth =   std::clamp(ImGui::GetContentRegionAvail().x, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Width));
			const float windowHeight =  std::clamp(ImGui::GetContentRegionAvail().y, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Height));
            //ImVec2 availableWindow = ImGui::GetContentRegionAvail();

            //std::cout << availableWindow.x << "," << availableWindow.y << std::endl;

            //std::cout << windowWidth << "," << windowHeight << std::endl;

            SceneToImgui::GetInstance()->RescaleFramebuffer(windowWidth, windowHeight);
            glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

            // get screen position of the Scene window's content area
            ImVec2 pos = ImGui::GetCursorScreenPos();

            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->GetTexture(SceneToImgui::GAME_SCENE),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + windowWidth, pos.y + windowHeight),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
                
            
            SceneToImgui::GetInstance()->IsHovering = ImGui::IsWindowHovered();
            // check if the mouse is hovering over the Scene window
            if (ImGui::IsWindowHovered())
            {

                    //Mouse Position Handling for Object Picking/Dragging
                    ImVec2 mousePos = ImGui::GetMousePos();
                    GameObject camera;
                    gGOFactory->FetchGO("MainCamera", camera);

                    // calc mouse position relative to the Scene window's content area
                    ImVec2 relativeMousePos = { mousePos.x - pos.x, mousePos.y - pos.y };

                    //std::cout << relativeMousePos.x << "," << relativeMousePos.y << std::endl;


                    // make sure mouse is within the bounds of the Scene content area
                    if (relativeMousePos.x >= 0 && relativeMousePos.x <= windowWidth &&
                        relativeMousePos.y >= 0 && relativeMousePos.y <= windowHeight)
                    {
                        // scale the coordinates to 1920x1080
                        double scaledX = (relativeMousePos.x / windowWidth) * 1920.0f;
                        double scaledY = (relativeMousePos.y / windowHeight) * 1080.0f;

                        static double worldDeltaX = 0.f;
                        static double worldDeltaY = 0.f;


                        //std::cout << "World Pos = " << worldX << "," << worldY << std::endl;

                        // update InputSystem with the relative mouse position
                        Input.SetMousePosition(scaledX, scaledY);


                        // if dragging, update the drag position within the Scene window
                        if (Input.IsDragging() && camera.HasComponent<Transform>())
                        {
                            Input.SetDragCurrentPos({ scaledX, scaledY });

                            Vec2d startDragPos = Input.GetDragStartPos();
                            Vec2d currentMousePos = Input.GetDragCurrentPos();
                            /*if(mIsDebug)
                            {
                                std::cout << "Start Pos: " << startDragPos << std::endl;
                                std::cout << "Current Pos: " << currentMousePos << std::endl;
                            }*/
                            Vec2d delta(currentMousePos.x - startDragPos.x, currentMousePos.y - startDragPos.y);

                            Transform& cameraTransform = camera.GetComponent<Transform>();

                            double worldDeltaX = ((delta.x / windowWidth /*NEED TO CHANGE THIS TO FIX IT*/)) / cameraTransform.GetScale().x;
                            double worldDeltaY = -((delta.y / windowHeight/*NEED TO CHANGE THIS TO FIX IT*/)) / cameraTransform.GetScale().y;

                            if (mIsDebug)
                            {
                                std::cout << "Window Size  = " << windowWidth << "," << windowHeight << std::endl;
                                std::cout << "Delta = " << delta.x << "," << delta.y << std::endl;
                                std::cout << "World Delta = " << worldDeltaX << "," << worldDeltaY << std::endl;
                            }
                            Input.SetDragStartPos(currentMousePos);



                            if (HierarchyWindow::selectedGO != nullptr)
                            {
                                if (HierarchyWindow::selectedGO->HasComponent<Transform>())
                                {
                                    Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();

                                    selectedTransform.GetPos().x += worldDeltaX;
                                    selectedTransform.GetPos().y += worldDeltaY;

                                }
                            }

                        }
                    }
            }
        }
 
		ImGui::End();
	}
}
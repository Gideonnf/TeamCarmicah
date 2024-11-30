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
#include "../Systems/SoundSystem.h"
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
            const float windowWidth = std::clamp(ImGui::GetContentRegionAvail().x, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Width));
            const float windowHeight = std::clamp(ImGui::GetContentRegionAvail().y, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Height));
            ImVec2 windowSize(windowWidth, windowHeight);
            //ImVec2 windowBottomRight(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

            //// Draw the border around the window (including title bar)
            //ImColor borderColor = ImColor(255, 0, 0); // Red border (customize as needed)
            //float borderThickness = 2.0f;
            ////ImGui::InvisibleButton("Window Area",windowBottomRight);

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

            if (!mIsPaused)
            {
                if (ImGui::Button("Pause"))
                {
                    mIsPaused = !mIsPaused;
                    auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
                    souSystem->PauseAllSounds();
                }
            }
            else
            {
                if (ImGui::Button("Unpause"))
                {
                    mIsPaused = !mIsPaused;
                    auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
                    souSystem->ResumeAllSounds();

                }
            }




            /*if (Input.IsKeyPressed(KEY_W))
            {
                mIsDebug = !mIsDebug;
            }*/

            //ImVec2 availableWindow = ImGui::GetContentRegionAvail();

            //std::cout << availableWindow.x << "," << availableWindow.y << std::endl;

            //std::cout << windowWidth << "," << windowHeight << std::endl;

            //SceneToImgui::GetInstance()->RescaleFramebuffer(windowWidth, windowHeight);
            glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

            // get screen position of the Scene window's content area
            ImVec2 pos = ImGui::GetCursorScreenPos();

            /*if (Input.IsKeyPressed(KEY_W))
            {
                std::cout << "Window Size: " << windowWidth << "," << windowHeight << std::endl;
                std::cout << "Position of Image Top Left: " << pos.x << "," << pos.y << std::endl;
                std::cout << "Position of Image Bot Right: " << pos.x + windowWidth << "," << pos.y + windowHeight << std::endl;
            }*/

            ImGui::GetWindowDrawList()->AddImage(
                (ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->GetTexture(SceneToImgui::GAME_SCENE),
                ImVec2(pos.x, pos.y),
                ImVec2(pos.x + windowWidth, pos.y + windowHeight),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
            ImGui::Dummy(windowSize);
            if (ImGui::BeginDragDropTarget())
            {
                //Area to handle anything that's dropped into the SceneWindow
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB_PAYLOAD"))
                {

                    std::string textureName = *(const std::string*)payload->Data;
                    gGOFactory->CreatePrefab(textureName);
                }
                ImGui::EndDragDropTarget();
            }

            SceneToImgui::GetInstance()->SetHovering(SceneToImgui::GAME_SCENE, ImGui::IsWindowHovered());
            // SceneToImgui::GetInstance()->IsHovering = ImGui::IsWindowHovered();
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


                        Input.SetDragStartPos(currentMousePos);

                        if (HierarchyWindow::selectedGO != nullptr)
                        {
                            CM_CORE_INFO("Selected entity : " + std::to_string(HierarchyWindow::selectedGO->GetID()))
                        }

                    }
                }
            }
        }

        ImGui::End();
    }
}
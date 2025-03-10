/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:          CameraWindow.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:         Handles getting input from imGUI side when in the editor window

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
#include "ECS/ComponentManager.h"
#include "EditorWindow.h"
#include "CameraWindow.h"
#include "SceneToImgui.h"
#include "Systems/GOFactory.h"
#include "Input/InputSystem.h"
#include "Graphics/GraphicsAssetTypes.h"
#include "Graphics/RenderHelper.h"


namespace Carmicah
{

	CameraWindow::CameraWindow() : EditorWindow("Editor Camera", ImVec2(0, 0), ImVec2(0, 0)), mOriginalAngle{}, mGizmosID{} { mIsVisible = true; }

    void CameraWindow::Update()
    {
		if (Input.IsKeyPressed(KEY_Q))
			RenderHelper::GetInstance()->mEditorMode = GIZMOS_MODE::GIZMOS_NONE;
		else if (Input.IsKeyPressed(KEY_W))
			RenderHelper::GetInstance()->mEditorMode = GIZMOS_MODE::GIZMOS_TRANSLATE;
		else if (Input.IsKeyPressed(KEY_E))
			RenderHelper::GetInstance()->mEditorMode = GIZMOS_MODE::GIZMOS_SCALE;
		else if (Input.IsKeyPressed(KEY_R))
			RenderHelper::GetInstance()->mEditorMode = GIZMOS_MODE::GIZMOS_ROTATE;


		bool camWindowActive = ImGui::Begin(mTitle);
		RenderHelper::GetInstance()->mEditorWindowActive = camWindowActive;
        if (camWindowActive)
        {
			// Rendering
			const float windowWidth = std::clamp(ImGui::GetContentRegionAvail().x, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Width));
			const float windowHeight = std::clamp(ImGui::GetContentRegionAvail().y, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Height));
			RenderHelper::GetInstance()->mEditorWindomDim = Vec2f(windowWidth, windowHeight);
			glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->GetTexture(SceneToImgui::EDITOR_SCENE),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + windowWidth, pos.y + windowHeight),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			// Calc movement
			SceneToImgui::GetInstance()->SetHovering(SceneToImgui::EDITOR_SCENE, ImGui::IsWindowHovered());
			if (ImGui::IsWindowHovered())
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				ImVec2 relativeMousePos = { mousePos.x - pos.x, mousePos.y - pos.y };

				if (relativeMousePos.x >= 0 && relativeMousePos.x <= windowWidth &&
					relativeMousePos.y >= 0 && relativeMousePos.y <= windowHeight)
				{
					double scaledX = (relativeMousePos.x / windowWidth) * AssetManager::GetInstance()->enConfig.Width;
					double scaledY = (relativeMousePos.y / windowHeight) * AssetManager::GetInstance()->enConfig.Height;
					Vec2d currentMousePos{ scaledX, scaledY };

					if (Input.IsMousePressed(MOUSE_BUTTON_LEFT) && HierarchyWindow::selectedGO != nullptr)
					{
						mStartClickPos = Vec2f{ static_cast<float>(scaledX), static_cast<float>(scaledY) };
						if (HierarchyWindow::selectedGO->HasComponent<Transform>())
						{
							Transform& cam = RenderHelper::GetInstance()->mEditorCam;
							Mtx3x3f camSpace{};
							camSpace.scaleThis(cam.Scale()).rotDegThis(cam.Rot()).translateThis(-cam.Pos());
							Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();
							Vec2f selectedTransformPos{ selectedTransform.worldSpace.m20, selectedTransform.worldSpace.m21 };

							mSelectedObjPos = camSpace * selectedTransformPos;
							mSelectedObjPos.x = (mSelectedObjPos.x + 1.f) * AssetManager::GetInstance()->enConfig.Width * 0.5f;
							mSelectedObjPos.y = (1.f - mSelectedObjPos.y) * AssetManager::GetInstance()->enConfig.Height * 0.5f;
							mOriginalAngle = selectedTransform.Rot();
						}
						else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
						{
							UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();

							mSelectedObjPos = Vec2f(selectedTransform.worldSpace.m20, AssetManager::GetInstance()->enConfig.Height - selectedTransform.worldSpace.m21);
							mOriginalAngle = selectedTransform.Rot();
						}
						mGizmosID = RenderHelper::GetInstance()->mSelectedID;
					}

					if (Input.IsMouseHold(MOUSE_BUTTON_LEFT) && HierarchyWindow::selectedGO != nullptr)
					{
						Vec2d prevMousePos = Input.GetMousePosition();// Previous frame's pos
						Vec2d delta(currentMousePos.x - prevMousePos.x, currentMousePos.y - prevMousePos.y);

						if (std::abs(delta.x) > DBL_EPSILON || std::abs(delta.y) > DBL_EPSILON)
						{
							switch (RenderHelper::GetInstance()->mEditorMode)
							{
							case GIZMOS_MODE::GIZMOS_NONE:
							{
								if (HierarchyWindow::selectedGO->HasComponent<Transform>())
								{
									Transform& camTrans = RenderHelper::GetInstance()->mEditorCam;
									float worldDeltaX = static_cast<float>(((delta.x / AssetManager::GetInstance()->enConfig.Width * 2.0))) / camTrans.GetScale().x;
									float worldDeltaY = -static_cast<float>((delta.y / AssetManager::GetInstance()->enConfig.Height * 2.0)) / camTrans.GetScale().y;
									Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();

									if(selectedTransform.ParentID() == 0)
										selectedTransform.GetPos() += Vec2f(worldDeltaX, worldDeltaY);
									else
									{
										Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(selectedTransform.ParentID());
										selectedTransform.GetPos() += Vec2f(
											worldDeltaX * parentTransform.rotTrans.m[0] + worldDeltaY * parentTransform.rotTrans.m[1],
											worldDeltaX * parentTransform.rotTrans.m[3] + worldDeltaY * parentTransform.rotTrans.m[4]
											);
									}
								}
								else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
								{
									UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
									if (selectedTransform.ParentID() == 0)
										selectedTransform.GetPos() += Vec2f(static_cast<float>(delta.x), static_cast<float>(-delta.y));
									else
									{
										UITransform& parentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(selectedTransform.ParentID());
										selectedTransform.GetPos() += Vec2f(
											static_cast<float>(delta.x) * parentTransform.rotTrans.m[0] + static_cast<float>(-delta.y) * parentTransform.rotTrans.m[1],
											static_cast<float>(delta.x) * parentTransform.rotTrans.m[3] + static_cast<float>(-delta.y) * parentTransform.rotTrans.m[4]
										);
									}
								}
								break;
							}
							case GIZMOS_MODE::GIZMOS_ROTATE:
							{
								if (mGizmosID == std::numeric_limits<unsigned int>().max())
								{
									Vec2f vec1 = mStartClickPos - mSelectedObjPos;
									Vec2f vec2{ static_cast<float>(currentMousePos.x) - mSelectedObjPos.x,
										static_cast<float>(currentMousePos.y) - mSelectedObjPos.y };

									float angl{(atan2f(vec1.dot(vec2), vec1.x * vec2.y - vec1.y * vec2.x) - PI * 0.5f) * 180.f / PI };
									
									if (HierarchyWindow::selectedGO->HasComponent<Transform>())
									{
										Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();
										selectedTransform.Rot(mOriginalAngle + angl);
									}
									else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
									{
										UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
										selectedTransform.Rot(mOriginalAngle + angl);
									}

								}
								break;
							}
							case GIZMOS_MODE::GIZMOS_TRANSLATE:
							{
								if (mGizmosID == std::numeric_limits<unsigned int>().max())
								{
									if (HierarchyWindow::selectedGO->HasComponent<Transform>())
									{
										Transform& camTrans = RenderHelper::GetInstance()->mEditorCam;
										double worldDeltaY = -((delta.y / AssetManager::GetInstance()->enConfig.Height * 2.0)) / camTrans.GetScale().y;
										Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();

										selectedTransform.PosYAdd(static_cast<float>(worldDeltaY));
									}
									else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
									{
										UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
										selectedTransform.PosYAdd(static_cast<float>(-delta.y));
									}
								}
								else if (mGizmosID == std::numeric_limits<unsigned int>().max() - 1)
								{
									if (HierarchyWindow::selectedGO->HasComponent<Transform>())
									{
										Transform& camTrans = RenderHelper::GetInstance()->mEditorCam;
										double worldDeltaX = ((delta.x / AssetManager::GetInstance()->enConfig.Width * 2.0)) / camTrans.GetScale().x;
										Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();

										selectedTransform.PosXAdd(static_cast<float>(worldDeltaX));
									}
									else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
									{
										UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
										selectedTransform.PosXAdd(static_cast<float>(delta.x));
									}
								}
								break;
							}
							case GIZMOS_MODE::GIZMOS_SCALE:
							{
								if (mGizmosID == std::numeric_limits<unsigned int>().max())
								{
									if (HierarchyWindow::selectedGO->HasComponent<Transform>())
									{
										Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();
										selectedTransform.ScaleYAdd(static_cast<float>(-delta.y) * 0.1f);
									}
									else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
									{
										UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
										selectedTransform.ScaleYAdd(static_cast<float>(-delta.y));
									}
								}
								else if (mGizmosID == std::numeric_limits<unsigned int>().max() - 1)
								{
									if (HierarchyWindow::selectedGO->HasComponent<Transform>())
									{
										Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();
										selectedTransform.ScaleXAdd(static_cast<float>(delta.x) * 0.1f);
									}
									else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
									{
										UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
										selectedTransform.ScaleXAdd(static_cast<float>(delta.x));
									}
								}
								break;
							}
							}
						}
					}
					// End
					Input.SetMousePosition(scaledX, scaledY);
				}
			}
        }
        ImGui::End();
    }
}

#include "pch.h"
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "../ECS/BaseSystem.h"
#include "EditorWindow.h"
#include "CameraWindow.h"
#include "SceneToImgui.h"
#include "Systems/GOFactory.h"
#include "Input/InputSystem.h"
#include "Graphics/RenderHelper.h"


namespace Carmicah
{

    CameraWindow::CameraWindow() : EditorWindow("Editor Camera", ImVec2(0, 0), ImVec2(0, 0)) { mIsVisible = true; }

    void CameraWindow::Update()
    {
		if (Input.GetInstance()->IsKeyPressed(KEY_Q))
			RenderHelper::GetInstance()->mEditorMode = RenderHelper::GIZMOS_MODE::GIZMOS_NONE;
		else if (Input.GetInstance()->IsKeyPressed(KEY_W))
			RenderHelper::GetInstance()->mEditorMode = RenderHelper::GIZMOS_MODE::GIZMOS_TRANSLATE;
		else if (Input.GetInstance()->IsKeyPressed(KEY_E))
			RenderHelper::GetInstance()->mEditorMode = RenderHelper::GIZMOS_MODE::GIZMOS_SCALE;
		else if (Input.GetInstance()->IsKeyPressed(KEY_R))
			RenderHelper::GetInstance()->mEditorMode = RenderHelper::GIZMOS_MODE::GIZMOS_ROTATE;

		bool camWindowActive = ImGui::Begin(mTitle);
		RenderHelper::GetInstance()->mEditorWindowActive = camWindowActive;
        if (camWindowActive)
        {
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

					if (Input.IsMouseHold(MOUSE_BUTTON_LEFT))
					{
						Vec2d prevMousePos = Input.GetMousePosition();// Previous frame's pos

						Vec2d delta(currentMousePos.x - prevMousePos.x, currentMousePos.y - prevMousePos.y);

						if (HierarchyWindow::selectedGO != nullptr)
						{
							if (HierarchyWindow::selectedGO->HasComponent<Transform>())
							{
								Transform& camTrans = RenderHelper::GetInstance()->mEditorCam;
								double worldDeltaX = ((delta.x / AssetManager::GetInstance()->enConfig.Width * 2.0)) / camTrans.GetScale().x;
								double worldDeltaY = -((delta.y / AssetManager::GetInstance()->enConfig.Height * 2.0)) / camTrans.GetScale().y;
								Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();

								selectedTransform.PosXAdd(worldDeltaX);
								selectedTransform.PosYAdd(worldDeltaY);
							}
							else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
							{
								UITransform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
								selectedTransform.PosXAdd(delta.x);
								selectedTransform.PosYAdd(-delta.y);
							}
						}

					}
					Input.SetMousePosition(scaledX, scaledY);
				}

			}
        }
        ImGui::End();
    }
}
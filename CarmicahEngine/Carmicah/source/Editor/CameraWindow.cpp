
#include "pch.h"
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "../ECS/BaseSystem.h"
#include "EditorWindow.h"
#include "CameraWindow.h"
#include "SceneToImgui.h"


namespace Carmicah
{

    CameraWindow::CameraWindow() : EditorWindow("Editor Camera", ImVec2(0, 0), ImVec2(0, 0)) { mIsVisible = true; }

    void CameraWindow::Update()
    {

        if (ImGui::Begin(mTitle))
        {
			const float windowWidth = std::clamp(ImGui::GetContentRegionAvail().x, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Width));
			const float windowHeight = std::clamp(ImGui::GetContentRegionAvail().y, 0.f, static_cast<float>(AssetManager::GetInstance()->enConfig.Height));
			glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddImage(
				(ImTextureID)(uintptr_t)SceneToImgui::GetInstance()->GetTexture(SceneToImgui::EDITOR_SCENE),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + windowWidth, pos.y + windowHeight),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
        }
        ImGui::End();
    }
}
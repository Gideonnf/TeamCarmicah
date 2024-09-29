#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/HeirarchyWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"

namespace Carmicah
{
	HeirarchyWindow::HeirarchyWindow() : EditorWindow("Heirarchy", ImVec2(900, 300), ImVec2(0, 0)) { isVisible = true; }

	void HeirarchyWindow::Update()
	{
		if (ImGui::Begin(title))
		{
			static Transform playerTrans{ 0.5f, 0.5f, 1.f, 45.f, 1.f, 1.f };
			static Collider2D playerCollider{ 1, 2, 3, 4 };
			static Renderer toRender{};
			static char goName[256] = "";
			ImGui::Text("Game Object Name: ");
			ImGui::SameLine();
			ImGui::InputText("##GameObjectNameInput", goName, IM_ARRAYSIZE(goName));

			if (ImGui::BeginTable("Transform Table", 2, ImGuiTableFlags_Borders))
			{
				//Column Headers
				ImGui::TableNextColumn();
				ImGui::Text("Attribute");
				ImGui::TableNextColumn();
				ImGui::Text("Value");

				//Position (X,Y,Z)
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("xPos");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##xPos", &playerTrans.xPos);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yPos");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##yPos", &playerTrans.yPos);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("zPos");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##zPos", &playerTrans.zPos);

				// Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##rot", &playerTrans.rot);

				// Scale (xScale, yScale)
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("xScale");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##xScale", &playerTrans.xScale);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				ImGui::InputFloat("##yScale", &playerTrans.yScale);

			}
			ImGui::EndTable();

			if (ImGui::Button("Create Game Object"))
			{
				static std::string name(goName);
				GameObject newObj = gGOFactory->CreateGO(goName);
				newObj.AddComponent<Transform>(playerTrans);
				newObj.AddComponent<Collider2D>(playerCollider);
				newObj.AddComponent<Renderer>(toRender);
			}
		}
		ImGui::End();
	}
}
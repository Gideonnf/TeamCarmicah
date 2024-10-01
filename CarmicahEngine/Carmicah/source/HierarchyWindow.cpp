#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "Editor/EditorWindow.h"
#include "Editor/HierarchyWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"

namespace Carmicah
{
	HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void HierarchyWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
			static Transform playerTrans{};
			//static Collider2D playerCollider{ 1.0, 2.0, 3.0, 4.0 };
			static Renderer toRender{};
			static char goName[256] = "Duck";
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
				GameObject newObj = gGOFactory->CreatePrefab(goName);
				newObj.GetComponent<Transform>().xPos += 2.0;
				//newObj.AddComponent<Transform>(playerTrans);
				////newObj.AddComponent<Collider2D>(playerCollider);
				//newObj.AddComponent<Renderer>(toRender);
			}
		}
		ImGui::End();
	}
}
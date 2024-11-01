/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"

namespace Carmicah
{
	InspectorWindow::InspectorWindow() : EditorWindow("Inspector", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void InspectorWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
			if (HierarchyWindow::selectedGO != nullptr)
			{
				ImGui::Text("Selected Game Object: %s", HierarchyWindow::selectedGO->GetName().c_str());
				Entity selectedEntity = HierarchyWindow::selectedGO->GetID();
				if (HierarchyWindow::selectedGO->HasComponent<Transform>())
				{
					Transform& selectedTransform = HierarchyWindow::selectedGO->GetComponent<Transform>();
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
						ImGui::DragFloat("##xPos", &selectedTransform.pos.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yPos");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##yPos", &selectedTransform.pos.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("zPos");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##zPos", &selectedTransform.depth, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						// Rotation
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Rotation");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##rot", &selectedTransform.rot, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						// Scale (xScale, yScale)
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("xScale");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##xScale", &selectedTransform.scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yScale");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##yScale", &selectedTransform.scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
					}
					ImGui::EndTable();
				}
				else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
				{
					UITransform& selectedUITransform = HierarchyWindow::selectedGO->GetComponent<UITransform>();
					if (ImGui::BeginTable("UI Transform Table", 2, ImGuiTableFlags_Borders))
					{
						//Column Headers
						ImGui::TableNextColumn();
						ImGui::Text("Attribute");
						ImGui::TableNextColumn();
						ImGui::Text("Value");

						// Position x and y
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("xPos");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##xPos", &selectedUITransform.pos.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yPos");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##yPos", &selectedUITransform.pos.y,0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						// Scale (xScale, yScale)
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("xScale");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##xScale", &selectedUITransform.scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yScale");
						ImGui::TableNextColumn();
						ImGui::DragFloat("##yScale", &selectedUITransform.scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

						//ImGui::TableNextRow();
						//ImGui::TableNextColumn();
						//ImGui::Text("Update Object");
						//ImGui::TableNextColumn();
						//std::string UpdateGO = "Update " + selectedGO->GetName();
						//if (ImGui::Button(UpdateGO.c_str()))
						//{
						//	
						//	//gGOFactory->Destroy(selectedEntity);
						//	//selectedGO = nullptr;
						//}
					}
					ImGui::EndTable();
				}

				if (HierarchyWindow::selectedGO->HasComponent<Animation>())
				{
					std::string animGO = "Change Animation of " + HierarchyWindow::selectedGO->GetName();
					if (ImGui::Button(animGO.c_str()))
					{
						HierarchyWindow::selectedGO->GetComponent<Animation>().notChangedAnim = true;
						HierarchyWindow::selectedGO->GetComponent<Renderer>().texture = "Duck";
						//gGOFactory->Destroy(selectedEntity);
						//selectedGO = nullptr;
					}
				}

				std::string destroyGO = "Destroy " + HierarchyWindow::selectedGO->GetName();
				if (ImGui::Button(destroyGO.c_str()))
				{
					gGOFactory->Destroy(selectedEntity);
					HierarchyWindow::selectedGO = nullptr;
				}
			}
		}
		ImGui::End();
	}
}
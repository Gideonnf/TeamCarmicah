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

	void InspectorWindow::GOTable(GameObject* go)
	{
		if (go->HasComponent<Transform>())
		{
			Transform& selectedTransform = go->GetComponent<Transform>();
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
		else if (go->HasComponent<UITransform>())
		{
			UITransform& selectedUITransform = go->GetComponent<UITransform>();
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
				ImGui::DragFloat("##yPos", &selectedUITransform.pos.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

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

		if (go->HasComponent<Animation>())
		{
			std::string animGO = "Change Animation of " + go->GetName();
			if (ImGui::Button(animGO.c_str()))
			{
				go->GetComponent<Animation>().notChangedAnim = true;
				go->GetComponent<Renderer>().texture = "Duck";
				//gGOFactory->Destroy(selectedEntity);
				//selectedGO = nullptr;
			}
		}
		
		// render rigibody data
		if (go->HasComponent<RigidBody>())
		{
			RigidBody& rb = go->GetComponent<RigidBody>();
			if (ImGui::BeginTable("Rigidbody Table", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Velocity X");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##VelocityX", &rb.velocity.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Velocity Y");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##VelocityY", &rb.velocity.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Gravity");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##Gravity", &rb.gravity, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::EndTable();
			}
		}

		// render collider data
		if (go->HasComponent<Collider2D>())
		{
			Collider2D& col = go->GetComponent<Collider2D>();
			if (ImGui::BeginTable("Collider2D Table", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Shape");
				ImGui::TableNextColumn();
				ImGui::Text("%s", col.shape.c_str());

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Min X");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##MinX", &col.min.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Min Y");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##MinY", &col.min.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Max X");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##MaxX", &col.max.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Max Y");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##MaxY", &col.max.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::EndTable();
			}
		}

		if (go->HasComponent<TextRenderer>())
		{
			TextRenderer& text = go->GetComponent<TextRenderer>();
			if (ImGui::BeginTable("TextRenderer Table", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Text");

				// Convert std::string to char array buffer
				char buffer[256];
				strncpy(buffer, text.txt.c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

				ImGui::TableNextColumn();
				if (ImGui::InputText("##Text", buffer, sizeof(buffer)))
				{
					// Update the std::string if the user changes the input
					text.txt = buffer;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Font");
				ImGui::TableNextColumn();
				ImGui::Text("%s", text.font.c_str());

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Color R");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##ColorR", &text.colorR, 0.01f, 0.0f, 1.0f, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Color G");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##ColorG", &text.colorG, 0.01f, 0.0f, 1.0f, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Color B");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##ColorB", &text.colorB, 0.01f, 0.0f, 1.0f, "%.3f");

				ImGui::EndTable();
			}
		}

	}

	void InspectorWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
			if (HierarchyWindow::selectedGO != nullptr)
			{
				ImGui::Text("Selected Game Object: %s", HierarchyWindow::selectedGO->GetName().c_str());
				Entity selectedEntity = HierarchyWindow::selectedGO->GetID();

				GOTable(HierarchyWindow::selectedGO);

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
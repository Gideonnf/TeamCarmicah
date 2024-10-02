/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			HierarchyWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This HierarchyWindow class is a derived class from EditorWindow.
				It currently provides an list of GameObjects, and has a button that allows for the creation/destruction of new GameObjects.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "HierarchyWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include <random>

namespace Carmicah
{
	HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }
	std::vector<GameObject> createdList;

	void HierarchyWindow::Update()
	{
			static Transform playerTrans{};
			static GameObject* selectedGO;
			//static Collider2D playerCollider{ 1.0, 2.0, 3.0, 4.0 };
			//static Renderer toRender{};
		if (ImGui::Begin(mTitle))
		{
			if(ImGui::BeginChild("Game Object List: ", ImVec2(900,300)))
			{
				gGOFactory->ForAllGO([](GameObject& go)
					{
						if (ImGui::Button(go.GetName().c_str()))
						{
							selectedGO = &go;
						}
					});
				ImGui::EndChild();
			}
			if (selectedGO != nullptr)
			{
				ImGui::Text("Selected Game Object: %s", selectedGO->GetName().c_str());
				Entity selectedEntity = selectedGO->GetID();
				Transform& selectedTransform = selectedGO->GetComponent<Transform>();
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
					ImGui::InputFloat("##xPos", &selectedTransform.xPos);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yPos");
					ImGui::TableNextColumn();
					ImGui::InputFloat("##yPos", &selectedTransform.yPos);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("zPos");
					ImGui::TableNextColumn();
					ImGui::InputFloat("##zPos", &selectedTransform.zPos);

					// Rotation
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Rotation");
					ImGui::TableNextColumn();
					ImGui::InputFloat("##rot",  &selectedTransform.rot);

					// Scale (xScale, yScale)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xScale");
					ImGui::TableNextColumn();
					ImGui::InputFloat("##xScale", &selectedTransform.xScale);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yScale");
					ImGui::TableNextColumn();
					ImGui::InputFloat("##yScale", &selectedTransform.yScale);

				}
				ImGui::EndTable();
				std::string destroyGO = "Destroy " + selectedGO->GetName();
				if(ImGui::Button(destroyGO.c_str()))
				{
					gGOFactory->Destroy(selectedEntity);
					selectedGO = nullptr;
				}
			}

			static char goName[256] = "Duck";
			ImGui::Text("Game Object Name: ");
			ImGui::SameLine();
			ImGui::Text(goName); //Cannot be edited for now.

			//ImGui::InputText("##GameObjectNameInput", goName, IM_ARRAYSIZE(goName));


			if (ImGui::Button("Create Game Object"))
			{
				//static std::string name(goName);
				GameObject newObj = gGOFactory->CreatePrefab(goName);
				newObj.GetComponent<Transform>().xPos += 2.0;
				//newObj.AddComponent<Transform>(playerTrans);
				////newObj.AddComponent<Collider2D>(playerCollider);
				//newObj.AddComponent<Renderer>(toRender);
			}

			if (ImGui::Button("Create 2500 Game Objects"))
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> distrib(-10.0f, 10.0f);
				std::uniform_real_distribution<float> rotDistrib(-360.f, 360.f);
				for (int i = 0; i < 2500; ++i)
				{
					GameObject newObj = gGOFactory->CreatePrefab(goName);
					float random_xPos = distrib(gen);
					float random_yPos = distrib(gen);
					float random_rot = rotDistrib(gen);

					newObj.GetComponent<Transform>().xPos = random_xPos;
					newObj.GetComponent<Transform>().yPos = random_yPos;
					newObj.GetComponent<Transform>().rot = random_rot;

					createdList.push_back(newObj);
				}
			}

			if (ImGui::Button("Destroy all Game Objects"))
			{
				for (auto& obj : createdList)
				{
					obj.Destroy();
					//gGOFactory->Destroy(obj.GetID());
				}

				createdList.clear();
				//gGOFactory->DestroyAll();
			}
		}
		ImGui::End();
	}
}
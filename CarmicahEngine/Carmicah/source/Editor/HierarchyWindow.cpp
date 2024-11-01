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
#include "Components/UITransform.h"
#include "SceneToImgui.h"
#include <random>

namespace Carmicah
{
	HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }
	std::vector<GameObject> createdList;
	GameObject* HierarchyWindow::selectedGO = nullptr;

	void HierarchyWindow::Update()
	{
			//static Transform playerTrans{};
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
				if (selectedGO->HasComponent<Transform>())
				{
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
						ImGui::DragFloat("##xPos", &selectedTransform.pos.x,0.05f,-FLT_MAX,FLT_MAX,"%.3f");

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
				else if (selectedGO->HasComponent<UITransform>())
				{
					UITransform& selectedUITransform = selectedGO->GetComponent<UITransform>();
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
						ImGui::InputFloat("##xPos", &selectedUITransform.pos.x);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yPos");
						ImGui::TableNextColumn();
						ImGui::InputFloat("##yPos", &selectedUITransform.pos.y);

						// Scale (xScale, yScale)
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("xScale");
						ImGui::TableNextColumn();
						ImGui::InputFloat("##xScale", &selectedUITransform.scale.x);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("yScale");
						ImGui::TableNextColumn();
						ImGui::InputFloat("##yScale", &selectedUITransform.scale.y);

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
				
				if (selectedGO->HasComponent<Animation>())
				{
					std::string animGO = "Change Animation of " + selectedGO->GetName();
					if (ImGui::Button(animGO.c_str()))
					{
						selectedGO->GetComponent<Animation>().notChangedAnim = true;
						selectedGO->GetComponent<Renderer>().texture = "Duck";
						//gGOFactory->Destroy(selectedEntity);
						//selectedGO = nullptr;
					}
				}

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
			ImGui::Text(goName); //Cannot be edited for now

			if (ImGui::Button("Create Game Object"))
			{
				//static std::string name(goName);
				GameObject newObj = gGOFactory->CreatePrefab(goName);
				newObj.GetComponent<Transform>().pos.x += 2.0;
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

					newObj.GetComponent<Transform>().pos.x = random_xPos;
					newObj.GetComponent<Transform>().pos.y = random_yPos;
					newObj.GetComponent<Transform>().rot = random_rot;

					createdList.push_back(newObj);
				}
			}

			if (ImGui::Button("Destroy all 2500 Game Objects"))
			{
				for (auto& obj : createdList)
				{
					if (selectedGO != nullptr && obj.GetID() == selectedGO->GetID())
					{
						selectedGO = nullptr;
					}
					obj.Destroy();
					//gGOFactory->Destroy(obj.GetID());
				}

				createdList.clear();
				//gGOFactory->DestroyAll();
			}

			std::string buttonName = "Save current scene: " + SceneToImgui::GetInstance()->currentScene;
			if (ImGui::Button(buttonName.c_str()))
			{
				std::string sceneFile;
				AssetManager::GetInstance()->GetScene(SceneToImgui::GetInstance()->currentScene, sceneFile);
				SerializerSystem::GetInstance()->SerializeScene(sceneFile);
				//gGOFactory->DestroyAll();
			}
		}
		ImGui::End();
	}

	void HierarchyWindow::EntityDestroyed(Entity id)
	{
		if (selectedGO != nullptr && id == selectedGO->GetID())
		{
			selectedGO = nullptr;
		}
	}
}
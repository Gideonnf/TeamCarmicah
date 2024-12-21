/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			HierarchyWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This HierarchyWindow class is a derived class from EditorWindow.
				It currently provides an list of GameObjects, and has a button that allows for the creation of new GameObjects.
				It also has a button to allow for saving of the scene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "AssetWindow.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"
#include "SceneToImgui.h"
#include <random>


namespace Carmicah
{
	HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy", ImVec2(0, 0), ImVec2(0, 0)) { mIsVisible = true; }
	bool HierarchyWindow::mShowScene = true;
	GameObject* HierarchyWindow::selectedGO = nullptr;
	Prefab* HierarchyWindow::inspectedPrefab = nullptr;

	std::vector<GameObject> createdList;

	void HierarchyWindow::DrawCustomSeparator(GameObject& go)
	{
		// Calculate the size of the separator based on text length
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImVec2 textSize = ImGui::CalcTextSize(go.GetName().c_str());
		ImVec2 separatorSize = ImVec2(textSize.x, 2.0f); // 2.0f for line thickness
		std::string buttonText = "Separator_" + std::to_string(go.GetID());
		// Create an invisible button for interaction
		if (ImGui::InvisibleButton(buttonText.c_str(), ImVec2(separatorSize.x, separatorSize.y + 2.0f)))
		{
			//Clicking will do nothing
		}

		// Draw the line visually
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(
			ImVec2(cursorPos.x, cursorPos.y + separatorSize.y),
			ImVec2(cursorPos.x + separatorSize.x, cursorPos.y + separatorSize.y),
			ImGui::GetColorU32(ImGuiCol_Separator), 1.0f);
	}

	void HierarchyWindow::GOButton(GameObject& go)
	{
		bool hasChildren = false;
		//CHECK THIS AGAIN LATER
		gGOFactory->ForGOChildren(go, [&hasChildren](GameObject&) 
		{
			hasChildren = true;
		});

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

		if (!hasChildren)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		DrawCustomSeparator(go);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				GameObject& droppedGO = *(GameObject*)payload->Data;

				if (droppedGO.GetID() == go.GetID())
				{
					CM_CORE_WARN("Not swapping any hierarchy");
				}

				else
				{
					auto first = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
					auto second = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), go.GetID());
					std::swap(*first, *second);
					CM_CORE_INFO("Swap success");
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::TreeNodeEx(go.GetName().c_str(), flags))
		{
			if (ImGui::IsItemClicked())
			{
				selectedGO = &go;
			}

			//Source and Target Logic

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("GAMEOBJECT", &go, sizeof(GameObject));
				ImGui::Text("Dragging %s", go.GetName().c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
				{
					GameObject& droppedGO = *(GameObject*)payload->Data;
					
					droppedGO.SetParent(go);
					
				}
				ImGui::EndDragDropTarget();
			}



			// Check if go has child
			gGOFactory->ForGOChildren(go, [this](GameObject& childGo)
				{
					GOButton(childGo);
				});

			ImGui::TreePop();
		}
	}

	void HierarchyWindow::PrefabButton(Prefab& prefab)
	{
		bool hasChildren = false;

		prefab.ForPrefabChildren(prefab, [&hasChildren](Prefab&)
			{
				hasChildren = true;
			});

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

		if (!hasChildren)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (ImGui::TreeNodeEx(prefab.mName.c_str(),flags))
		{
			if(ImGui::IsItemClicked())
			{
				inspectedPrefab = &prefab;
			}
		
			prefab.ForPrefabChildren(prefab, [this](Prefab& childPrefab)
				{
					PrefabButton(childPrefab);
				});
			ImGui::TreePop();
		}
	}

	void HierarchyWindow::Update()
	{

		if (ImGui::Begin(mTitle))
		{

			if (ImGui::BeginChild("Game Object List: ", ImVec2(0, 400), ImGuiChildFlags_AlwaysUseWindowPadding))
			{
				if (mShowScene)
				{

					if(ImGui::TreeNodeEx(gGOFactory->sceneGO.sceneName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
					{
						gGOFactory->ForAllSceneGOs([this](GameObject& go)
							{
								GOButton(go);

							});
						ImGui::TreePop();
					}
				}
				else if (AssetWindow::selectedPrefab != nullptr)
				{
					bool backToScene = false;
					if (ImGui::Button("Back"))
					{
						AssetWindow::selectedPrefab = nullptr;
						inspectedPrefab = nullptr;
						mShowScene = true;
						backToScene = true;
					}
					if (!backToScene)
					{
						PrefabButton(*AssetWindow::selectedPrefab);
					}
				}
				ImGui::EndChild();
			}

#pragma region Unchilding

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
				{
					GameObject& droppedGO = *(GameObject*)payload->Data;

					droppedGO.SetParent(gGOFactory->sceneGO.sceneID);
				}
				ImGui::EndDragDropTarget();
			}

#pragma endregion

			static char goName[1024] = "Default";
			ImGui::Dummy(ImVec2(0, 20));
			ImGui::Text("Game Object Name: ");
			ImGui::SameLine();
			ImGui::InputText("##GameObjectCreation", goName, sizeof(goName));
			if (ImGui::Button("Create Default2D"))
			{
				gGOFactory->CreateGO(goName, true);
				std::strncpy(goName, "Default", sizeof(goName) - 1);
				goName[sizeof(goName) - 1] = '\0';
			}
			ImGui::SameLine();
			if (ImGui::Button("Create UIDefault"))
			{
				gGOFactory->CreateGO(goName, false);
				std::strncpy(goName, "Default", sizeof(goName) - 1);
				goName[sizeof(goName) - 1] = '\0';
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
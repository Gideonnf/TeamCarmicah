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
	std::vector<GameObject> createdList;
	GameObject* HierarchyWindow::selectedGO = nullptr;
	Prefab* HierarchyWindow::inspectedPrefab = nullptr;

	void HierarchyWindow::GOButton(GameObject& go)
	{
		if (ImGui::Button(go.GetName().c_str()))
		{
			selectedGO = &go;
		}

		ImGui::Indent();
		// Check if go has child
		gGOFactory->ForGOChildren(go, [this](GameObject& childGo) 
		{
				GOButton(childGo);
		});

		ImGui::Unindent();
	}

	void HierarchyWindow::PrefabButton(Prefab& prefab)
	{
		if (ImGui::Button(prefab.mName.c_str()))
		{
			inspectedPrefab = &prefab;
		}

		ImGui::Indent();
		prefab.ForPrefabChildren(prefab, [this](Prefab& childPrefab)
		{
			PrefabButton(childPrefab);
		});
	}

	void HierarchyWindow::Update()
	{
		static auto assetManager = AssetManager::GetInstance();
		auto prefabMap = assetManager->GetAssetMap<Prefab>();
			//static Transform playerTrans{};
			//static Collider2D playerCollider{ 1.0, 2.0, 3.0, 4.0 };
			//static Renderer toRender{};
		if (ImGui::Begin(mTitle))
		{
			if(ImGui::BeginChild("Game Object List: ", ImVec2(0,400),ImGuiChildFlags_AlwaysUseWindowPadding))
			{
				/*gGOFactory->ForAllGO([](GameObject& go)
					{
						if (ImGui::Button(go.GetName().c_str()))
						{
							selectedGO = &go;
						}
					});*/
				if(mShowScene && AssetWindow::selectedPrefab == nullptr)
				{
					gGOFactory->ForAllSceneGOs([this](GameObject& go)
						{
							GOButton(go);

						});
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
					if(!backToScene)
					{
						PrefabButton(*AssetWindow::selectedPrefab);
					}
				}
				ImGui::EndChild();
			}
			static char goName[256] = "Duck";
			ImGui::Text("Game Object Name: ");
			ImGui::SameLine();
			ImGui::Text(goName); //Cannot be edited for now

			if (ImGui::Button("Create Game Object"))
			{
				//static std::string name(goName);
				GameObject newObj = gGOFactory->CreatePrefab(goName);
				newObj.GetComponent<Transform>().PosXAdd(2.0);
				//newObj.AddComponent<Transform>(playerTrans);
				////newObj.AddComponent<Collider2D>(playerCollider);
				//newObj.AddComponent<Renderer>(toRender);
			}

			std::string buttonName = "Save current scene: " + SceneToImgui::GetInstance()->currentScene;
			if (ImGui::Button(buttonName.c_str()))
			{
				std::string sceneFile;
				AssetManager::GetInstance()->GetScene(SceneToImgui::GetInstance()->currentScene, sceneFile);
				SerializerSystem::GetInstance()->SerializeScene(sceneFile);
				//gGOFactory->DestroyAll();
			}

			std::string goCloneButton = "Clone GO";
			if (ImGui::Button(goCloneButton.c_str()))
			{
				if(selectedGO != nullptr)
				{
					gGOFactory->CloneGO(*selectedGO);
				}
				//gGOFactory->CreateGO();
			}

			std::string goCreateButton = "Create Blank GO";
			if (ImGui::Button(goCreateButton.c_str()))
			{
				gGOFactory->CreateGO();
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
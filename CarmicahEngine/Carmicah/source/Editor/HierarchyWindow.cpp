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

	void HierarchyWindow::Update()
	{
			//static Transform playerTrans{};
			//static Collider2D playerCollider{ 1.0, 2.0, 3.0, 4.0 };
			//static Renderer toRender{};
		if (ImGui::Begin(mTitle))
		{
			if(ImGui::BeginChild("Game Object List: ", ImVec2(900,300)))
			{
				/*gGOFactory->ForAllGO([](GameObject& go)
					{
						if (ImGui::Button(go.GetName().c_str()))
						{
							selectedGO = &go;
						}
					});*/
				gGOFactory->ForAllSceneGOs([this](GameObject& go)
					{
						GOButton(go);

					});
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

			std::string goCloneButton = "Clone GO";
			if (ImGui::Button(goCloneButton.c_str()))
			{
				gGOFactory->CloneGO(*selectedGO);
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
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

	void HierarchyWindow::DrawCustomSeparator(GameObject& go)
	{
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImVec2 separatorSize = ImVec2(100.f, 2.0f); // 2.0f for line thickness
		std::string buttonText = "Separator_" + std::to_string(go.GetID());
		//buttonID++;
		ImVec2 buttonSize = ImVec2(100.f, separatorSize.y + 2.0f);
		// Create an invisible button for interaction
		if (ImGui::InvisibleButton(buttonText.c_str(),buttonSize))
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

		if (selectedGO == &go)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		DrawCustomSeparator(go);

		//Dropping on the line between GameObjects
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
 				GameObject& droppedGO = *(GameObject*)payload->Data;
				Entity targettedGOParent = 0;

				if (droppedGO.GetID() == go.GetID())
				{
					//CM_CORE_WARN("Not re-arranging any hierarchy");
				}

				else
				{
					if(go.HasComponent<Transform>())
					{
						targettedGOParent = go.GetComponent<Transform>().parent;

						if (targettedGOParent == gGOFactory->sceneGO.sceneID)
						{
							droppedGO.SetParent(gGOFactory->sceneGO.sceneID);

							//If the droppedGO and the targetted GO has the same parent(only works for sceneGO, children no vectors rn)
							auto droppedIt = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
							auto targettedGO = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), go.GetID());

							if (targettedGO < droppedIt)
							{
								Editor::mSceneHierarchy.erase(droppedIt);
								Editor::mSceneHierarchy.insert(targettedGO, droppedGO.GetID());
							}
							else
							{
								Editor::mSceneHierarchy.insert(targettedGO, droppedGO.GetID());
								auto newDroppedIt = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
								Editor::mSceneHierarchy.erase(newDroppedIt);
							}

						}
					}

					else if (go.HasComponent<UITransform>())
					{
						targettedGOParent = go.GetComponent<UITransform>().parent;

						if (targettedGOParent == gGOFactory->sceneGO.sceneID)
						{
							droppedGO.SetParent(gGOFactory->sceneGO.sceneID);

							//If the droppedGO and the targetted GO has the same parent(only works for sceneGO, children no vectors rn)
							auto droppedIt = std::find(Editor::mSceneUIHierarchy.begin(), Editor::mSceneUIHierarchy.end(), droppedGO.GetID());
							auto targettedGO = std::find(Editor::mSceneUIHierarchy.begin(), Editor::mSceneUIHierarchy.end(), go.GetID());

							if (targettedGO < droppedIt)
							{
								Editor::mSceneUIHierarchy.erase(droppedIt);
								Editor::mSceneUIHierarchy.insert(targettedGO, droppedGO.GetID());
							}
							else
							{
								Editor::mSceneUIHierarchy.insert(targettedGO, droppedGO.GetID());
								Editor::mSceneUIHierarchy.erase(droppedIt);
							}

						}
					}

					//If they have different parents
					if(targettedGOParent != gGOFactory->sceneGO.sceneID)
					{
						//Find out the new parent (should be the parent of GO below the line)
						Entity newParentID = 0;

						if (go.HasComponent<Transform>())
						{
							newParentID = go.GetComponent<Transform>().parent;

							//Set the parent (should auto-update the mChildrenHierarchy too,setting it at the end)
							if(droppedGO.SetParent(gGOFactory->GetMIDToGO()[newParentID]))
							{

								//Sort it accordingly
								auto droppedIt = std::find(Editor::mChildrenHierarchy[newParentID].begin(), Editor::mChildrenHierarchy[newParentID].end(), droppedGO.GetID());
								auto targettedGO = std::find(Editor::mChildrenHierarchy[newParentID].begin(), Editor::mChildrenHierarchy[newParentID].end(), go.GetID());

								if (targettedGO < droppedIt)
								{
									Editor::mChildrenHierarchy[newParentID].erase(droppedIt);
									Editor::mChildrenHierarchy[newParentID].insert(targettedGO, droppedGO.GetID());
								}
								else
								{
									Editor::mChildrenHierarchy[newParentID].insert(targettedGO, droppedGO.GetID());
									Editor::mChildrenHierarchy[newParentID].erase(droppedIt);
								}
							}
						}

						else if (go.HasComponent<UITransform>())
						{
							newParentID = go.GetComponent<UITransform>().parent;

							//Set the parent (should auto-update the mChildrenHierarchy too,setting it at the end)
							if(droppedGO.SetParent(gGOFactory->GetMIDToGO()[newParentID]))
							{

								//Sort it accordingly
								auto droppedIt = std::find(Editor::mChildrenHierarchy[newParentID].begin(), Editor::mChildrenHierarchy[newParentID].end(), droppedGO.GetID());
								auto targettedGO = std::find(Editor::mChildrenHierarchy[newParentID].begin(), Editor::mChildrenHierarchy[newParentID].end(), go.GetID());

								if (targettedGO < droppedIt)
								{
									Editor::mChildrenHierarchy[newParentID].erase(droppedIt);
									Editor::mChildrenHierarchy[newParentID].insert(targettedGO, droppedGO.GetID());
								}
								else
								{
									Editor::mChildrenHierarchy[newParentID].insert(targettedGO, droppedGO.GetID());
									Editor::mChildrenHierarchy[newParentID].erase(droppedIt);
								}
							}
						}
					}
					//CM_CORE_INFO("Re-arranging success");
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

			//Setting Parent (drop on the actual node)
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
				{
					GameObject& droppedGO = *(GameObject*)payload->Data;
					
					//Set it to the end of the list if u drag it back onto its same parent (why not) can remove if not good
					if (!droppedGO.SetParent(go))
					{
						auto it = std::find(Editor::mChildrenHierarchy[go.GetID()].begin(), Editor::mChildrenHierarchy[go.GetID()].end(), droppedGO.GetID());

						if (it != Editor::mChildrenHierarchy[go.GetID()].end())
						{
							Editor::mChildrenHierarchy[go.GetID()].erase(it);
							Editor::mChildrenHierarchy[go.GetID()].push_back(droppedGO.GetID());
						}

					}
					else
					{

					}
					//auto it = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
					//Editor::mSceneHierarchy.erase(it);
					
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

	void HierarchyWindow::DisplayCollisionLogic()
	{
		if (ImGui::Button("Edit Collision Logic"))
		{
			ImGui::OpenPopup("CollisionFlagLogic");
		}

		if (ImGui::BeginPopup("CollisionFlagLogic"))
		{
			
			int maxLayers = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetMaxLayers();
			const uint32_t* layerArray = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerMap();

			if (ImGui::BeginTable("Collision Logic",maxLayers + 1))
			{
				ImGui::TableSetupColumn("Layer");
				for (int i = maxLayers - 1; i >= 0; --i)
				{
					uint32_t layerBit = 1 << i;
					const char* columnName = nullptr;

					columnName = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerName(static_cast<CollisionLayer>(layerBit));

					ImGui::TableSetupColumn(columnName, ImGuiTableColumnFlags_WidthStretch);
				}

				ImGui::TableHeadersRow();

				for (int row = 0; row < maxLayers; ++row)
				{
					uint32_t layerBit1 = 1 << row;
					int layer1Index = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerIndex(static_cast<CollisionLayer>(layerBit1));
					const char* layerName1 = nullptr;

					layerName1 = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerName(static_cast<CollisionLayer>(layerBit1));

					// Add a row for this layer
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text(layerName1);

					// Add checkboxes for this layer in remaining columns
					for (int col = maxLayers - 1; col >= 0; --col)
					{
						uint32_t layerBit2 = 1 << col;
						//int layer2Index = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerIndex(static_cast<CollisionLayer>(layerBit2));
						const char* layerName2 = nullptr;
						layerName2 = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerName(static_cast<CollisionLayer>(layerBit2));
						ImGui::TableNextColumn();
						//Setting the style
						ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
						ImGui::AlignTextToFramePadding();

						bool combinedEnabled = layerArray[layer1Index] & layerBit2;

						std::string combinedLayer = "##" + std::string(layerName1) + std::string(layerName2);


						if (ImGui::Checkbox(combinedLayer.c_str(), &combinedEnabled))
						{
							if (combinedEnabled)
							{
								SystemManager::GetInstance()->GetSystem<TransformSystem>()->EnableLayerInteraction(static_cast<CollisionLayer>(layerBit1), static_cast<CollisionLayer>(layerBit2));
							}
							else
							{
								SystemManager::GetInstance()->GetSystem<TransformSystem>()->DisableLayerInteraction(static_cast<CollisionLayer>(layerBit1), static_cast<CollisionLayer>(layerBit2));
							}
						}
						//Pop style
						ImGui::PopStyleVar();

						if (layerBit1 == layerBit2)
						{
							break;
						}
					}
				}
				ImGui::EndTable();
			}
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void HierarchyWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
#pragma region Debug Region
			/*static int counter = 0;
			if(counter++ % 300 == 0)
			{
				std::string GOFactoryChildren = "No. of children sceneGO has: " + std::to_string(gGOFactory->sceneGO.children.size());
				CM_CORE_INFO(GOFactoryChildren);
				std::string parentedObjects = "No of objects parented to sceneHierarchy: " + std::to_string(Editor::mSceneHierarchy.size());
				CM_CORE_INFO(parentedObjects);
				std::string UIparentedObjects = "No of objects parented to sceneHierarchy: " + std::to_string(Editor::mSceneUIHierarchy.size());
				CM_CORE_INFO(UIparentedObjects);

				std::string equalParented = "Total Number of Parented Objects: " + std::to_string(Editor::mSceneHierarchy.size() + Editor::mSceneUIHierarchy.size()) + " = " + GOFactoryChildren;
				CM_CORE_INFO(equalParented);
				std::string parentsWithChildren = "No of objects with children: " + std::to_string(Editor::mChildrenHierarchy.size());
				CM_CORE_INFO(parentsWithChildren);

				for (const auto& entry : Editor::mChildrenHierarchy)
				{
					std::string entityChildren = "Entity ID(Editor) " + std::to_string(entry.first) + " has " + std::to_string(entry.second.size()) + " children";
					CM_CORE_INFO(entityChildren);
					if (gGOFactory->GetMIDToGO()[entry.first].HasComponent<Transform>())
					{
						std::string actualChildrenNo = "Entity ID " + std::to_string(gGOFactory->GetMIDToGO()[entry.first].GetID()) + " actually has " + std::to_string(gGOFactory->GetMIDToGO()[entry.first].GetComponent<Transform>().children.size()) + " children";
						CM_CORE_INFO(actualChildrenNo);
					}
				}

				CM_CORE_INFO("--------------------");
			}
			
			if (selectedGO != nullptr)
			{
				if (selectedGO->HasComponent<Transform>())
				{
					
				}
			}*/
#pragma endregion

			if (ImGui::BeginChild("Game Object List: ", ImVec2(0, 400), ImGuiChildFlags_AlwaysUseWindowPadding))
			{
				if (mShowScene)
				{
					if(ImGui::BeginTabBar("Tabs"))
					{

						//Scene Hierarchy
						if (ImGui::BeginTabItem("Scene Hierarchy"))
						{
							if(ImGui::TreeNodeEx(SceneToImgui::GetInstance()->currentScene.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
							{

								gGOFactory->ForAllSceneGOs([this](GameObject& go)
									{
										GOButton(go);

									});
					
								ImVec2 cursorPos = ImGui::GetCursorScreenPos();
								ImVec2 separatorSize = ImVec2(100.f, 2.0f); // 2.0f for line thickness
								std::string buttonText = "Separator_Final";
								//buttonID++;
								ImVec2 buttonSize = ImVec2(100.f, separatorSize.y + 2.0f);
								// Create an invisible button for interaction
								if (ImGui::InvisibleButton(buttonText.c_str(), buttonSize))
								{
									//Clicking will do nothing
								}

								// Draw the line visually
								ImDrawList* drawList = ImGui::GetWindowDrawList();
								drawList->AddLine(
									ImVec2(cursorPos.x, cursorPos.y + separatorSize.y),
									ImVec2(cursorPos.x + separatorSize.x, cursorPos.y + separatorSize.y),
									ImGui::GetColorU32(ImGuiCol_Separator), 1.0f);
						

								//The Bottom Line of the Entire Hierarchy
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
									{
										GameObject& droppedGO = *(GameObject*)payload->Data;

										if (droppedGO.GetID() == *(Editor::mSceneHierarchy.end() - 1))
										{
											CM_CORE_WARN("Not re-arranging any hierarchy");
										}

										else
										{
											if (!droppedGO.SetParent(gGOFactory->sceneGO.sceneID))
											{
												auto droppedIt = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
												Editor::mSceneHierarchy.erase(droppedIt);
												Editor::mSceneHierarchy.push_back(droppedGO.GetID());
											}
										}
									}
									ImGui::EndDragDropTarget();
								}
								ImGui::TreePop();
							}
							ImGui::EndTabItem();
						}

						// UI Hierarchy
						if (ImGui::BeginTabItem("UI Hierarchy"))
						{
							if (ImGui::TreeNodeEx(gGOFactory->sceneGO.sceneName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
							{
								gGOFactory->ForAllSceneUIGOs([this](GameObject& go)
									{
										GOButton(go);

									});

								ImVec2 cursorPos = ImGui::GetCursorScreenPos();
								ImVec2 separatorSize = ImVec2(100.f, 2.0f); // 2.0f for line thickness
								std::string buttonText = "Separator_Final";
								//buttonID++;
								ImVec2 buttonSize = ImVec2(100.f, separatorSize.y + 2.0f);
								// Create an invisible button for interaction
								if (ImGui::InvisibleButton(buttonText.c_str(), buttonSize))
								{
									//Clicking will do nothing
								}

								// Draw the line visually
								ImDrawList* drawList = ImGui::GetWindowDrawList();
								drawList->AddLine(
									ImVec2(cursorPos.x, cursorPos.y + separatorSize.y),
									ImVec2(cursorPos.x + separatorSize.x, cursorPos.y + separatorSize.y),
									ImGui::GetColorU32(ImGuiCol_Separator), 1.0f);


								//The Bottom Line of the Entire Hierarchy
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
									{
										GameObject& droppedGO = *(GameObject*)payload->Data;

										if (droppedGO.GetID() == *(Editor::mSceneHierarchy.end() - 1))
										{
											CM_CORE_WARN("Not re-arranging any hierarchy");
										}

										else
										{
											if (!droppedGO.SetParent(gGOFactory->sceneGO.sceneID))
											{
												auto droppedIt = std::find(Editor::mSceneHierarchy.begin(), Editor::mSceneHierarchy.end(), droppedGO.GetID());
												Editor::mSceneHierarchy.erase(droppedIt);
												Editor::mSceneHierarchy.push_back(droppedGO.GetID());
											}
										}
									}
									ImGui::EndDragDropTarget();
								}
								ImGui::TreePop();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();	
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
			//Unchilding by dragging out
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
				{
					GameObject& droppedGO = *(GameObject*)payload->Data;

					if(droppedGO.SetParent(gGOFactory->sceneGO.sceneID))
					{
						//This portion is handled by the msg sending
					}
				}
				ImGui::EndDragDropTarget();
			}

#pragma endregion
			if(mShowScene)
			{
				static char goName[256] = "Default";
				ImGui::Dummy(ImVec2(0, 20));
				ImGui::Text("Game Object Name: ");
				ImGui::SameLine();
				ImGui::InputText("##GameObjectCreation", goName, sizeof(goName));
				//Create Default
				if (ImGui::Button("Create Default2D"))
				{
					gGOFactory->CreateGO(goName, TRANSFORMTYPE::TRANSFORM);
					std::strncpy(goName, "Default", sizeof(goName) - 1);
					goName[sizeof(goName) - 1] = '\0';
				}
				ImGui::SameLine();
				//Creating UIDefault
				if (ImGui::Button("Create UIDefault"))
				{
					gGOFactory->CreateGO(goName, TRANSFORMTYPE::UITRANSFORM);
					std::strncpy(goName, "Default", sizeof(goName) - 1);
					goName[sizeof(goName) - 1] = '\0';
				}
				//Save scene
				std::string buttonName = "Save current scene: " + SceneToImgui::GetInstance()->currentScene;
				if (ImGui::Button(buttonName.c_str()))
				{
					std::string sceneFile;
					AssetManager::GetInstance()->GetScene(SceneToImgui::GetInstance()->currentScene, sceneFile);
					SerializerSystem::GetInstance()->SerializeScene(sceneFile);
					//gGOFactory->DestroyAll();
				}
				//Remove PrefabData
				if(selectedGO != nullptr && selectedGO->HasComponent<PrefabData>())
				{
					if (ImGui::Button("Un-prefab Object"))
					{
						selectedGO->RemoveComponent<PrefabData>();
					}
				}


				DisplayCollisionLogic();
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
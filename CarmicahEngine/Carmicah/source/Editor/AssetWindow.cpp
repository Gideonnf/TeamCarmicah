/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:	This file defines AssetWindow class which allows users to interact and manage assets.
		Prefabs can be selected from this window.
		Drag-drop functionality for textures to replace the textures of current GameObjects.
		Clicking on fonts to change fonts for GOs with TextRenderer.

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
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "../Systems/AssetManager.h"
#include "../Systems/SceneSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"

namespace Carmicah
{
	AssetWindow::AssetWindow() : EditorWindow("Asset Browser", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	Prefab* AssetWindow::selectedPrefab = nullptr;
	/**
	 * @brief Update function for the AssetWindow
	 * 
	 */
	void AssetWindow::Update()
	{
		//Obtaining all the AssetMaps
		static auto assetManager = AssetManager::GetInstance();
		static auto systemManager = SystemManager::GetInstance();
		//auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		//auto shaderMap = assetManager->GetAssetMap<Shader>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		auto audioMap = assetManager->GetAssetMap<FMOD::Sound*>();
		auto prefabMap = assetManager->GetAssetMap<Prefab>();
		auto sceneMap = assetManager->GetAssetMap<Scene>();

		if (ImGui::Begin(mTitle))
		{
			std::string name;
			/*if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				for (const auto& file : Editor::droppedFilePaths)
				{
					std::cout << file << std::endl;
				}
				Editor::droppedFilePaths.clear();
			}*/

			if (ImGui::Button("Reload Assets"))
			{
				AssetManager::GetInstance()->fileWatcher.Update();
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Indent();
				//ImGui::BeginChild("TextureTableRegion", ImVec2(400, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
				if (ImGui::BeginTable("TextureTable", 5))
				{

					
					for (const auto& entry : textureMap->mAssetMap)
					{
						name = entry.first + "##texture";
						if (name.find("SpriteSheet") != std::string::npos)
						{
							continue;
						}

						Mtx3x3f matrix = textureMap->mAssetList[entry.second].mtx;
						//GLuint textureID = assetManager->mArrayTex;
						Vec2f uv0(0, 0);
						Vec2f uv1(1, 1);
						uv0 = matrix * uv0;
						uv1 = matrix * uv1;
						float temp = -uv0.y;
						uv0.y = -uv1.y;
						uv1.y = temp;

						ImGui::TableNextColumn();
						if (ImGui::ImageButton(name.c_str(),
							reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[textureMap->mAssetList[entry.second].t])),
							ImVec2(50, 50),
							ImVec2(uv0.x, uv0.y),
							ImVec2(uv1.x, uv1.y)))
						{
							if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
							{
								Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
								for (const auto& textureEntry : textureMap->mAssetMap)
								{
									if (entry.second == textureEntry.second)
									{
										render.Texture(textureEntry.first);
									}
								}
							}

						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &entry.first, sizeof(entry.first));
							ImVec2 dragSize(50, 50);
							ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[textureMap->mAssetList[entry.second].t])),
								dragSize,
								ImVec2(uv0.x, uv0.y),
								ImVec2(uv1.x, uv1.y));
							ImGui::Text("%s", entry.first.c_str());

							ImGui::EndDragDropSource();
						}
						ImGui::SameLine();
						ImGui::Text("%s", entry.first.c_str());

					}
					ImGui::EndTable();
				}
				//ImGui::EndChild();
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Font"))
			{
				ImGui::Indent();

				for (const auto& entry : fontMap->mAssetMap)
				{
					name = entry.first + "##font";
					if (ImGui::Button(name.c_str()))
					{
						if(HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<TextRenderer>())
						{
							TextRenderer& text = HierarchyWindow::selectedGO->GetComponent<TextRenderer>();
							text.font = entry.first;
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Font!");
						ImGui::EndTooltip();
					}
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Audio"))
			{
				ImGui::Indent();
				for (const auto& entry : audioMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()));
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Prefab"))
			{
				ImGui::Indent();
				for (const auto& entry : prefabMap->mAssetMap)
				{
					name = entry.first + "##Prefab";
					if (ImGui::Button(name.c_str())){}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						selectedPrefab = &prefabMap->mAssetList[entry.second];
						if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
						{
							gGOFactory->CreatePrefab(entry.first);
						}
					}

					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup(name.c_str());
					}

					if (ImGui::BeginPopup(name.c_str()))
					{
						if (ImGui::Button("Edit Prefab"))
						{
							selectedPrefab = &prefabMap->mAssetList[entry.second];
							HierarchyWindow::inspectedPrefab = &prefabMap->mAssetList[entry.second];
							HierarchyWindow::mShowScene = false;
							HierarchyWindow::selectedGO = nullptr;
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
					{
						ImGui::SetDragDropPayload("PREFAB_PAYLOAD", &entry.first, sizeof(entry.first));
						ImGui::Text("Dragging Prefab %s", name.c_str());
						ImGui::EndDragDropSource();
					}
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Scene"))
			{
				for (const auto& entry : sceneMap->mAssetMap)
				{
					name = entry.first + "##Scene";

					if (ImGui::Button(name.c_str()))
					{
						systemManager->ChangeScene(entry.first);
					}
				}
			}
		}
		ImGui::End();
	}
}
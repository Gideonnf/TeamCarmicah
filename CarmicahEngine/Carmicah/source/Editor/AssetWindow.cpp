/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:	This file defines AssetWindow class which allows users to interact and manage assets.

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
		auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		auto shaderMap = assetManager->GetAssetMap<Shader>();
		auto imageTextureMap = assetManager->GetAssetMap<ImageTexture>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		//auto audioMap = assetManager->GetAssetMap<Audio>();
		auto prefabMap = assetManager->GetAssetMap<Prefab>();
		auto sceneMap = assetManager->GetAssetMap<Scene>();

		if (ImGui::Begin(mTitle))
		{
			std::string name;
			if (ImGui::CollapsingHeader("Primitive"))
			{
				ImGui::Indent();
				for (const auto& entry : primitiveMap->mAssetMap)
				{
					name = entry.first + "##Prim";
					if (ImGui::Button(name.c_str()))
					{
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Cool Shape!");
						ImGui::EndTooltip();
					}
				}
				ImGui::Unindent();
			}
			/*if (ImGui::CollapsingHeader("Shader"))
			{
				ImGui::Indent();
				for (const auto& entry : shaderMap->mAssetMap)
				{
					name = entry.first + "##Shader";
					if (ImGui::Button(name.c_str()))
					{
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImVec2 textureSize(100, 100);
						GLuint textureID = shaderMap->mAssetList[entry.second].s;
						ImGui::Text("Shader!");
						ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(textureID)), textureSize);
						ImGui::EndTooltip();
					}
				}
				ImGui::Unindent();
			}*/

			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Indent();
				
				for (const auto& entry : textureMap->mAssetMap)
				{
					name = entry.first + "##texture";
					if (ImGui::Button(name.c_str()))
					{
						Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
						for (const auto& textureEntry : textureMap->mAssetMap)
						{
							if (entry.second == textureEntry.second)
							{
								render.texture = textureEntry.first;
							}
						}

					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImVec2 textureSize(100, 100);
						GLuint textureID = textureMap->mAssetList[entry.second].t;
						ImGui::Text("Texture!");
						ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(textureID)), textureSize);
						ImGui::EndTooltip();
					}
				}
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
						if(HierarchyWindow::selectedGO->HasComponent<TextRenderer>())
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

			/*if (ImGui::CollapsingHeader("Audio"))
			{
				ImGui::Indent();
				for (const auto& entry : audioMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()));
				}
				ImGui::Unindent();
			}*/

			if (ImGui::CollapsingHeader("Prefab"))
			{
				ImGui::Indent();
				for (const auto& entry : prefabMap->mAssetMap)
				{
					name = entry.first + "##Prefab";
					if (ImGui::Button(name.c_str()))
					{
						selectedPrefab = &prefabMap->mAssetList[entry.second];
						HierarchyWindow::inspectedPrefab = &prefabMap->mAssetList[entry.second];
						HierarchyWindow::mShowScene = !HierarchyWindow::mShowScene;
						HierarchyWindow::selectedGO = nullptr;
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
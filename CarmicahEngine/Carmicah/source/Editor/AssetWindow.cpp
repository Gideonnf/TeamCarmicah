/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetWindow.cpp

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
#include "AssetWindow.h"
#include "../Systems/AssetManager.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"

namespace Carmicah
{
	AssetWindow::AssetWindow() : EditorWindow("Asset", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	void AssetWindow::Update()
	{
		//Obtainninng all the AssetMaps
		static auto assetManager = AssetManager::GetInstance();
		auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		auto shaderMap = assetManager->GetAssetMap<Shader>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		//auto audioMap = assetManager->GetAssetMap<Audio>();
		auto prefabMap = assetManager->GetAssetMap<Prefab>();

		if (ImGui::Begin(mTitle))
		{
			if (ImGui::CollapsingHeader("Primitive"))
			{
				ImGui::Indent();
				for (const auto& entry : primitiveMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()))
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
			if (ImGui::CollapsingHeader("Shader"))
			{
				ImGui::Indent();
				for (const auto& entry : shaderMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()))
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
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Indent();
				for (const auto& entry : textureMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()))
					{

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
					if (ImGui::Button(entry.first.c_str()))
					{

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
					if (ImGui::Button(entry.first.c_str()))
					{
						
					}
				}
				ImGui::Unindent();
			}
		}
		ImGui::End();
	}
}
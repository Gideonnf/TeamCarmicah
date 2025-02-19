/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This file defines AssetWindow class which allows users to interact and manage assets.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ASSETWINDOW_H_
#define ASSETWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "ECS/GameObject.h"
#include "Editor.h"
#include "EditorWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "../Systems/AssetManager.h"
#include "../Systems/AssetManager.h"
#include "../Systems/SceneSystem.h"
#include "../Systems/SoundSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"

#include <algorithm>


namespace Carmicah
{
	class AssetWindow : public EditorWindow
	{
	private:

	public:
		static std::string soundToPlay;
		static Prefab* selectedPrefab;
		static bool mSceneModified;

		/**
		 * @brief Construct a new Asset Window object
		 *
		 */
		AssetWindow();
		
		/**
		 * @brief Update function override from EditorWindow
		 *
		 */
		void Update() override;


		void TextureBrowser(std::shared_ptr<Carmicah::AssetType<Carmicah::Texture>> map);

	};
}



#endif
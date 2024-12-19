/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			HierarchyWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This HierarchyWindow class is a derived class from EditorWindow.
				It currently provides an list of GameObjects, and has a button that allows for the creation of new GameObjects.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef HEIRARCHYWINDOW_H_
#define HEIRARCHYWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "ECS/GameObject.h"
#include "../Systems/AssetTypes.h"

namespace Carmicah
{
	class HierarchyWindow : public EditorWindow
	{
	private:
	public:
		static bool mShowScene;
		static GameObject* selectedGO;
		static Prefab* inspectedPrefab;
		/**
		 * @brief Construct a new Hierarchy Window object
		 *
		 */
		HierarchyWindow();
		/**
		 * @brief Update function override from EditorWindow
		 *
		 */
		void Update() override;
		/**
		 * @brief Tells when an entity is destroyed.
		 *
		 * @param id
		 */
		void EntityDestroyed(Entity id) override;


		void GOButton(GameObject& go);

		void PrefabButton(Prefab& prefab);

	};
}

#endif
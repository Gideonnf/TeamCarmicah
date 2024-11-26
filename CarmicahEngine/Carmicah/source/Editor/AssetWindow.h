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
#include "EditorWindow.h"
#include "ECS/GameObject.h"
#include "../Systems/AssetManager.h"


namespace Carmicah
{
	class AssetWindow : public EditorWindow
	{
	private:

	public:

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

	};
}



#endif
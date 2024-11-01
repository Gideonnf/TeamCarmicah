/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef INSPECTORWINDOW_H_
#define INSPECTORWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "ECS/GameObject.h"


namespace Carmicah
{
	class InspectorWindow : public EditorWindow
	{
	private:

	public:
		/**
		 * @brief Construct a new Inspector Window object
		 *
		 */
		InspectorWindow();
		/**
		 * @brief Update function override from EditorWindow
		 *
		 */
		void Update() override;

	};
}



#endif
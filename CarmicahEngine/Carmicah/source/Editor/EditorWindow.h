/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			EditorWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This EditorWindow class serves as a base class for various editor window types, containing basic functionality.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef EDITORWINDOW_H_
#define EDITORWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "../ECS/BaseSystem.h" // why does editor window need base system included


namespace Carmicah
{
	class EditorWindow
	{
	public:

		const char* mTitle;
		ImVec2 mSize;
		ImVec2 mPos;
		bool mIsVisible;
		std::vector<std::shared_ptr<Message>> mMessages;

	public:
		/**
		 * @brief Construct a new Editor Window object
		 * 
		 * @param title 
		 * @param size 
		 * @param pos 
		 */
		EditorWindow(const char* title, ImVec2 size, ImVec2 pos);
		/**
		 * @brief Base class Update function for the class
		 * 
		 */
		virtual void Update();
		/**
		 * @brief Tells when an entity is destroyed.
		 * 
		 * @param id 
		 */
		virtual void EntityDestroyed(Entity id);

	};
}




#endif
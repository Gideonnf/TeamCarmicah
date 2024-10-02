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

namespace Carmicah
{
	class EditorWindow
	{
	public:

		const char* mTitle;
		ImVec2 mSize;
		ImVec2 mPos;
		bool mIsVisible;
	public:

		EditorWindow(const char* title, ImVec2 size, ImVec2 pos);

		virtual void Update();


	};
}




#endif
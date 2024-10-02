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
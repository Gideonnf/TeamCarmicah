#ifndef EDITORWINDOW_H_
#define EDITORWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>

namespace Carmicah
{
	class EditorWindow
	{
	public:

		const char* title;
		ImVec2 size;
		ImVec2 pos;
		bool isVisible;
	public:

		EditorWindow(const char* title, ImVec2 size, ImVec2 pos);

		virtual void Update();


	};
}




#endif
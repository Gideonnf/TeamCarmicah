#ifndef HEIRARCHYWINDOW_H_
#define HEIRARCHYWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "Editor/EditorWindow.h"

namespace Carmicah
{
	class HeirarchyWindow : public EditorWindow
	{
	private:
	
	public:

		HeirarchyWindow();

		void Update() override;


	};
}

#endif
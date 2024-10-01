#ifndef DEBUGWINDOW_H_
#define DEBUGWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "Editor/EditorWindow.h"

namespace Carmicah
{
	class DebugWindow : public EditorWindow
	{
	private:
		bool mShowFPS;
		bool mShowLogger;
	public:

		DebugWindow();

		void Update() override;


	};
}


#endif
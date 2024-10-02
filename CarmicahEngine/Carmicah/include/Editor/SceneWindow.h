#ifndef SCENEWINDOW_H_
#define SCENEWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "Editor/EditorWindow.h"
#include "log.h"

namespace Carmicah
{
	class SceneWindow : public EditorWindow
	{
	private:


	public:

		SceneWindow();

		void Update() override;


	};
}

#endif
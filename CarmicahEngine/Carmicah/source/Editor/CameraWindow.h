#ifndef CAMERAWINDOW_H_
#define CAMERAWINDOW_H_


#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "Editor.h"
#include "log.h"
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class CameraWindow : public EditorWindow
	{
	private:


	public:

		/**
		 * @brief Construct a new Scene Window object
		 *
		 */
		CameraWindow();
		/**
		 * @brief SceneWindow Update override from EditorWindow
		 *
		 */
		void Update() override;


	};
}

#endif
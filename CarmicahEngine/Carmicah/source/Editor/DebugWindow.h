/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			DebugWindow.h

 author:		Nicholas Lai (60%)
 co-author(s):	Yang Yujie (40%)

 email:			n.lai@digipen.edu

 brief:			This DebugWindow class is a derived class from EditorWindow.
				It currently provides an FPS tracker, a console log and profiler for tracking purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DEBUGWINDOW_H_
#define DEBUGWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "log.h"


namespace Carmicah
{
	class DebugWindow : public EditorWindow
	{
	private:
		bool mShowFPS;
		bool mShowLogger;
		bool mShowProfiling;
		bool mShowGPUProfiling;
		
		// Profiling variables
		/**
		 * @brief Renders the Profiling tab
		 * 
		 */
		void RenderProfilingTab();
		/**
		 * @brief Renders the GPU profiling tab
		 * 
		 */
		void RenderGPUProfilingTab();

	public:
		/**
		 * @brief Construct a new Debug Window object
		 * 
		 */
		DebugWindow();
		/**
		 * @brief Update override over EditorWindow class
		 * 
		 */
		void Update() override;
	};
}

#endif
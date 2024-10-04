/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Editor.h

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:			This editor class manages and coordinates the various editor windows.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef EDITOR_H_
#define EDITOR_H_
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "DebugWindow.h"
#include "EditorWindow.h"
#include "HierarchyWindow.h"
#include "SceneWindow.h"
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class Editor : public BaseSystem
	{
	public:
		/**
		 * @brief Construct a new Editor object
		 * 
		 */
		Editor();
		/**
		 * @brief Destroy the Editor object
		 * 
		 */
		~Editor();
		/**
		 * @brief Initializer function for Editor class
		 * 
		 * @param window 
		 */
		void Init(GLFWwindow* window);
		/**
		 * @brief Update function for Editor class
		 * 
		 */
		void Update();
		/**
		 * @brief Render function for the Editor class
		 * 
		 * @param window 
		 */
		void Render(GLFWwindow* window);
		/**
		 * @brief Exit function for the editor class
		 * 
		 */
		void Exit();
		/**
		 * @brief Tells the editor class when an entity is destroyed
		 * 
		 * @param id 
		 */
		void EntityDestroyed(Entity id) override;

	private:

		std::vector<std::unique_ptr<EditorWindow>> mWindows;
	};

}

#endif

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
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_internal.h>
#include "DebugWindow.h"
#include "EditorWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "AssetWindow.h"
#include "SceneWindow.h"
#include "CameraWindow.h"
#include "../ECS/BaseSystem.h"
#include "../Messaging/Message.h"
#include "../Systems/AssetManager.h"
#include "../Input/InputSystem.h"
#include "../Systems/GOFactory.h"

namespace Carmicah
{
	class Editor : public BaseSystem
	{
	public:

		static std::vector<std::string> sDroppedFilePaths;
		static bool mShowCloseConfirmation;
		static std::vector<Entity> mSceneHierarchy;

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
		void Update(GLFWwindow* window);
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

		void EntityAdded(Entity id) override;

		void ReceiveMessage(Message* msg) override;

		static void DropCallback(GLFWwindow* window, int count, const char** paths);

		static void CloseCallback(GLFWwindow* window);

	private:

		std::vector<std::unique_ptr<EditorWindow>> mWindows;

		//Vector to store any dropped file paths
		
	};

}

#endif

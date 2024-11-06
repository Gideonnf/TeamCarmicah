/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef INSPECTORWINDOW_H_
#define INSPECTORWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"
#include "ECS/GameObject.h"


namespace Carmicah
{
	class InspectorWindow : public EditorWindow
	{
	private:
		static std::string selectedComponentToAdd;
	public:
		/**
		 * @brief Construct a new Inspector Window object
		 *
		 */
		InspectorWindow();
		/**
		 * @brief Update function override from EditorWindow
		 *
		 */
		void Update() override;

		/**
		 * @brief Function that creates the inspector table
		 * 
		 * @tparam T 
		 * @param go 
		 * @param id 
		 */
		template<typename T>
		void InspectorTable(T* go, Entity id);
		/**
		 * @brief Function Overload that creates inspector table
		 * 
		 * @tparam T 
		 * @param go 
		 */
		template<typename T>
		void InspectorTable(T* go);
		/**
		 * @brief Function that creates and facilitates adding component
		 * 
		 * @param go 
		 */
		void AddComponentButton(GameObject* go);
		/**
		 * @brief Function  that facilitates removing a component from a  gameobject.
		 * 
		 * @tparam T 
		 * @param go 
		 */
		template<typename T>
		void RemoveComponentButton(Entity go);


	};
}



#endif
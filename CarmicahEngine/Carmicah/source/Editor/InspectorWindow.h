/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This file implements the InspectorWindow class, which provides an interface for inspecting and modifying the properties of game objects and their components. 
                The functionality is similar to Unity's Inspector, allowing users to interact with and edit properties such as transforms, scripts, and custom components in the editor.
                This window dynamically updates to reflect the selected game object and offers various tools for inspecting and editing the object’s state.

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

		//TODO: IF IT WORKS< APPLY IT FOR EVERYTHIGN ELSE
		template <typename T>
		void CheckForComponentChange(GameObject& go, T& newComponent, bool modified)
		{
			// Only used for prefab game objects
			if (go.HasComponent<T>() && go.HasComponent<PrefabData>())
			{
				T& component = go.GetComponent<T>();
				// change in the component
				if (modified)
				{
					go.GetComponent<PrefabData>().mComponentsModified.push_back(typeid(T).name());
				}
			}

			// Update the component after
			go.GetComponent<T>() = newComponent;
		}

	};
}



#endif
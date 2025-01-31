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
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"
#include "Scripting/ScriptSystem.h"
#include "Components/Prefab.h"
#include "Components/StateMachine.h"
#include "Systems/GOFactory.h"
#include "../Systems/AssetManager.h"


namespace Carmicah
{
	class InspectorWindow : public EditorWindow
	{
	private:
		static std::string selectedComponentToAdd;

		enum TABLETYPE
		{
			GAMEOBJECT = 0,
			PREFAB = 1
		};


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
		void InspectorTable(T* go, TABLETYPE type);
		
		/**
		 * @brief Function that creates and facilitates adding component
		 * 
		 * @param go 
		 */
		template<typename T>
		void AddComponentButton(T* go);
		/**
		 * @brief Function  that facilitates removing a component from a prefab/gameobject.
		 * 
		 * @tparam T 
		 * @param go 
		 */
		template<typename T>
		bool RemoveComponentButton(GameObject* go);

		template <typename T>
		bool RemoveComponentButton(Prefab* go);
		/**
		 * @brief Renders the transform table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderTransformTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the UItransform table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderUITransformTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Renderer component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderRenderingTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Animation component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderAnimationTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Rigidbody component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		bool RenderRigidBodyTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Collider2D component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderCollider2DTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the TextRenderer component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderTextRenderTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Button component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderButtonTable(T* data, TABLETYPE type);
		/**
		 * @brief Renders the Script component table
		 * 
		 * @tparam T 
		 * @param data 
		 * @param type 
		 */
		template<typename T>
		void RenderScriptTable(T* data, TABLETYPE type);
		
		
		void VariantVarSelectPopUp(std::string& varType);
		/**
		 * @brief Renders the StateMachine component table
		 *
		 * @tparam T
		 * @param data
		 * @param type
		 */
		template<typename T>
		void RenderStateMachineTable(T* go, TABLETYPE type);


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
				// Update the component after
				component = newComponent;
			}

		}

		template <typename T>
		void CheckForComponentChange(Prefab& go, T& newComponent, bool modified)
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
				// Update the component after
				component = newComponent;
			}

		}

	};
}



#endif
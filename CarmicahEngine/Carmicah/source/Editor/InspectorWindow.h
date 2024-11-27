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
#include "Systems/GOFactory.h"
#include "../Systems/AssetManager.h"


namespace Carmicah
{
	class InspectorWindow : public EditorWindow
	{
	private:
		static std::string selectedComponentToAdd;
		std::shared_ptr<AssetType<Texture>> textureMap;
		std::shared_ptr<AssetType<Font>> fontMap;
		std::shared_ptr<AssetType<AnimAtlas>> animMap;
		std::shared_ptr<AssetType<Button>> buttonMap;
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

		template<typename EntityID>
		void RenderTransformTable(Transform& data, EntityID id);

		template<typename EntityID>
		void RenderUITransformTable(UITransform& data, EntityID id);

		template<typename EntityID>
		void RenderRenderingTable(Renderer& data, EntityID id);

		template<typename EntityID>
		void RenderAnimationTable(Animation& data, EntityID id);

		template<typename EntityID>
		bool RenderRigidBodyTable(RigidBody& rb, EntityID id);

		template<typename EntityID>
		void RenderCollider2DTable(Collider2D& data, EntityID id);

		template<typename EntityID>
		void RenderTextRenderTable(TextRenderer& data, EntityID id);

		template<typename EntityID>
		void RenderButtonTable(Button& data, EntityID id);

		template<typename EntityID>
		void RenderScriptTable(Script& data, EntityID id);
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

	};
}



#endif
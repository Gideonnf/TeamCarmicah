/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.cpp

 author:		Nicholas Lai (70%)
 co-author(s):	Micah Lim (30%)

 email:			n.lai@digipen.edu

 brief: 		This file implements the InspectorWindow class, which provides an interface for inspecting and modifying the properties of game objects and their components. 
                The functionality is similar to Unity's Inspector, allowing users to interact with and edit properties such as transforms, scripts, and custom components in the editor.
                This window dynamically updates to reflect the selected game object and offers various tools for inspecting and editing the object’s state.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "AssetWindow.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "Components/Renderer.h"
#include "Components/UITransform.h"
#include "Scripting/ScriptSystem.h"
#include "Components/Prefab.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "Systems/GOFactory.h"

namespace Carmicah
{
	InspectorWindow::InspectorWindow() : EditorWindow("Inspector", ImVec2(900, 300), ImVec2(0, 0)) { mIsVisible = true; }

	std::string InspectorWindow::selectedComponentToAdd = "";
	/**
	 * @brief A drop down to select which component to add to the specified go.
	 * 
	 * @param go 
	 */
	void InspectorWindow::AddComponentButton(GameObject* go)
	{
		std::vector<const char*> componentsToAdd;

		static int selectedIndex;

		componentsToAdd.push_back("");

		if (!go->HasComponent<Renderer>())
		{
			componentsToAdd.push_back("Renderer");
		}
		if (!go->HasComponent<Transform>())
		{
			componentsToAdd.push_back("Transform");
		}
		if (!go->HasComponent<UITransform>())
		{
			componentsToAdd.push_back("UITransform");
		}
		if (!go->HasComponent<Animation>())
		{
			componentsToAdd.push_back("Animation");
		}
		if (!go->HasComponent<RigidBody>())
		{
			componentsToAdd.push_back("RigidBody");
		}
		if (!go->HasComponent<Collider2D>())
		{
			componentsToAdd.push_back("Collider2D");
		}
		if (!go->HasComponent<Button>())
		{
			componentsToAdd.push_back("Button");
		}
		if (!go->HasComponent<TextRenderer>())
		{
			componentsToAdd.push_back("TextRenderer");
		}
		if (!go->HasComponent<Script>())
		{
			componentsToAdd.push_back("Script");
		}
		

		if (ImGui::Combo("##", &selectedIndex, componentsToAdd.data(), (int)componentsToAdd.size()))
		{
			selectedComponentToAdd = componentsToAdd[selectedIndex];
		}
		ImGui::SameLine();
		if(ImGui::Button("Add Component"))
		{
			if (selectedComponentToAdd == "Renderer")
			{
				go->AddComponent<Renderer>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}

			if (selectedComponentToAdd == "Transform")
			{
				go->AddComponent<Transform>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "UITransform")
			{
				go->AddComponent<UITransform>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "Animation")
			{
				go->AddComponent<Animation>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "RigidBody")
			{
				go->AddComponent<RigidBody>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "Collider2D")
			{
				go->AddComponent<Collider2D>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "TextRenderer")
			{
				go->AddComponent<TextRenderer>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "Button")
			{
				go->AddComponent<Button>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "Script")
			{
				go->AddComponent<Script>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
		}
	}


	/**
	 * @brief Templated Function based on the Component type to remove that component from the specified Entity.
	 * 
	 * @tparam T 
	 * @param go 
	 */
	template <typename T>
	void InspectorWindow::RemoveComponentButton(Entity go)
	{
		std::string typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');
		
		std::string buttonLabel = "Remove " + typeName.substr(pos + 1);
		if(ImGui::Button(buttonLabel.c_str()))
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(go))
			{
				CM_CORE_ERROR("Revmoing from an entity that does not exist");
				return;
			}

			ComponentManager::GetInstance()->RemoveComponent<T>(go);
			//gGOFactory->
			EntityManager::GetInstance()->RemoveComponent<T>(go);
		}
	}


	/**
	 * @brief Templated function for Prefab (Might be changed in future)
	 * 
	 * @tparam T 
	 * @param go 
	 */
	template<typename T> void InspectorWindow::InspectorTable(T* go)
	{
		static auto assetManager = AssetManager::GetInstance();
		auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		auto animMap = assetManager->GetAssetMap<AnimAtlas>();
		auto buttonMap = assetManager->GetAssetMap<Button>();

		if (go->HasComponent<Transform>())
		{
			Transform& selectedTransform = go->GetComponent<Transform>();
			//InspectorWindow::RemoveComponentButton<Transform>(id);
			if (ImGui::CollapsingHeader("Transform Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("Transform Table", 2, ImGuiTableFlags_Borders))
				{
					//Column Headers
					ImGui::TableNextColumn();
					ImGui::Text("Attribute");
					ImGui::TableNextColumn();
					ImGui::Text("Value");

					//Position (X,Y,Z)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xPos", &selectedTransform.GetPos().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yPos", &selectedTransform.GetPos().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Depth");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##zPos", &selectedTransform.GetDepth(), 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					// Rotation
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Rotation");
					ImGui::TableNextColumn();
					if (ImGui::DragFloat("##rot", &selectedTransform.GetRot(), 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
					{
						// Wrap the rotation value between 0 and 360 degrees
						selectedTransform.Rot(fmodf(selectedTransform.Rot(), 360.0f));
						if (selectedTransform.Rot() < 0.0f)
						{
							selectedTransform.GetRot() += 360.0f;
						}
					}

					// Scale (xScale, yScale)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xScale", &selectedTransform.GetScale().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yScale", &selectedTransform.GetScale().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
					ImGui::EndTable();
				}
			}
		}
		else if (go->HasComponent<UITransform>())
		{
			UITransform& selectedUITransform = go->GetComponent<UITransform>();
			//InspectorWindow::RemoveComponentButton<UITransform>(id);
			if (ImGui::CollapsingHeader("UI Transform Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("UI Transform Table", 2, ImGuiTableFlags_Borders))
				{
					//Column Headers
					ImGui::TableNextColumn();
					ImGui::Text("Attribute");
					ImGui::TableNextColumn();
					ImGui::Text("Value");

					// Position x and y
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xPos", &selectedUITransform.GetPos().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yPos", &selectedUITransform.GetPos().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Depth");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##Depth", &selectedUITransform.GetDepth(), 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					// Scale (xScale, yScale)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xScale", &selectedUITransform.GetScale().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yScale", &selectedUITransform.GetScale().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					//ImGui::TableNextRow();
					//ImGui::TableNextColumn();
					//ImGui::Text("Update Object");
					//ImGui::TableNextColumn();
					//std::string UpdateGO = "Update " + selectedGO->GetName();
					//if (ImGui::Button(UpdateGO.c_str()))
					//{
					//	
					//	//gGOFactory->Destroy(selectedEntity);
					//	//selectedGO = nullptr;
					//}
					ImGui::EndTable();
				}
			}
		}

		if (go->HasComponent<Renderer>())
		{
			Renderer& render = go->GetComponent<Renderer>();
			if (ImGui::CollapsingHeader("Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<Renderer>(id);
				if (ImGui::BeginTable("Renderer Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Model");

					ImGui::TableNextColumn();
					ImGui::Text("%s", render.model.c_str());
					//ImGui::SameLine();
					//if (ImGui::Button("v"))
					//{
					//	ImGui::OpenPopup("Model Select");
					//}

					//if (ImGui::BeginPopup("Model Select"))
					//{
					//	for (const auto& entry : primitiveMap->mAssetMap)
					//	{
					//		if (ImGui::Button(entry.first.c_str()))
					//		{
					//			render.model = entry.first;
					//			ImGui::CloseCurrentPopup();
					//		}
					//	}
					//	ImGui::EndPopup();
					//}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Texture");
					ImGui::TableNextColumn();
					ImGui::Text("%s", render.Texture().c_str());
					ImGui::SameLine();
					if (ImGui::Button("v##."))
					{
						ImGui::OpenPopup("Texture Select");
					}

					if (ImGui::BeginPopup("Texture Select"))
					{
						for (const auto& entry : textureMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								render.Texture(entry.first);
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::EndTable();
				}
			}
		}
		if (go->HasComponent<Animation>())
		{
			Animation& anim = go->GetComponent<Animation>();
			if (ImGui::CollapsingHeader("Animation Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<Animation>(id);

				if (ImGui::BeginTable("Animation Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Animation");

					ImGui::TableNextColumn();
					ImGui::Text("%s", anim.animAtlas.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v##"))
					{
						ImGui::OpenPopup("Animation Select");
					}

					if (ImGui::BeginPopup("Animation Select"))
					{
						for (const auto& entry : animMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								anim.animAtlas = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}

					/*ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("MaxTime");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxTime", &anim.maxTime, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");*/
					ImGui::EndTable();
				}
			}
		}

		// render rigibody data
		if (go->HasComponent<RigidBody>())
		{
			RigidBody& rb = go->GetComponent<RigidBody>();
			if (ImGui::CollapsingHeader("Rigid Body Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<RigidBody>(id);
				if (ImGui::BeginTable("Rigidbody Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Velocity X");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##VelocityX", &rb.velocity.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Velocity Y");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##VelocityY", &rb.velocity.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Mass");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##Mass", &rb.mass, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Gravity");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##Gravity", &rb.gravity, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Rigidbody Type");
					ImGui::TableNextColumn();
					switch (rb.objectType)
					{
					case 0:
					{
						ImGui::Text("Static");
						break;
					}
					case 1:
					{
						ImGui::Text("Kinematic");

						break;
					}
					case 2:
					{
						ImGui::Text("Dynamic");

						break;
					}
					}
					/*ImGui::Text("%s", text.font.c_str());*/
					ImGui::SameLine();
					if (ImGui::Button("v###"))
					{
						ImGui::OpenPopup("ObjectType");
					}

					if (ImGui::BeginPopup("ObjectType"))
					{
						for (int i = 0; i < rbTypes::MAX_TYPES; ++i)
						{
							switch (i)
							{
							case 0:
							{
								if (ImGui::Button("Static"))
								{
									rb.objectType = rbTypes::STATIC;
									ImGui::CloseCurrentPopup();
								}
								break;

							}
							case 1:
							{
								if (ImGui::Button("Kinematic"))
								{
									rb.objectType = rbTypes::KINEMATIC;
									ImGui::CloseCurrentPopup();
								}
								break;
							}
							case 2:
							{
								if (ImGui::Button("Dynamic"))
								{
									rb.objectType = rbTypes::DYNAMIC;
									ImGui::CloseCurrentPopup();
								}
								break;
							}
							}

						}
						ImGui::EndPopup();
					}

					ImGui::EndTable();
				}
			}
		}

		// render collider data
		if (go->HasComponent<Collider2D>())
		{
			Collider2D& col = go->GetComponent<Collider2D>();
			if (ImGui::CollapsingHeader("Collider Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<Collider2D>(id);
				if (ImGui::BeginTable("Collider2D Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Shape");
					ImGui::TableNextColumn();
					ImGui::Text("%s", col.shape.c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Min X");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MinX", &col.min.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Min Y");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MinY", &col.min.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Max X");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxX", &col.max.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Max Y");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxY", &col.max.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::EndTable();
				}
			}
		}

		// show text 
		// color picker
		if (go->HasComponent<TextRenderer>())
		{
			TextRenderer& text = go->GetComponent<TextRenderer>();
			if (ImGui::CollapsingHeader("Text Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<TextRenderer>(id);
				if (ImGui::BeginTable("TextRenderer Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Text");

					char buffer[256];
					strncpy_s(buffer, text.txt.c_str(), sizeof(buffer));
					buffer[sizeof(buffer) - 1] = '\0';

					ImGui::TableNextColumn();
					if (ImGui::InputText("##Text", buffer, sizeof(buffer)))
					{
						text.txt = buffer;
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Font");
					ImGui::TableNextColumn();
					ImGui::Text("%s", text.font.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v####"))
					{
						ImGui::OpenPopup("Font Select");
					}

					if (ImGui::BeginPopup("Font Select"))
					{
						for (const auto& entry : fontMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								text.font = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Color");

					// Add RGB color picker
					float color[3] = { text.colorR, text.colorG, text.colorB };
					ImGui::TableNextColumn();
					if (ImGui::ColorEdit3("##Color", color))
					{
						// Update the component's color with the selected values
						text.colorR = color[0];
						text.colorG = color[1];
						text.colorB = color[2];
					}

					ImGui::EndTable();
				}
			}
		}

		if (go->HasComponent<Button>())
		{
			Button& butt = go->GetComponent<Button>();
			if (ImGui::CollapsingHeader("Button Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<Button>(id);
				if (ImGui::BeginTable("Button Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Pressed Image");
					ImGui::TableNextColumn();
					ImGui::Text(butt.ButtonImagePressed.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v#####"))
					{
						ImGui::OpenPopup("Pressed Image Select");
					}
					if (ImGui::BeginPopup("Pressed Image Select"))
					{
						for (const auto& entry : textureMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								butt.ButtonImagePressed = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("isPressed = %s", butt.isPressed ? "true" : "false");
					ImGui::TableNextColumn();
					if (ImGui::Button("Toggle Pressed State"))
					{
						butt.isPressed = !butt.isPressed;
					}
					ImGui::EndTable();
				}
			}
		}
		if (go->HasComponent<Script>())
		{
			Script& script = go->GetComponent<Script>();
			if (ImGui::CollapsingHeader("Script Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				//InspectorWindow::RemoveComponentButton<Script>(id);
				ImGui::Text(script.scriptName.c_str());
				ImGui::SameLine();
				if (ImGui::Button("v#####"))
				{
					ImGui::OpenPopup("Script Select");
				}

				if (ImGui::BeginPopup("Script Select"))
				{
					for (const auto& entry : gScriptSystem->mEntityClasses)
					{
						if (ImGui::Button(entry.first.c_str()))
						{
							script.scriptName = entry.first;
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}


			}
		}
	}
	/**
	 * @brief Templated function for GameObjects
	 * 
	 * @tparam T 
	 * @param go 
	 * @param id 
	 */
	template<typename T> void InspectorWindow::InspectorTable(T* go, Entity id)
	{
		static auto assetManager = AssetManager::GetInstance();
		auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		auto animMap = assetManager->GetAssetMap<AnimAtlas>();
		auto buttonMap = assetManager->GetAssetMap<Button>();

		if (go->HasComponent<Transform>())
		{
			Transform& selectedTransform = go->GetComponent<Transform>();
			//InspectorWindow::RemoveComponentButton<Transform>(id);
			if (ImGui::CollapsingHeader("Transform Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("Transform Table", 2, ImGuiTableFlags_Borders))
				{
					//Column Headers
					ImGui::TableNextColumn();
					ImGui::Text("Attribute");
					ImGui::TableNextColumn();
					ImGui::Text("Value");

					//Position (X,Y,Z)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xPos", &selectedTransform.GetPos().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yPos", &selectedTransform.GetPos().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Depth");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##zPos", &selectedTransform.GetDepth(), 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					// Rotation
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Rotation");
					ImGui::TableNextColumn();
					if(ImGui::DragFloat("##rot", &selectedTransform.GetRot(), 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
					{
						// Wrap the rotation value between 0 and 360 degrees
						selectedTransform.Rot(fmodf(selectedTransform.Rot(), 360.0f));
						if (selectedTransform.Rot() < 0.0f)
						{
							selectedTransform.GetRot() += 360.0f;
						}
					}

					// Scale (xScale, yScale)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xScale", &selectedTransform.GetScale().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yScale", &selectedTransform.GetScale().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");
					ImGui::EndTable();
				}
			}
		}
		else if (go->HasComponent<UITransform>())
		{
			UITransform& selectedUITransform = go->GetComponent<UITransform>();
			//InspectorWindow::RemoveComponentButton<UITransform>(id);
			if (ImGui::CollapsingHeader("UI Transform Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginTable("UI Transform Table", 2, ImGuiTableFlags_Borders))
				{
					//Column Headers
					ImGui::TableNextColumn();
					ImGui::Text("Attribute");
					ImGui::TableNextColumn();
					ImGui::Text("Value");

					// Position x and y
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xPos", &selectedUITransform.GetPos().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yPos");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yPos", &selectedUITransform.GetPos().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Depth");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##Depth", &selectedUITransform.GetDepth(), 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					// Scale (xScale, yScale)
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("xScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##xScale", &selectedUITransform.GetScale().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("yScale");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##yScale", &selectedUITransform.GetScale().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

					//ImGui::TableNextRow();
					//ImGui::TableNextColumn();
					//ImGui::Text("Update Object");
					//ImGui::TableNextColumn();
					//std::string UpdateGO = "Update " + selectedGO->GetName();
					//if (ImGui::Button(UpdateGO.c_str()))
					//{
					//	
					//	//gGOFactory->Destroy(selectedEntity);
					//	//selectedGO = nullptr;
					//}
					ImGui::EndTable();
				}
			}
		}

		if (go->HasComponent<Renderer>())
		{
			Renderer& render = go->GetComponent<Renderer>();
			if (ImGui::CollapsingHeader("Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<Renderer>(id);
				if (ImGui::BeginTable("Renderer Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Model");

					ImGui::TableNextColumn();
					ImGui::Text("%s", render.model.c_str());
					
					/*ImGui::SameLine();
					if (ImGui::Button("v"))
					{
						ImGui::OpenPopup("Model Select");
					}

					if (ImGui::BeginPopup("Model Select"))
					{
						for (const auto& entry : primitiveMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								render.model = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}*/
					
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Texture");
					ImGui::TableNextColumn();
					ImGui::Text("%s", render.Texture().c_str());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_PAYLOAD"))
						{
							std::string textureName = *(const std::string*)payload->Data;
							render.Texture(textureName);
						}

						ImGui::EndDragDropTarget();
					}
					ImGui::SameLine();
					if (ImGui::Button("v##."))
					{
						ImGui::OpenPopup("Texture Select");
					}

					if (ImGui::BeginPopup("Texture Select"))
					{
						for (const auto& entry : textureMap->mAssetMap)
						{
							if (entry.first.empty()) continue; // TODO: Find out why "" is being added to asset map
							if (ImGui::Button(entry.first.c_str()))
							{
								render.Texture(entry.first);
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::EndTable();
				}
			}

			//CheckForComponentChange<Renderer>(*go, render);
		}
		if (go->HasComponent<Animation>())
		{
			Animation& anim = go->GetComponent<Animation>();
			if (ImGui::CollapsingHeader("Animation Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<Animation>(id);

				if (ImGui::BeginTable("Animation Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Animation");

					ImGui::TableNextColumn();
					ImGui::Text("%s", anim.animAtlas.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v##"))
					{
						ImGui::OpenPopup("Animation Select");
					}

					if (ImGui::BeginPopup("Animation Select"))
					{
						for (const auto& entry : animMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								anim.animAtlas = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}

					/*ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("MaxTime");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxTime", &anim.maxTime, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");*/
					ImGui::EndTable();
				}
			}
		}
		
		// render rigibody data
		if (go->HasComponent<RigidBody>())
		{
			RigidBody rb = go->GetComponent<RigidBody>();
			bool modified = false;
			if (ImGui::CollapsingHeader("Rigid Body Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<RigidBody>(id);
				if (ImGui::BeginTable("Rigidbody Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Velocity X");
					ImGui::TableNextColumn();
					modified |= ImGui::DragFloat("##VelocityX", &rb.velocity.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Velocity Y");
					ImGui::TableNextColumn();
					modified |= ImGui::DragFloat("##VelocityY", &rb.velocity.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Mass");
					ImGui::TableNextColumn();
					modified |= ImGui::DragFloat("##Mass", &rb.mass, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Gravity");
					ImGui::TableNextColumn();
					modified |= ImGui::DragFloat("##Gravity", &rb.gravity, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Rigidbody Type");
					ImGui::TableNextColumn();
					switch (rb.objectType)
					{
					case 0:
					{
						ImGui::Text("Static");
						break;
					}
					case 1:
					{
						ImGui::Text("Kinematic");

						break;
					}
					case 2:
					{
						ImGui::Text("Dynamic");

						break;
					}
					}
					/*ImGui::Text("%s", text.font.c_str());*/
					ImGui::SameLine();
					if (ImGui::Button("v###"))
					{
						ImGui::OpenPopup("ObjectType");
					}

					if (ImGui::BeginPopup("ObjectType"))
					{
						for (int i = 0; i < rbTypes::MAX_TYPES; ++i)
						{
							switch (i)
							{
							case 0:
							{
								if (ImGui::Button("Static"))
								{
									modified = true;
									rb.objectType = rbTypes::STATIC;
									ImGui::CloseCurrentPopup();
								}
								break;

							}
							case 1:
							{
								if (ImGui::Button("Kinematic"))
								{
									modified = true;
									rb.objectType = rbTypes::KINEMATIC;
									ImGui::CloseCurrentPopup();
								}
								break;
							}
							case 2:
							{
								if (ImGui::Button("Dynamic"))
								{
									modified = true;
									rb.objectType = rbTypes::DYNAMIC;
									ImGui::CloseCurrentPopup();
								}
								break;
							}
							}

						}
						ImGui::EndPopup();
					}

					ImGui::EndTable();
				}
			}

			CheckForComponentChange<RigidBody>(*go, rb, modified);
		}

		// render collider data
		if (go->HasComponent<Collider2D>())
		{
			Collider2D& col = go->GetComponent<Collider2D>();
			if (ImGui::CollapsingHeader("Collider Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<Collider2D>(id);
				if (ImGui::BeginTable("Collider2D Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Shape");
					ImGui::TableNextColumn();
					ImGui::Text("%s", col.shape.c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Min X");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MinX", &col.min.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Min Y");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MinY", &col.min.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Max X");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxX", &col.max.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Max Y");
					ImGui::TableNextColumn();
					ImGui::DragFloat("##MaxY", &col.max.y, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");

					ImGui::EndTable();
				}
			}
		}

		// show text 
		// color picker
		if (go->HasComponent<TextRenderer>())
		{
			TextRenderer& text = go->GetComponent<TextRenderer>();
			if (ImGui::CollapsingHeader("Text Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<TextRenderer>(id);
				if (ImGui::BeginTable("TextRenderer Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Text");

					char buffer[256];
					
					strncpy_s(buffer, text.txt.c_str(), sizeof(buffer));
					buffer[sizeof(buffer) - 1] = '\0';

					ImGui::TableNextColumn();
					if (ImGui::InputText("##Text", buffer, sizeof(buffer)))
					{
						text.txt = buffer;
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Font");
					ImGui::TableNextColumn();
					ImGui::Text("%s", text.font.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v####"))
					{
						ImGui::OpenPopup("Font Select");
					}

					if (ImGui::BeginPopup("Font Select"))
					{
						for (const auto& entry : fontMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								text.font = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();	
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Color");

					// Add RGB color picker
					float color[3] = { text.colorR, text.colorG, text.colorB };
					ImGui::TableNextColumn();
					if (ImGui::ColorEdit3("##Color", color))
					{
						// Update the component's color with the selected values
						text.colorR = color[0];
						text.colorG = color[1];
						text.colorB = color[2];
					}

					ImGui::EndTable();
				}
			}
		}

		if (go->HasComponent<Button>())
		{
			Button& butt = go->GetComponent<Button>();
			if (ImGui::CollapsingHeader("Button Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<Button>(id);
				if (ImGui::BeginTable("Button Table", 2, ImGuiTableFlags_Borders))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Pressed Image");
					ImGui::TableNextColumn();
					ImGui::Text(butt.ButtonImagePressed.c_str());
					ImGui::SameLine();
					if (ImGui::Button("v#####"))
					{
						ImGui::OpenPopup("Pressed Image Select");
					}
					if (ImGui::BeginPopup("Pressed Image Select"))
					{
						for (const auto& entry : textureMap->mAssetMap)
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								butt.ButtonImagePressed = entry.first;
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::EndPopup();
					}
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("isPressed = %s", butt.isPressed ? "true" : "false");
					ImGui::TableNextColumn();
					if (ImGui::Button("Toggle Pressed State"))
					{
						butt.isPressed = !butt.isPressed;
					}
					ImGui::EndTable();
				}
			}
		}
		if (go->HasComponent<Script>())
		{
			Script& script = go->GetComponent<Script>();
			if (ImGui::CollapsingHeader("Script Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				InspectorWindow::RemoveComponentButton<Script>(id);
				ImGui::Text(script.scriptName.c_str());
				ImGui::SameLine();
				if (ImGui::Button("v#####"))
				{
					ImGui::OpenPopup("Script Select");
				}

				if (ImGui::BeginPopup("Script Select"))
				{
					for (const auto& entry : gScriptSystem->mEntityClasses)
					{
						if (ImGui::Button(entry.first.c_str()))
						{
							script.scriptName = entry.first;
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}

				std::shared_ptr<ScriptObject> scriptRef = gScriptSystem->GetScriptInstance(id);
				if (scriptRef != nullptr)
				{
					const auto& fields = scriptRef->GetScriptClass()->mFields;
					for (const auto& it : fields)
					{
						if (it.second.mType == ScriptFieldType::Float)
						{
							float data = scriptRef->GetFieldValue<float>(it.second.mName);
							if (ImGui::DragFloat(it.second.mName.c_str(), &data))
							{
								scriptRef->SetFieldValue(it.second.mName, data);
							}
						}
					}
				}
			}
		}
	}

	/**
	 * @brief Update function override for the Inspector Window
	 * 
	 */
	void InspectorWindow::Update()
	{
		if (ImGui::Begin(mTitle))
		{
			if (HierarchyWindow::selectedGO != nullptr)
			{
				ImGui::Text("Selected Game Object: %s", HierarchyWindow::selectedGO->GetName().c_str());
				Entity selectedEntity = HierarchyWindow::selectedGO->GetID();

				AddComponentButton(HierarchyWindow::selectedGO);

				InspectorTable<GameObject>(HierarchyWindow::selectedGO, selectedEntity);

				std::string saveGO = "Create " + HierarchyWindow::selectedGO->GetName() + " as a Prefab";
				if (ImGui::Button(saveGO.c_str()))
				{
					NewPrefabMsg msg(HierarchyWindow::selectedGO->GetID());
					mMessages.push_back(std::make_shared<NewPrefabMsg>(msg));
					//gGOFactory->Destroy(selectedEntity);
					//HierarchyWindow::selectedGO = nullptr;
				}

				std::string destroyGO = "Destroy " + HierarchyWindow::selectedGO->GetName();
				if (ImGui::Button(destroyGO.c_str()))
				{
					gGOFactory->Destroy(selectedEntity);
					HierarchyWindow::selectedGO = nullptr;
				}
			}

			if (HierarchyWindow::inspectedPrefab != nullptr)
			{
				ImGui::Text("Selected Prefab: %s", HierarchyWindow::inspectedPrefab->GetName().c_str());
				Entity selectedPrefabID = HierarchyWindow::inspectedPrefab->GetID();
				UNUSED(selectedPrefabID);
				InspectorTable<Prefab>(HierarchyWindow::inspectedPrefab);

				if (ImGui::Button("Save Changes to Prefab"))
				{
					Serializer.SerializePrefab(*AssetWindow::selectedPrefab);
					ModifyPrefabMsg msg(*AssetWindow::selectedPrefab);
					mMessages.push_back(std::make_shared<ModifyPrefabMsg>(msg));
				}


				if (ImGui::Button("Create Prefab"))
				{
					gGOFactory->CreatePrefab(AssetWindow::selectedPrefab->GetName());
					AssetWindow::selectedPrefab = nullptr;
					HierarchyWindow::inspectedPrefab = nullptr;
					HierarchyWindow::mShowScene = true;
				}
			}
		}
		ImGui::End();
	}
}
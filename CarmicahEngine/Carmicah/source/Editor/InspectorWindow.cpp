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
#include "InspectorWindow.h"
#include "HierarchyWindow.h"

namespace Carmicah
{
	InspectorWindow::InspectorWindow() : EditorWindow("Inspector", ImVec2(900, 300), ImVec2(0, 0)) 
	{ 
		mIsVisible = true; 
	}

	std::string InspectorWindow::selectedComponentToAdd = "";
	/**
	 * @brief A drop down to select which component to add to the specified go.
	 *
	 * @param go
	 */
	template<typename T>
	void InspectorWindow::AddComponentButton(T* go)
	{
		std::vector<const char*> componentsToAdd;

		static int selectedIndex;

		componentsToAdd.push_back("");

		if (!go->HasComponent<Renderer>())
		{
			componentsToAdd.push_back("Renderer");
		}
		if (!go->HasComponent<Transform>() && !go->HasComponent<UITransform>())
		{
			componentsToAdd.push_back("Transform");
		}
		if (!go->HasComponent<UITransform>() && !go->HasComponent<Transform>())
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


		if (ImGui::Combo("##Component", &selectedIndex, componentsToAdd.data(), (int)componentsToAdd.size()))
		{
			selectedComponentToAdd = componentsToAdd[selectedIndex];
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Component"))
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
	bool InspectorWindow::RemoveComponentButton(GameObject* go)
	{
		Entity id = go->GetID();
		std::string typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');

		std::string buttonLabel = "Remove " + typeName.substr(pos + 1);
		if (ImGui::Button(buttonLabel.c_str()))
		{
			if (!EntityManager::GetInstance()->DoesEntityExist(id))
			{
				CM_CORE_ERROR("Revmoing from an entity that does not exist");
				return false;
			}

			ComponentManager::GetInstance()->RemoveComponent<T>(id);
			//gGOFactory->
			EntityManager::GetInstance()->RemoveComponent<T>(id);
		}

		return true;
	}

	template <typename T>
	bool InspectorWindow::RemoveComponentButton(Prefab* go)
	{
		std::string typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');
		std::string buttonLabel = "Remove " + typeName.substr(pos + 1);
		if (ImGui::Button(buttonLabel.c_str()))
		{
			if (go->RemoveComponent<T>())
				return true;
		}
		return false;
	}

	template<typename T>
	void InspectorWindow::RenderTransformTable(T* go, TABLETYPE type)
	{
		UNUSED(type);
		Transform& selectedTransform = go->GetComponent<Transform>();
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
				Vec2f pos = selectedTransform.Pos();
				if (ImGui::DragFloat("##xPos", &pos.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Pos(pos);
					if (selectedTransform.parent != 0)
					{
						//Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(selectedTransform.parent);

						//float worldX = parentTransform.worldSpace.m00 * selectedTransform.Pos().x + parentTransform.worldSpace.m01 * selectedTransform.Pos().y + parentTransform.worldSpace.m02;
						//float worldY = parentTransform.worldSpace.m10 * selectedTransform.Pos().x + parentTransform.worldSpace.m11 * selectedTransform.Pos().y + parentTransform.worldSpace.m12;
					}
					else
					{
					}
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yPos");
				ImGui::TableNextColumn();
				//Vec2f pos = data.Pos();

				if (ImGui::DragFloat("##yPos", &pos.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Pos(pos);
					if (selectedTransform.parent != 0)
					{
						//Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(selectedTransform.parent);
						//CM_CORE_INFO("Parent World X : " + std::to_string(parentTransform.worldSpace.m20) + ", Parent World Y : " + std::to_string(parentTransform.worldSpace.m21));

						//float worldX = parentTransform.worldSpace.m00 * selectedTransform.Pos().x + parentTransform.worldSpace.m01 * selectedTransform.Pos().y + parentTransform.worldSpace.m02;
						//float worldY = parentTransform.worldSpace.m10 * selectedTransform.Pos().x + parentTransform.worldSpace.m11 * selectedTransform.Pos().y + parentTransform.worldSpace.m12;

					}
					else
					{
					}
				}
				//CM_CORE_INFO("World X : " + std::to_string(selectedTransform.WorldPos().x) + ", World Y : " + std::to_string(selectedTransform.WorldPos().y));
				//CM_CORE_INFO("World X : " + std::to_string(selectedTransform.worldSpace.m20) + ", World Y : " + std::to_string(selectedTransform.worldSpace.m21));

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Depth");
				ImGui::TableNextColumn();
				float depth = selectedTransform.Depth();
				if (ImGui::DragFloat("##zPos", &depth, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Depth(depth);
				}

				// Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				float rotVar = selectedTransform.Rot();
				if (ImGui::DragFloat("##rot", &rotVar, 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.GetRot() = rotVar;
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
				Vec2f scale = selectedTransform.Scale();
				if (ImGui::DragFloat("##xScale", &scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Scale(scale);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##yScale", &scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Scale(scale);
				}
				ImGui::EndTable();
			}
		}
	}

	template<typename T>
	void InspectorWindow::RenderUITransformTable(T* go, TABLETYPE type)
	{
		UNUSED(type);
		UITransform& selectedUITransform = go->GetComponent<UITransform>();
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

				//Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				if(ImGui::DragFloat("##Rotation", &selectedUITransform.GetRot(), 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.Rot(fmodf(selectedUITransform.GetRot(), 360.0f));
					if (selectedUITransform.GetRot() < 0.0f)
					{
						selectedUITransform.GetRot() += 360.0f;
					}
				}

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

	template<typename T>
	void InspectorWindow::RenderRenderingTable(T* go, TABLETYPE type)
	{
		auto textureMap = AssetManager::GetInstance()->GetAssetMap<Texture>();
		Renderer& render = go->GetComponent<Renderer>();
		if (ImGui::CollapsingHeader("Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<Renderer>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<Renderer>(go))
					return;
				break;
			}
			default:
				break;
			}
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
				ImGui::SameLine();
				if (ImGui::Button("v##."))
				{
					ImGui::OpenPopup("Texture Select");
				}
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
				ImGui::SetNextWindowSize(ImVec2(300, 500));
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
	}
	
	template<typename T>
	void InspectorWindow::RenderAnimationTable(T* go, TABLETYPE type)
	{
		auto animMap = AssetManager::GetInstance()->GetAssetMap<AnimAtlas>();
		Animation& anim = go->GetComponent<Animation>();
		if (ImGui::CollapsingHeader("Animation Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<Animation>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<Animation>(go))
					return;
				break;
			}
			default:
				break;
			}
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
						if(entry.first.empty()){}
						else
						{
							if (ImGui::Button(entry.first.c_str()))
							{
								anim.animAtlas = entry.first;
								ImGui::CloseCurrentPopup();
							}
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

	template<typename T>
	bool InspectorWindow::RenderRigidBodyTable(T* go, TABLETYPE type)
	{
		bool modified = false;
		RigidBody& rb = go->GetComponent<RigidBody>();
		if (ImGui::CollapsingHeader("Rigid Body Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<RigidBody>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<RigidBody>(go))
					return false;
				break;
			}
			default:
				break;
			}
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

		return modified;
	}

	template<typename T>
	void InspectorWindow::RenderCollider2DTable(T* go, TABLETYPE type)
	{
		Collider2D& col = go->GetComponent<Collider2D>();
		if (ImGui::CollapsingHeader("Collider Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<Collider2D>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<Collider2D>(go))
					return;
				break;
			}
			default:
				break;
			}
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

				ImGui::TableNextColumn();
				ImGui::Text("Custom Width");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##CustomWidth", &col.customWidth, 0.1f, 0.5f, FLT_MAX, "%.3f");

				ImGui::TableNextColumn();
				ImGui::Text("Custom Height");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##CustomHeight", &col.customHeight, 0.1f, 0.5f, FLT_MAX, "%.3f");

				ImGui::EndTable();
			}
		}
	}

	template<typename T>
	void InspectorWindow::RenderTextRenderTable(T* go, TABLETYPE type)
	{
		auto fontMap = AssetManager::GetInstance()->GetAssetMap<Font>();
		TextRenderer& text = go->GetComponent<TextRenderer>();
		if (ImGui::CollapsingHeader("Text Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<TextRenderer>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<TextRenderer>(go))
					return;
				break;
			}
			default:
				break;
			}
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

	template<typename T>
	void InspectorWindow::RenderButtonTable(T* go, TABLETYPE type)
	{
		auto textureMap = AssetManager::GetInstance()->GetAssetMap<Texture>();
		Button& butt = go->GetComponent<Button>();
		if (ImGui::CollapsingHeader("Button Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<Button>(go);
				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<Button>(go))
					return;
				break;
			}
			default:
				break;
			}
			if (ImGui::BeginTable("Button Table", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Pressed Image");
				ImGui::SameLine();
				if (ImGui::Button("v#####"))
				{
					ImGui::OpenPopup("Pressed Image Select");
				}
				ImGui::TableNextColumn();
				ImGui::Text(butt.ButtonImagePressed.c_str());
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

	template<typename T>
	void InspectorWindow::RenderScriptTable(T* go, TABLETYPE type)
	{
		Script& script = go->GetComponent<Script>();
		unsigned int id = go->GetID();
		if (ImGui::CollapsingHeader("Script Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				InspectorWindow::RemoveComponentButton<Script>(go);
				ImGui::Text(script.scriptName.c_str());
				if(script.scriptName.empty())
				{
					ImGui::SameLine();
					if (ImGui::Button("v#####"))
					{
						ImGui::OpenPopup("Script Select");
					}
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
								gScriptSystem->UpdateScriptComponent(id);
							}
						}
						else if (it.second.mType == ScriptFieldType::Bool)
						{
							bool data = scriptRef->GetFieldValue<bool>(it.second.mName);
							if (ImGui::Checkbox(it.second.mName.c_str(), &data))
							{
								scriptRef->SetFieldValue(it.second.mName, data);
								gScriptSystem->UpdateScriptComponent(id);
							}
						}
						else if (it.second.mType == ScriptFieldType::String)
						{
							std::string str = scriptRef->GetFieldValue<std::string>(it.second.mName);
							char buffer[128];
							std::strncpy(buffer, str.c_str(), sizeof(buffer) - 1);
							buffer[sizeof(str)] = '\0';

							if (ImGui::InputText(it.second.mName.c_str(), buffer, sizeof(buffer)))
							{
								scriptRef->SetFieldValue<std::string>(it.second.mName, buffer);
								gScriptSystem->UpdateScriptComponent(id);
							}
						}
					}
				}

				break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<Script>(go))
					return;
				ImGui::Text(script.scriptName.c_str());
				if (script.scriptName.empty())
				{
					ImGui::SameLine();
					if (ImGui::Button("v#####"))
					{
						ImGui::OpenPopup("Script Select");
					}
				}

				for (auto& [fieldName, fieldValue] : script.scriptableFieldMap)
				{
					std::visit([&](auto&& arg)
						{
							using T = std::decay_t<decltype(arg)>;
							if constexpr (std::is_same_v<T, int>)
							{
								if (ImGui::DragInt(fieldName.c_str(), &arg))
								{

								}
							}
							else if constexpr (std::is_same_v<T, float>)
							{
								if (ImGui::DragFloat(fieldName.c_str(), &arg))
								{

								}
							}
							else if constexpr (std::is_same_v<T, bool>)
							{
								if (ImGui::Checkbox(fieldName.c_str(), &arg))
								{

								}
							}
							else if constexpr (std::is_same_v<T, std::string>)
							{
								char buffer[128];
								std::strncpy(buffer, arg.c_str(), sizeof(buffer) - 1);
								buffer[sizeof(buffer) - 1] = '\0';

								if (ImGui::InputText(fieldName.c_str(), buffer, sizeof(buffer)))
								{
									arg = std::string(buffer);
								}
							}
						}, fieldValue);
				}
				break;
			}
			default:
				break;
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

	/**
	 * @brief Templated function for GameObjects
	 * 
	 * @tparam T 
	 * @param go 
	 * @param id 
	 */
	template<typename T> void InspectorWindow::InspectorTable(T* go, TABLETYPE type)
	{
		if (go->HasComponent<Transform>())
		{
			RenderTransformTable(go, type);
		}
		else if (go->HasComponent<UITransform>())
		{
			RenderUITransformTable(go, type);
		}

		if (go->HasComponent<Renderer>())
		{
			RenderRenderingTable(go, type);
			//CheckForComponentChange<Renderer>(*go, render);
		}
		if (go->HasComponent<Animation>())
		{
			RenderAnimationTable(go, type);
		}
		
		// render rigibody data
		if (go->HasComponent<RigidBody>())
		{
			bool modified = false;
			RigidBody& rb = go->GetComponent<RigidBody>();
			modified = RenderRigidBodyTable(go, type);
			if (type == PREFAB)
			{
				CheckForComponentChange<RigidBody>(*go, rb, modified);
			}
		}

		// render collider data
		if (go->HasComponent<Collider2D>())
		{
			RenderCollider2DTable(go, type);
		}

		// show text 
		// color picker
		if (go->HasComponent<TextRenderer>())
		{
			RenderTextRenderTable(go, type);
		}

		if (go->HasComponent<Button>())
		{
			RenderButtonTable(go, type);
		}
		if (go->HasComponent<Script>())
		{
			RenderScriptTable(go, type);
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
				char inputBuffer[1024];
				std::strncpy(inputBuffer, HierarchyWindow::selectedGO->GetName().c_str(), sizeof(inputBuffer)- 1);
				ImGui::Text("Selected Game Object:");
				ImGui::SameLine();
				if (ImGui::InputText("##RenameGO", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (inputBuffer[0] != '\0')
					{
						std::string newName = gGOFactory->CreateGOName(inputBuffer);
						gGOFactory->UpdateGOName(*HierarchyWindow::selectedGO, newName);
						//HierarchyWindow::selectedGO->SetName(newName);
					}
					if (inputBuffer[0] == '\0')
					{
						CM_CORE_ERROR("Trying to make empty GO name!");
					}
				}
				Entity selectedEntity = HierarchyWindow::selectedGO->GetID();

				AddComponentButton(HierarchyWindow::selectedGO);

				InspectorTable<GameObject>(HierarchyWindow::selectedGO, GAMEOBJECT);

				ImGui::Dummy(ImVec2(0, 20));

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

				std::string goCloneButton = "Clone GO";
				if (ImGui::Button(goCloneButton.c_str()))
				{
					if (HierarchyWindow::selectedGO != nullptr)
					{
						gGOFactory->CloneGO(*HierarchyWindow::selectedGO);
					}
					//gGOFactory->CreateGO();
				}
			}

			if (HierarchyWindow::inspectedPrefab != nullptr)
			{
				ImGui::Text("Selected Prefab: %s", HierarchyWindow::inspectedPrefab->GetName().c_str());
				Entity selectedPrefabID = HierarchyWindow::inspectedPrefab->GetID();
				UNUSED(selectedPrefabID);

				AddComponentButton<Prefab>(HierarchyWindow::inspectedPrefab);

				InspectorTable<Prefab>(HierarchyWindow::inspectedPrefab, PREFAB);

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
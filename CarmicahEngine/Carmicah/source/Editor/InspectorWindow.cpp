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
		//textureMap = assetManager->GetAssetMap<Texture>();
		//fontMap = assetManager->GetAssetMap<Font>();
		//animMap = assetManager->GetAssetMap<AnimAtlas>();
		//buttonMap = assetManager->GetAssetMap<Button>();
	}

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
	void InspectorWindow::RemoveComponentButton(Entity go)
	{
		std::string typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');

		std::string buttonLabel = "Remove " + typeName.substr(pos + 1);
		if (ImGui::Button(buttonLabel.c_str()))
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


	template<typename EntityID>
	void InspectorWindow::RenderTransformTable(Transform& data, EntityID id)
	{

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
				Vec2f pos = data.Pos();
				if (ImGui::DragFloat("##xPos", &pos.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.Pos(pos);
					if (data.parent != 0)
					{
						Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(data.parent);

						float worldX = parentTransform.worldSpace.m00 * data.Pos().x + parentTransform.worldSpace.m01 * data.Pos().y + parentTransform.worldSpace.m02;
						float worldY = parentTransform.worldSpace.m10 * data.Pos().x + parentTransform.worldSpace.m11 * data.Pos().y + parentTransform.worldSpace.m12;
						data.WorldPos(worldX, worldY);
					}
					else
					{
						data.WorldPos(data.Pos());
					}
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yPos");
				ImGui::TableNextColumn();
				//Vec2f pos = data.Pos();

				if (ImGui::DragFloat("##yPos", &pos.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.Pos(pos);
					if (data.parent != 0)
					{
						Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(data.parent);
						//CM_CORE_INFO("Parent World X : " + std::to_string(parentTransform.worldSpace.m20) + ", Parent World Y : " + std::to_string(parentTransform.worldSpace.m21));

						float worldX = parentTransform.worldSpace.m00 * data.Pos().x + parentTransform.worldSpace.m01 * data.Pos().y + parentTransform.worldSpace.m02;

						float worldY = parentTransform.worldSpace.m10 * data.Pos().x + parentTransform.worldSpace.m11 * data.Pos().y + parentTransform.worldSpace.m12;

						data.WorldPos(worldX, worldY);
					}
					else
					{
						data.WorldPos(data.Pos());
					}
				}
				//CM_CORE_INFO("World X : " + std::to_string(selectedTransform.WorldPos().x) + ", World Y : " + std::to_string(selectedTransform.WorldPos().y));
				//CM_CORE_INFO("World X : " + std::to_string(selectedTransform.worldSpace.m20) + ", World Y : " + std::to_string(selectedTransform.worldSpace.m21));

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Depth");
				ImGui::TableNextColumn();
				float depth = data.Depth();
				if (ImGui::DragFloat("##zPos", &depth, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.Depth(depth);
				}

				// Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				float rotVar = data.Rot();
				if (ImGui::DragFloat("##rot", &rotVar, 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.GetRot() = rotVar;
					// Wrap the rotation value between 0 and 360 degrees
					data.Rot(fmodf(data.Rot(), 360.0f));
					if (data.Rot() < 0.0f)
					{
						data.GetRot() += 360.0f;
					}
				}

				// Scale (xScale, yScale)
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("xScale");
				ImGui::TableNextColumn();
				Vec2f scale = data.Scale();
				if (ImGui::DragFloat("##xScale", &scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.Scale(scale);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				if (ImGui::DragFloat("##yScale", &scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					data.Scale(scale);
				}
				ImGui::EndTable();
			}
		}
	}

	template<typename EntityID>
	void InspectorWindow::RenderUITransformTable(UITransform& data, EntityID id)
	{
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
				ImGui::DragFloat("##xPos", &data.GetPos().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yPos");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##yPos", &data.GetPos().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Depth");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##Depth", &data.GetDepth(), 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

				// Scale (xScale, yScale)
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("xScale");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##xScale", &data.GetScale().x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##yScale", &data.GetScale().y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f");

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

	template<typename EntityID>
	void InspectorWindow::RenderRenderingTable(Renderer& data, EntityID id)
	{
		auto& textureMap = AssetManager::GetInstance()->GetAssetMap<Texture>();

		if (ImGui::CollapsingHeader("Renderer Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			InspectorWindow::RemoveComponentButton<Renderer>(id);
			if (ImGui::BeginTable("Renderer Table", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Model");

				ImGui::TableNextColumn();
				ImGui::Text("%s", data.model.c_str());

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
				ImGui::Text("%s", data.Texture().c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_PAYLOAD"))
					{
						std::string textureName = *(const std::string*)payload->Data;
						data.Texture(textureName);
					}

					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (ImGui::Button("v##."))
				{
					ImGui::OpenPopup("Texture Select");
				}
				ImGui::SetNextWindowSize(ImVec2(300, 500));
				if (ImGui::BeginPopup("Texture Select"))
				{
					for (const auto& entry : textureMap->mAssetMap)
					{
						if (entry.first.empty()) continue; // TODO: Find out why "" is being added to asset map
						if (ImGui::Button(entry.first.c_str()))
						{
							data.Texture(entry.first);
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}
				ImGui::EndTable();
			}
		}
	}
	
	template<typename EntityID>
	void InspectorWindow::RenderAnimationTable(Animation& anim, EntityID id)
	{
		auto& animMap = AssetManager::GetInstance()->GetAssetMap<AnimAtlas>();
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

	template<typename EntityID>
	bool InspectorWindow::RenderRigidBodyTable(RigidBody& rb, EntityID id)
	{
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

		return modified;
	}

	template<typename EntityID>
	void InspectorWindow::RenderCollider2DTable(Collider2D& col, EntityID id)
	{
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

	template<typename EntityID>
	void InspectorWindow::RenderTextRenderTable(TextRenderer& text, EntityID id)
	{
		auto& fontMap = AssetManager::GetInstance()->GetAssetMap<Font>();
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

	template<typename EntityID>
	void InspectorWindow::RenderButtonTable(Button& butt, EntityID id)
	{
		auto& textureMap = AssetManager::GetInstance()->GetAssetMap<Texture>();

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

	template<typename EntityID>
	void InspectorWindow::RenderScriptTable(Script& script, EntityID id)
	{
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
		if (go->HasComponent<Transform>())
		{
			Transform& selectedTransform = go->GetComponent<Transform>();
			//InspectorWindow::RemoveComponentButton<Transform>(id);
			RenderTransformTable(selectedTransform, id);
		}
		else if (go->HasComponent<UITransform>())
		{
			UITransform& selectedUITransform = go->GetComponent<UITransform>();
			
			RenderUITransformTable(selectedUITransform, id);
		}

		if (go->HasComponent<Renderer>())
		{
			Renderer& render = go->GetComponent<Renderer>();
			
			RenderRenderingTable(render, id);
			//CheckForComponentChange<Renderer>(*go, render);
		}
		if (go->HasComponent<Animation>())
		{
			Animation& anim = go->GetComponent<Animation>();
			RenderAnimationTable(anim, id);
		}
		
		// render rigibody data
		if (go->HasComponent<RigidBody>())
		{
			RigidBody& rb = go->GetComponent<RigidBody>();
			bool modified = false;
			modified = RenderRigidBodyTable(rb, id);

			CheckForComponentChange<RigidBody>(*go, rb, modified);
		}

		// render collider data
		if (go->HasComponent<Collider2D>())
		{
			Collider2D& col = go->GetComponent<Collider2D>();
			RenderCollider2DTable(col, id);
		}

		// show text 
		// color picker
		if (go->HasComponent<TextRenderer>())
		{
			TextRenderer& text = go->GetComponent<TextRenderer>();
			RenderTextRenderTable(text, id);
		}

		if (go->HasComponent<Button>())
		{
			Button& butt = go->GetComponent<Button>();
			RenderButtonTable(butt, id);
		}
		if (go->HasComponent<Script>())
		{
			Script& script = go->GetComponent<Script>();
			RenderScriptTable(script, id);
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
						HierarchyWindow::selectedGO->SetName(newName);
					}
					if (inputBuffer[0] == '\0')
					{
						CM_CORE_ERROR("Trying to make empty GO name!");
					}
				}
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
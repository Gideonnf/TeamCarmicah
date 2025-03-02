/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			InspectorWindow.cpp

 author:		Nicholas Lai (90%)
 co-author(s):	Micah Lim (10%)

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
		if (!go->HasComponent<StateMachine>())
		{
			componentsToAdd.push_back("StateMachine");
		}
		if (!go->HasComponent<ParticleEmitter>())
		{
			componentsToAdd.push_back("Particle");
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
			if (selectedComponentToAdd == "StateMachine")
			{
				go->AddComponent<StateMachine>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
			if (selectedComponentToAdd == "Particle")
			{
				go->AddComponent<ParticleEmitter>();
				selectedComponentToAdd = "";
				selectedIndex = 0;
			}
		}
	}

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

			EntityManager::GetInstance()->RemoveComponent<T>(id);
			ComponentManager::GetInstance()->RemoveComponent<T>(id);
			//gGOFactory->
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
		float tempValue = 0;
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
				tempValue = selectedTransform.GetPos().x;
				if (ImGui::DragFloat("##xPos", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.PosX(tempValue);
					if (selectedTransform.ParentID() != 0)
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
				tempValue = selectedTransform.GetPos().y;
				if (ImGui::DragFloat("##yPos", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.PosY(tempValue);
					if (selectedTransform.ParentID() != 0)
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
				tempValue = selectedTransform.Depth();
				if (ImGui::DragFloat("##zPos", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.Depth(tempValue);
				}

				// Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				tempValue = selectedTransform.Rot();
				if (ImGui::DragFloat("##rot", &tempValue, 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.GetRot() = tempValue;
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
				tempValue = scale.x;
				if (ImGui::DragFloat("##xScale", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.ScaleX(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				tempValue = scale.y;
				if (ImGui::DragFloat("##yScale", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedTransform.ScaleY(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("transformTag");
				ImGui::TableNextColumn();
				char inputBuffer[1024];
				std::strncpy(inputBuffer, selectedTransform.transformTag.c_str(), sizeof(inputBuffer) - 1);
				if (ImGui::InputText("##TransformTag", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					selectedTransform.transformTag = inputBuffer;
					/*if (inputBuffer[0] != '\0')
					{
						selectedTransform.transformTag = inputBuffer;
					}
					if (inputBuffer[0] == '\0')
					{
						CM_CORE_ERROR("Trying to make empty transformTag name!");
					}*/
				}

				ImGui::EndTable();


				//Collision Flags
				ImGui::Text("Collision Flag:");
				ImGui::SameLine();
				
				unsigned int &colMask = selectedTransform.collisionMask;
				CollisionLayer currentlyActive = CollisionLayer::TOTAL_LAYERS;

				for (int i = 0; i < 32; ++i)
				{
					uint32_t layerBit = 1 << i;
					char const* layerName = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerName(static_cast<CollisionLayer>(layerBit));
					bool isEnabled = colMask & layerBit;

					if (layerName == nullptr)
						continue;

					if (std::strcmp(layerName,"NULL") == 0)
						continue;
					if (isEnabled)
					{
						ImGui::Text(layerName);
						currentlyActive = static_cast<CollisionLayer>(layerBit);
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("v##colFlags"))
				{
					ImGui::OpenPopup("Collision Flag Select");
				}

				if(ImGui::BeginPopup("Collision Flag Select"))
				{
					for (int i = 0; i < 32; ++i)
					{
						uint32_t layerBit = 1 << i;
						char const* layerName = SystemManager::GetInstance()->GetSystem<TransformSystem>()->GetLayerName(static_cast<CollisionLayer>(layerBit));
						bool isEnabled = colMask & layerBit;
						if (layerName == nullptr)
							continue;

						if (std::strcmp(layerName, "NULL") == 0)
							continue;
						if (!isEnabled)
						{
							if (ImGui::Selectable(layerName))
							{
								go->AddCollisionLayer(static_cast<CollisionLayer>(layerBit));
								go->RemoveCollisionLayer(currentlyActive);
							}
						}
					}
					ImGui::EndPopup();
				}
			}
		}
	}

	template<typename T>
	void InspectorWindow::RenderUITransformTable(T* go, TABLETYPE type)
	{
		UNUSED(type);
		UITransform& selectedUITransform = go->GetComponent<UITransform>();
		float tempValue = 0;
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
				Vec2f pos = selectedUITransform.GetPos();
				tempValue = pos.x;
				if (ImGui::DragFloat("##xPos", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.PosX(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yPos");
				ImGui::TableNextColumn();
				tempValue = pos.y;
				if (ImGui::DragFloat("##yPos", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.PosY(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Depth");
				ImGui::TableNextColumn();
				tempValue = selectedUITransform.Depth();
				if (ImGui::DragFloat("##Depth", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.Depth(tempValue);
				}

				//Rotation
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				tempValue = selectedUITransform.Rot();
				if(ImGui::DragFloat("##Rotation", &tempValue, 1.0f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.Rot(tempValue);
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
				Vec2f scale = selectedUITransform.Scale();
				tempValue = scale.x;
				if (ImGui::DragFloat("##xScale", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.ScaleX(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("yScale");
				ImGui::TableNextColumn();
				tempValue = scale.y;
				if (ImGui::DragFloat("##yScale", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					selectedUITransform.ScaleY(tempValue);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("transformTag");
				ImGui::TableNextColumn();
				char inputBuffer[1024];
				std::strncpy(inputBuffer, selectedUITransform.transformTag.c_str(), sizeof(inputBuffer) - 1);
				if (ImGui::InputText("##TransformTag", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (inputBuffer[0] != '\0')
					{
						selectedUITransform.transformTag = inputBuffer;
					}
					if (inputBuffer[0] == '\0')
					{
						CM_CORE_ERROR("Trying to make empty transformTag name!");
					}
				}

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
		float tempValue = 0.f;
		UNUSED(tempValue);
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
				if (ImGui::Button("v##textureSelect"))
				{
					ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(400, 400));
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
					static char buffer[256];
					static std::string searchPrompt;
					ImGui::Text("Search:");
					ImGui::SameLine();
					if (ImGui::InputText("##TextureSearch", buffer, sizeof(buffer)))
					{
						searchPrompt = buffer;
					}

					for (const auto& entry : textureMap->mAssetMap)
					{
						if (entry.first.empty()) {}
						else
						{
							if(searchPrompt.empty())
							{
								if (ImGui::Button(entry.first.c_str()))
								{
									render.Texture(entry.first);
									ImGui::CloseCurrentPopup();
								}
							}
							else
							{
								if (entry.first.find(searchPrompt) != std::string::npos)
								{
									if (ImGui::Button(entry.first.c_str()))
									{
										render.Texture(entry.first);
										ImGui::CloseCurrentPopup();
									}
								}
							}
						}
					}
					ImGui::EndPopup();
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Colour");
				
				ImGui::TableNextColumn();
				float color[4] = { render.R(),render.G(),render.B(),render.A() };

				if (ImGui::ColorPicker4("##ColorPicker", color))
				{
					render.SetR(color[0]);
					render.SetG(color[1]);
					render.SetB(color[2]);
					render.SetA(color[3]);
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
				ImGui::Text("%s", anim.GetAnimAtlas().c_str());
				ImGui::SameLine();
				if (ImGui::Button("v##"))
				{
					ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(400, 400));
					ImGui::OpenPopup("Animation Select");
				}

				
				if (ImGui::BeginPopup("Animation Select"))
				{
					static char buffer[256] = "";
					static std::string searchPrompt;
					ImGui::Text("Search:");
					ImGui::SameLine();
					if (ImGui::InputText("##animSelect", buffer, sizeof(buffer)))
					{
						searchPrompt = buffer;
					}
					

					for (const auto& entry : animMap->mAssetMap)
					{
						if(entry.first.empty()){}
						else
						{
							if(searchPrompt.empty())
							{
								if (ImGui::Button(entry.first.c_str()))
								{
									anim.ChangeAnim(entry.first);
									ImGui::CloseCurrentPopup();
								}
							}
							else
							{
								if (entry.first.find(searchPrompt) != std::string::npos)
								{
									if (ImGui::Button(entry.first.c_str()))
									{
										anim.ChangeAnim(entry.first);
										ImGui::CloseCurrentPopup();
									}
								}
							}
						}

					}
					ImGui::EndPopup();
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Speed Modifier");
				ImGui::TableNextColumn();
				ImGui::DragFloat("##AnimSpeed", &anim.speed, 0.05f, 0.01f, FLT_MAX, "%.3f");
				ImGui::EndTable();
			}
		}
	}

	template<typename T>
	bool InspectorWindow::RenderRigidBodyTable(T* go, TABLETYPE type)
	{
		bool modified = false;
		bool wasModified = false;
		float tempValue = 0;
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
				tempValue = rb.velocity.x;
				wasModified = ImGui::DragFloat("##VelocityX", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
				modified |= wasModified;
				if (wasModified)
				{
					rb.velocity.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Velocity Y");
				ImGui::TableNextColumn();
				tempValue = rb.velocity.y;
				wasModified = ImGui::DragFloat("##VelocityY", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
				modified |= wasModified;
				if (wasModified)
				{
					rb.velocity.y = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Mass");
				ImGui::TableNextColumn();
				tempValue = rb.mass;
				wasModified = ImGui::DragFloat("##Mass", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
				modified |= wasModified;
				if (wasModified)
				{
					rb.mass = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Gravity");
				ImGui::TableNextColumn();
				tempValue = rb.gravity;
				wasModified = ImGui::DragFloat("##Gravity", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
				modified |= wasModified;
				if (wasModified)
				{
					rb.gravity = tempValue;
				}

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
		float tempValue = 0;
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
				tempValue = col.min.x;
				if (ImGui::DragFloat("##MinX", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					col.min.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Min Y");
				ImGui::TableNextColumn();
				tempValue = col.min.y;
				if (ImGui::DragFloat("##MinY", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					col.min.y = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Max X");
				ImGui::TableNextColumn();
				tempValue = col.max.x;
				if (ImGui::DragFloat("##MaxX", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					col.max.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Max Y");
				ImGui::TableNextColumn();
				tempValue = col.max.y;
				if (ImGui::DragFloat("##MaxY", &tempValue, 0.1f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					col.max.y = tempValue;
				}

				ImGui::TableNextColumn();
				ImGui::Text("Custom Width");
				ImGui::TableNextColumn();
				tempValue = col.GetCustomWidth();
				if (ImGui::DragFloat("##CustomWidth", &tempValue, 0.1f, 0.5f, FLT_MAX, "%.3f"))
				{
					if (tempValue < 0.0f)
					{
						tempValue = 1.0f;
						col.CustomWidth(tempValue);
					}
					else
					{
						col.CustomWidth(tempValue);
					}
				}
				

				ImGui::TableNextColumn();
				ImGui::Text("Custom Height");
				ImGui::TableNextColumn();
				tempValue = col.GetCustomHeight();
				if (ImGui::DragFloat("##CustomHeight", &tempValue, 0.1f, 0.5f, FLT_MAX, "%.3f"))
				{
					if (tempValue < 0.0f)
					{
						tempValue = 1.0f;
						col.CustomHeight(tempValue);
					}
					else
					{
						col.CustomHeight(tempValue);
					}
				}
				

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
						if (entry.first.empty())
						{
							continue;
						}
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
					if (ImGui::Button("v##scriptSelect"))
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
						else if (it.second.mType == ScriptFieldType::Int)
						{
							int data = scriptRef->GetFieldValue<int>(it.second.mName);
							if (ImGui::DragInt(it.second.mName.c_str(), &data))
							{
								scriptRef->SetFieldValue(it.second.mName, data);
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
					if (ImGui::Button("v##scriptSelect"))
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

						// if its a prefab type then have to manually populate 
						// the scriptablefield map
						if (type == PREFAB)
						{
							gScriptSystem->UpdateScriptPrefabComponent(script);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
	}

	template<typename T>
	void InspectorWindow::RenderStateMachineTable(T* go, TABLETYPE type)
	{
		StateMachine& stateMachine = go->GetComponent<StateMachine>();
		//Entity id = go->GetID();

		if (ImGui::CollapsingHeader("State Machine Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				if (InspectorWindow::RemoveComponentButton<StateMachine>(go))
					break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<StateMachine>(go))
					return;
				break;
			}
			default:
				break;
			}

			if (ImGui::BeginTable("StateMachine Settings", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextColumn();
				ImGui::Text("Attribute");
				ImGui::TableNextColumn();
				ImGui::Text("Setting");
				
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Current State:");
				std::string currState = stateMachine.currState;
				ImGui::TableNextColumn();
				ImGui::Text(currState.c_str());

				/*ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Next State:");
				std::string nextState = stateMachine.nextState;
				ImGui::TableNextColumn();
				ImGui::Text(nextState.c_str());*/

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Starting State:");
				ImGui::SameLine();
				if (ImGui::Button("v##smStartingState"))
				{
					ImGui::OpenPopup("Starting State Select");
				}

				if (ImGui::BeginPopup("Starting State Select"))
				{
					for (auto& state : stateMachine.stateMap)
					{
						if (ImGui::Selectable(state.first.c_str()))
						{
							stateMachine.startingState = state.first;
						}
					}

					ImGui::EndPopup();
				}
				std::string startState = stateMachine.startingState;
				ImGui::TableNextColumn();
				ImGui::Text(startState.c_str());


				ImGui::EndTable();
			}
		}
	}

	template<typename T>
	void InspectorWindow::RenderParticleTable(T* go, TABLETYPE type)
	{
		auto textureMap = AssetManager::GetInstance()->GetAssetMap<Texture>();
		ParticleEmitter& particle = go->GetComponent<ParticleEmitter>();
		float tempValue = 0.f;
		int tempInt = 0;
		if (ImGui::CollapsingHeader("Particle Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (type)
			{
			case GAMEOBJECT:
			{
				if (InspectorWindow::RemoveComponentButton<ParticleEmitter>(go))
					break;
			}
			case PREFAB:
			{
				if (InspectorWindow::RemoveComponentButton<ParticleEmitter>(go))
					return;
				break;
			}
			default:
				break;
			}

			if (ImGui::BeginTable("StateMachine Settings", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableNextColumn();
				ImGui::Text("Attribute");
				ImGui::TableNextColumn();
				ImGui::Text("Setting");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Texture");
				ImGui::SameLine();
				if (ImGui::Button("v##ParticleTextureSelect"))
				{
					ImGui::OpenPopup("ParticleTexture Select");
				}
				ImGui::TableNextColumn();
				ImGui::Text("%s", particle.texture.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_PAYLOAD"))
					{
						std::string textureName = *(const std::string*)payload->Data;
						particle.texture = textureName;
					}

					ImGui::EndDragDropTarget();
				}
				ImGui::SetNextWindowSize(ImVec2(300, 500));
				if (ImGui::BeginPopup("ParticleTexture Select"))
				{
					for (const auto& entry : textureMap->mAssetMap)
					{
						if (entry.first.empty()) continue; // TODO: Find out why "" is being added to asset map
						if (entry.first.find("SpriteSheet") != std::string::npos)
						{
							continue;
						}
						if (ImGui::Button(entry.first.c_str()))
						{
							particle.texture = entry.first;
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}


				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle xSpeed");
				ImGui::TableNextColumn();
				tempValue = particle.particleSpeed.x;
				if (ImGui::DragFloat("##xSpeed", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.particleSpeed.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle ySpeed");
				ImGui::TableNextColumn();
				tempValue = particle.particleSpeed.y;
				if (ImGui::DragFloat("##ySpeed", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.particleSpeed.y = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particles per second");
				ImGui::TableNextColumn();
				tempInt = static_cast<int>(particle.spawnPerSec);
				if (ImGui::DragInt("##PPS", &tempInt,1.f,0,100))
				{
					particle.spawnPerSec = static_cast<size_t>(tempInt);
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle Lifetime");
				ImGui::TableNextColumn();
				tempValue = particle.particleLifeTime;
				if (ImGui::DragFloat("##LifeTime", &tempValue, 0.05f, 0.f, FLT_MAX, "%.3f"))
				{
					particle.particleLifeTime = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Spawn BurstTime");
				ImGui::TableNextColumn();
				tempValue = particle.spawnBurstTime;
				if (ImGui::DragFloat("##SpawnBurstTime", &tempValue, 0.05f, 0.f, FLT_MAX, "%.3f"))
				{
					particle.spawnBurstTime = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle SpeedRange");
				ImGui::TableNextColumn();
				tempValue = particle.speedRange;
				if (ImGui::DragFloat("##SpeedRange", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.speedRange = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle SizeRangeMin");
				ImGui::TableNextColumn();
				tempValue = particle.scaleRange.x;
				if (ImGui::DragFloat("##SizeRangeMin", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.scaleRange.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle SizeRangeMax");
				ImGui::TableNextColumn();
				tempValue = particle.scaleRange.y;
				if (ImGui::DragFloat("##SizeRangeMax", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.scaleRange.y = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle AngleRange");
				ImGui::TableNextColumn();
				tempValue = particle.angleRange;
				if (ImGui::DragFloat("##AngleRange", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.angleRange = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle xSpawnRadius");
				ImGui::TableNextColumn();
				tempValue = particle.spawnRadius.x;
				if (ImGui::DragFloat("##xSpawnRadius", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.spawnRadius.x = tempValue;
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle ySpawnRadius");
				ImGui::TableNextColumn();
				tempValue = particle.spawnRadius.y;
				if (ImGui::DragFloat("##ySpawnRadius", &tempValue, 0.05f, -FLT_MAX, FLT_MAX, "%.3f"))
				{
					particle.spawnRadius.y = tempValue;
				}


				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Emitter Active");
				ImGui::TableNextColumn();
				bool checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::EMITTER_ACTIVE);
				if (ImGui::Checkbox("##EmitterActive", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::EMITTER_ACTIVE, checkBoxCheckedBoxedDiff);
				
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Emitter Burst type");
				ImGui::TableNextColumn();
				checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::EMITTER_BURST);
				if (ImGui::Checkbox("##EmitterBurst", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::EMITTER_BURST, checkBoxCheckedBoxedDiff);
				
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle Fade");
				ImGui::TableNextColumn();
				checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::PARTICLES_FADE);
				if (ImGui::Checkbox("##ParticleFade", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::PARTICLES_FADE, checkBoxCheckedBoxedDiff);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle Friction");
				ImGui::TableNextColumn();
				checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::PARTICLES_FRICTION);
				if(ImGui::Checkbox("##ParticleFriction", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::PARTICLES_FRICTION, checkBoxCheckedBoxedDiff);
				
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle Gravity");
				ImGui::TableNextColumn();
				checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::PARTICLES_GRAVITY);
				if(ImGui::Checkbox("##ParticleGravity", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::PARTICLES_GRAVITY, checkBoxCheckedBoxedDiff);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle Radial");
				ImGui::TableNextColumn();
				checkBoxCheckedBoxedDiff = particle.HasEmitterQualities(ParticleEmitter::PARTICLES_RADIAL);
				if(ImGui::Checkbox("##ParticleRadial", &checkBoxCheckedBoxedDiff))
					particle.SetEmitterQualities(ParticleEmitter::PARTICLES_RADIAL, checkBoxCheckedBoxedDiff);

				ImGui::EndTable();
			}
		}
	}


	/**
	 * @brief Inspector Table that displays the components that are currently added.
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

		if (go->HasComponent<StateMachine>())
		{
			RenderStateMachineTable(go, type);
		}

		if (go->HasComponent<ParticleEmitter>())
		{
			RenderParticleTable(go, type);
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

				ImGui::Text("Selected Entity: ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(selectedEntity).c_str());

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
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:	This file defines AssetWindow class which allows users to interact and manage assets.
		Prefabs can be selected from this window.
		Drag-drop functionality for textures to replace the textures of current GameObjects.
		Clicking on fonts to change fonts for GOs with TextRenderer.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <ImGUI/imgui.h>
#include "AssetWindow.h"



namespace Carmicah
{
	AssetWindow::AssetWindow() : EditorWindow("Asset Browser", ImVec2(900, 300), ImVec2(0, 0)) 
	{ 
		mIsVisible = true; 
		assetDir = AssetManager::GetInstance()->enConfig.assetLoc;
	}

	Prefab* AssetWindow::selectedPrefab = nullptr;
	bool AssetWindow::mSceneModified = false;

	template<typename T>
	void AssetWindow::DisplayAllAssets(std::filesystem::path path, std::shared_ptr<Carmicah::AssetType<T>> map)
	{
		auto& fileMap = AssetManager::GetInstance()->fileWatcher.fileMap;
		std::string ext;
		std::string name;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			name = entry.path().stem().string();
			ext = entry.path().extension().string();
			if (entry.is_directory())
			{
				if (ImGui::TreeNodeEx(entry.path().filename().string().c_str(),ImGuiTreeNodeFlags_OpenOnArrow))
				{
					DisplayAllAssets<T>(entry.path(),map);
					ImGui::TreePop();
				}
			}
			else
			{
				if(ext == ".png") //Its an image
				{
					
					bool isSpritesheet = false;
					std::string txtCheck = name + ".txt";

					//Check if its a spritesheet
					auto fileIt = std::find_if(fileMap.begin(), fileMap.end(), [&](const auto& entry)
						{
							return entry.first.find(txtCheck) != std::string::npos;
						});

					if (fileIt != fileMap.end()) //Its a spritesheet
					{
						isSpritesheet = true;
					}

					if (isSpritesheet)
					{
						std::string tableName = "##" + txtCheck;
						if (ImGui::BeginTable(tableName.c_str(), 5))
						{
							std::for_each(map->mAssetMap.begin(), map->mAssetMap.end(), [&](const auto& entry)
								{
									auto it = entry.first.find(name);
									if (it != std::string::npos)
									{
										Mtx3x3f matrix = map->mAssetList[entry.second].mtx;
										//GLuint textureID = assetManager->mArrayTex;
										Vec2f uv0(0, 0);
										Vec2f uv1(1, 1);
										uv0 = matrix * uv0;
										uv1 = matrix * uv1;
										float temp = -uv0.y;
										uv0.y = -uv1.y;
										uv1.y = temp;

										ImGui::TableNextColumn();
										if (ImGui::ImageButton(entry.first.c_str(),
											reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
											ImVec2(50, 50),
											ImVec2(uv0.x, uv0.y),
											ImVec2(uv1.x, uv1.y)))
										{
										}
										if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
										{
											if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
											{
												if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
												{
													Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
													for (const auto& textureEntry : map->mAssetMap)
													{
														if (entry.second == textureEntry.second)
														{
															render.Texture(textureEntry.first);
														}
													}
												}
											}
										}
										if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
										{
											ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &entry.first, sizeof(entry.first));
											ImVec2 dragSize(50, 50);
											ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
												dragSize,
												ImVec2(uv0.x, uv0.y),
												ImVec2(uv1.x, uv1.y));
											ImGui::Text("%s", entry.first.c_str());

											ImGui::EndDragDropSource();
										}
										ImGui::SameLine();
										ImGui::Text("%s", entry.first.c_str());
									}
								});
							ImGui::EndTable();
						}
					}

					else
					{
						auto it = map->mAssetMap.find(name);
						if (it != map->mAssetMap.end())
						{
							const auto& imageFound = *it;
							Mtx3x3f matrix = map->mAssetList[imageFound.second].mtx;
							//GLuint textureID = assetManager->mArrayTex;
							Vec2f uv0(0, 0);
							Vec2f uv1(1, 1);
							uv0 = matrix * uv0;
							uv1 = matrix * uv1;
							float temp = -uv0.y;
							uv0.y = -uv1.y;
							uv1.y = temp;

							ImGui::TableNextColumn();
							if (ImGui::ImageButton(name.c_str(),
								reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[imageFound.second].t])),
								ImVec2(50, 50),
								ImVec2(uv0.x, uv0.y),
								ImVec2(uv1.x, uv1.y)))
							{
							}
							if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
							{
								if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
								{
									if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
									{
										Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
										for (const auto& textureEntry : map->mAssetMap)
										{
											if (imageFound.second == textureEntry.second)
											{
												render.Texture(textureEntry.first);
											}
										}
									}
								}
							}
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
							{
								ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &imageFound.first, sizeof(imageFound.first));
								ImVec2 dragSize(50, 50);
								ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[imageFound.second].t])),
									dragSize,
									ImVec2(uv0.x, uv0.y),
									ImVec2(uv1.x, uv1.y));
								ImGui::Text("%s", imageFound.first.c_str());

								ImGui::EndDragDropSource();
							}
							ImGui::SameLine();
							ImGui::Text("%s", imageFound.first.c_str());
						}
					}
				}
				else if (ext == ".txt") //Ignore .txts for the asset browser
				{
					continue;
				}

				else
				{
					if (ImGui::TreeNodeEx(entry.path().stem().string().c_str()))
					{

						ImGui::TreePop();
					}
				}
			}
		}
	}

	void AssetWindow::TextureBrowserNew(std::string& name, std::shared_ptr<Carmicah::AssetType<Carmicah::Texture>> map)
	{
		std::filesystem::path pathLoc = assetDir.c_str() + std::string("/Images");

		if (!std::filesystem::exists(pathLoc))
		{
			CM_CORE_WARN("Path Location does not exist!");
			return;
		}
		if(ImGui::CollapsingHeader("TextureNew"))
		{
			DisplayAllAssets<Texture>(pathLoc, map);
		}
	}
	
	void AssetWindow::TextureBrowserOld(std::string& name, std::shared_ptr<Carmicah::AssetType<Carmicah::Texture>> map)
	{
		
		if (ImGui::CollapsingHeader("Texture"))
		{
			
			ImGui::Indent();

			if (ImGui::TreeNodeEx("Spritesheets"))
			{
				if (ImGui::BeginTable("##SpritesheetTable", 5))
				{
					for (const auto& entry : map->mAssetMap)
					{
						name = entry.first + "##texture";
						std::transform(name.begin(), name.end(), name.begin(), [](char& ch)
							{
								return std::tolower(ch);
							});
						if (name.find("spritesheet") == std::string::npos)
						{
							continue;
						}

						Mtx3x3f matrix = map->mAssetList[entry.second].mtx;
						//GLuint textureID = assetManager->mArrayTex;
						Vec2f uv0(0, 0);
						Vec2f uv1(1, 1);
						uv0 = matrix * uv0;
						uv1 = matrix * uv1;
						float temp = -uv0.y;
						uv0.y = -uv1.y;
						uv1.y = temp;

						ImGui::TableNextColumn();
						if (ImGui::ImageButton(name.c_str(),
							reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
							ImVec2(50, 50),
							ImVec2(uv0.x, uv0.y),
							ImVec2(uv1.x, uv1.y)))
						{
						}
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
							{
								if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
								{
									Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
									for (const auto& textureEntry : map->mAssetMap)
									{
										if (entry.second == textureEntry.second)
										{
											render.Texture(textureEntry.first);
										}
									}
								}
							}
						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &entry.first, sizeof(entry.first));
							ImVec2 dragSize(50, 50);
							ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
								dragSize,
								ImVec2(uv0.x, uv0.y),
								ImVec2(uv1.x, uv1.y));
							ImGui::Text("%s", entry.first.c_str());

							ImGui::EndDragDropSource();
						}
						ImGui::SameLine();
						ImGui::Text("%s", entry.first.c_str());

					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Image Textures"))
			{
				if (ImGui::BeginTable("##ImageTextureTable", 5))
				{
					for (const auto& entry : map->mAssetMap)
					{
						name = entry.first + "##texture";
						std::transform(name.begin(), name.end(), name.begin(), [](char& ch)
							{
								return std::tolower(ch);
							});
						if (name.find("spritesheet") != std::string::npos)
						{
							continue;
						}

						//Area for Rainne's Texture Magic
						Mtx3x3f matrix = map->mAssetList[entry.second].mtx;
						//GLuint textureID = assetManager->mArrayTex;
						Vec2f uv0(0, 0);
						Vec2f uv1(1, 1);
						uv0 = matrix * uv0;
						uv1 = matrix * uv1;
						float temp = -uv0.y;
						uv0.y = -uv1.y;
						uv1.y = temp;

						ImGui::TableNextColumn();
						if (ImGui::ImageButton(name.c_str(),
							reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
							ImVec2(50, 50),
							ImVec2(uv0.x, uv0.y),
							ImVec2(uv1.x, uv1.y)))
						{
						}
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
							{
								if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
								{
									Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
									for (const auto& textureEntry : map->mAssetMap)
									{
										if (entry.second == textureEntry.second)
										{
											render.Texture(textureEntry.first);
										}
									}
								}
							}
						}
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &entry.first, sizeof(entry.first));
							ImVec2 dragSize(50, 50);
							ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[map->mAssetList[entry.second].t])),
								dragSize,
								ImVec2(uv0.x, uv0.y),
								ImVec2(uv1.x, uv1.y));
							ImGui::Text("%s", entry.first.c_str());

							ImGui::EndDragDropSource();
						}
						ImGui::SameLine();
						ImGui::Text("%s", entry.first.c_str());

					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}

			ImGui::Unindent();
		}
	}


	/**
	 * @brief Update function for the AssetWindow
	 * 
	 */
	void AssetWindow::Update()
	{
		//Obtaining all the AssetMaps
		static auto assetManager = AssetManager::GetInstance();
		static auto systemManager = SystemManager::GetInstance();
		//auto primitiveMap = assetManager->GetAssetMap<Primitive>();
		//auto shaderMap = assetManager->GetAssetMap<Shader>();
		auto textureMap = assetManager->GetAssetMap<Texture>();
		auto fontMap = assetManager->GetAssetMap<Font>();
		auto audioMap = assetManager->GetAssetMap<FMOD::Sound*>();
		auto prefabMap = assetManager->GetAssetMap<Prefab>();
		auto sceneMap = assetManager->GetAssetMap<Scene>();
		char inputBuffer[1024] = "";

#pragma region Debugging
		for (const auto& entry : AssetManager::GetInstance()->fileWatcher.fileMap)
		{
			/*if (entry.first.find("bg.png") != std::string::npos)
			{
				CM_CORE_INFO("FOUND PNG!");
			}
			if (entry.first.find("bg.txt") != std::string::npos)
			{
				CM_CORE_INFO("FOUND TXT!");
			}*/
		}

#pragma endregion



		if (ImGui::Begin(mTitle))
		{
			std::string name;

			if (ImGui::Button("Reload Assets"))
			{
				AssetManager::GetInstance()->fileWatcher.Update();
			}


			TextureBrowserNew(name, textureMap);


			TextureBrowserOld(name, textureMap);


			//if (ImGui::CollapsingHeader("Textures"))
			//{
			//	ImGui::Indent();
			//	//ImGui::BeginChild("TextureTableRegion", ImVec2(400, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
			//	if (ImGui::BeginTable("TextureTable", 5))
			//	{

			//		
			//		for (const auto& entry : textureMap->mAssetMap)
			//		{
			//			name = entry.first + "##texture";
			//			if (name.find("SpriteSheet") != std::string::npos)
			//			{
			//				continue;
			//			}

			//			Mtx3x3f matrix = textureMap->mAssetList[entry.second].mtx;
			//			//GLuint textureID = assetManager->mArrayTex;
			//			Vec2f uv0(0, 0);
			//			Vec2f uv1(1, 1);
			//			uv0 = matrix * uv0;
			//			uv1 = matrix * uv1;
			//			float temp = -uv0.y;
			//			uv0.y = -uv1.y;
			//			uv1.y = temp;

			//			ImGui::TableNextColumn();
			//			if (ImGui::ImageButton(name.c_str(),
			//				reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[textureMap->mAssetList[entry.second].t])),
			//				ImVec2(50, 50),
			//				ImVec2(uv0.x, uv0.y),
			//				ImVec2(uv1.x, uv1.y)))
			//			{}
			//			if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
			//			{
			//				if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
			//				{
			//					if (HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<Renderer>())
			//					{
			//						Renderer& render = HierarchyWindow::selectedGO->GetComponent<Renderer>();
			//						for (const auto& textureEntry : textureMap->mAssetMap)
			//						{
			//							if (entry.second == textureEntry.second)
			//							{
			//								render.Texture(textureEntry.first);
			//							}
			//						}
			//					}
			//				}
			//			}
			//			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			//			{
			//				ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &entry.first, sizeof(entry.first));
			//				ImVec2 dragSize(50, 50);
			//				ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(AssetManager::GetInstance()->mPreviewTexs[textureMap->mAssetList[entry.second].t])),
			//					dragSize,
			//					ImVec2(uv0.x, uv0.y),
			//					ImVec2(uv1.x, uv1.y));
			//				ImGui::Text("%s", entry.first.c_str());

			//				ImGui::EndDragDropSource();
			//			}
			//			ImGui::SameLine();
			//			ImGui::Text("%s", entry.first.c_str());

			//		}
			//		ImGui::EndTable();
			//	}
			//	//ImGui::EndChild();
			//	ImGui::Unindent();
			//}

			if (ImGui::CollapsingHeader("Font"))
			{
				ImGui::Indent();

				for (const auto& entry : fontMap->mAssetMap)
				{
					name = entry.first + "##font";
					if (ImGui::Button(name.c_str()))
					{
						if(HierarchyWindow::selectedGO != nullptr && HierarchyWindow::selectedGO->HasComponent<TextRenderer>())
						{
							TextRenderer& text = HierarchyWindow::selectedGO->GetComponent<TextRenderer>();
							text.font = entry.first;
						}
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Font!");
						ImGui::EndTooltip();
					}
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Audio"))
			{
				ImGui::Indent();
				for (const auto& entry : audioMap->mAssetMap)
				{
					if (ImGui::Button(entry.first.c_str()))
					{
						
					}
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Prefab"))
			{
				ImGui::Indent();
				for (const auto& entry : prefabMap->mAssetMap)
				{
					name = entry.first + "##Prefab";
					if (ImGui::Button(name.c_str())){}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						selectedPrefab = &prefabMap->mAssetList[entry.second];
						if (ImGui::GetIO().MouseClickedCount[ImGuiMouseButton_Left] == 2)
						{
							gGOFactory->CreatePrefab(entry.first);
						}
					}

					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup(name.c_str());
					}

					if (ImGui::BeginPopup(name.c_str()))
					{
						if (ImGui::Button("Edit Prefab"))
						{
							selectedPrefab = &prefabMap->mAssetList[entry.second];
							HierarchyWindow::inspectedPrefab = &prefabMap->mAssetList[entry.second];
							HierarchyWindow::mShowScene = false;
							HierarchyWindow::selectedGO = nullptr;
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
					{
						ImGui::SetDragDropPayload("PREFAB_PAYLOAD", &entry.first, sizeof(entry.first));
						ImGui::Text("Dragging Prefab %s", name.c_str());
						ImGui::EndDragDropSource();
					}
				}
				ImGui::Unindent();
			}

			if (ImGui::CollapsingHeader("Scene"))
			{
				for (const auto entry : sceneMap->mAssetMap)
				{
					if (entry.first.empty())
					{
						continue;
					}

					name = entry.first + "##Scene";

					if (ImGui::Button(name.c_str()))
					{
						systemManager->ChangeScene(entry.first);
					}

					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup(name.c_str());

					}

					if (ImGui::BeginPopup(name.c_str()))
					{
						//std::strncpy(inputBuffer, entry.first.c_str(), sizeof(inputBuffer) - 1);

						//ImGui::Text("Rename Scene: ");
						//ImGui::SameLine();

						//if (ImGui::InputText("##Scene Name:", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
						//{
						//	/*if (inputBuffer[0] != '\0')
						//	{
						//		gGOFactory->sceneGO.sceneName = inputBuffer;
						//	}*/

						//	if (inputBuffer != entry.first)
						//	{
						//		assetManager->RenameScene(entry.first, inputBuffer, AssetManager::GetInstance()->enConfig.assetLoc.c_str());
						//		mSceneModified = true;
						//		AssetManager::GetInstance()->fileWatcher.Update();
						//		AssetManager::GetInstance()->fileWatcher.Update();
						//		/*std::string sceneFile;
						//		AssetManager::GetInstance()->GetScene(inputBuffer, sceneFile);
						//		SerializerSystem::GetInstance()->SerializeScene(sceneFile);*/
						//		
						//	}

						//	if (inputBuffer[0] == '\0')
						//	{
						//		CM_CORE_ERROR("Empty Scene Name!");
						//	}

						//	ImGui::CloseCurrentPopup();
						//}

						if (ImGui::Selectable("Duplicate Scene"))
						{
							AssetManager::GetInstance()->CloneScene(entry.first);
						}

						if(entry.first != "DefaultScene")
						{
							if (ImGui::Selectable("Delete Scene"))
							{
								AssetManager::GetInstance()->DeleteScene(entry.first);
								mSceneModified = true;
							}
						}
						ImGui::EndPopup();
					}

					if (mSceneModified)
					{
						mSceneModified = false;
						break;
					}
				}
			}
		}
		ImGui::End();
	}
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Editor.cpp

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This editor class manages and coordinates the various editor windows.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include "Editor.h"


namespace Carmicah
{

	std::vector<std::string> Editor::sDroppedFilePaths{};
	bool Editor::mShowCloseConfirmation = false;
	std::vector<Entity> Editor::mSceneHierarchy;
	std::vector<Entity> Editor::mSceneUIHierarchy;
	std::unordered_map<Entity, std::vector<Entity>> Editor::mChildrenHierarchy;

	Editor::Editor()
	{
		
	}

	Editor::~Editor()
	{

	}
	void Editor::Init(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();  (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Window
		io.IniFilename = nullptr; //Disable this to enable saving of your windows settings

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init("#version 460");
		//Allows for dropped objects
		glfwSetWindowUserPointer(window, this);
		glfwSetDropCallback(window, DropCallback);
		glfwSetWindowCloseCallback(window, CloseCallback);

		//Creating Windows
		//For Testing
		//mWindows.push_back(std::make_unique<EditorWindow>("B", ImVec2(100,100), ImVec2(200, 100)));
		mWindows.push_back(std::make_unique<SceneWindow>());
		mWindows.push_back(std::make_unique<CameraWindow>());
		mWindows.push_back(std::make_unique<HierarchyWindow>());
		mWindows.push_back(std::make_unique<DebugWindow>());
		mWindows.push_back(std::make_unique<AssetWindow>());
		mWindows.push_back(std::make_unique<InspectorWindow>());

		//Initialise the copy
		InitFullHierarchy();

	}

	void Editor::Update(GLFWwindow* window)
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		static bool sFirstTime = true;
		static ImVec2 popupSize(300, 150);
		//float mainMenuHeight{};
		ImGuiWindowFlags dockingWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		

		// Begin full-screen window
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		if(ImGui::Begin("DockingWindow", nullptr, dockingWindowFlags))
		{

#pragma region DockingSettings
			// Create the main docking space
			ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
			if (sFirstTime)
			{
				sFirstTime = false;

				/*ImGui::DockBuilderRemoveNode(dockspaceID);
				ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);*/
				ImGuiID dockMain = dockspaceID; // Main area
				ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.4f, nullptr, &dockMain);
				ImGuiID dockBotLeft = ImGui::DockBuilderSplitNode(dockBottom, ImGuiDir_Left, 0.5f, nullptr, &dockBottom);
				ImGuiID dockBotRight = dockBottom;
				ImGuiID dockLeft = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.2f, nullptr, &dockMain);
				ImGuiID dockRight = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.25f, nullptr,&dockMain);
				// Dock your windows into the split areas
				ImGui::DockBuilderDockWindow("Asset Browser", dockBotLeft);
				ImGui::DockBuilderDockWindow("Debug", dockBotRight);
				ImGui::DockBuilderDockWindow("Scene", dockMain);
				ImGui::DockBuilderDockWindow("Editor Camera", dockMain);
				ImGui::DockBuilderDockWindow("Inspector", dockRight);
				ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
				ImGui::DockBuilderFinish(dockMain);
			}
#pragma endregion

#pragma region Main Menu Bar
			//Main Menu Bar
			if (ImGui::BeginMenuBar())
			{
				//mainMenuHeight = ImGui::GetFrameHeight();
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::BeginMenu("New"))
					{
						if (ImGui::MenuItem("Create New Scene"))
						{

						}
						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Window"))
				{
					for (const auto& window : mWindows)
					{

						//Toggle Asset Browser Visibility
						if (auto assetWindow = dynamic_cast<AssetWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Asset Browser", nullptr, assetWindow->mIsVisible))
							{
								assetWindow->mIsVisible = !assetWindow->mIsVisible;
							}
						}

						//Toggle Debug Window Visibility
						if (auto debugWindow = dynamic_cast<DebugWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Debug", nullptr, debugWindow->mIsVisible))
							{
								debugWindow->mIsVisible = !debugWindow->mIsVisible;
							}
						}

						//Toggle Heirarchy Window Visibility
						if (auto heirarchyWindow = dynamic_cast<HierarchyWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Hierarchy", nullptr, heirarchyWindow->mIsVisible))
							{
								heirarchyWindow->mIsVisible = !heirarchyWindow->mIsVisible;
							}
						}
						//Toggle Inspector Window Visibility
						if (auto inspectorWindow = dynamic_cast<InspectorWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Inspector", nullptr, inspectorWindow->mIsVisible))
							{
								inspectorWindow->mIsVisible = !inspectorWindow->mIsVisible;
							}
						}
						//Toggle Scene Window Visibility
						if (auto sceneWindow = dynamic_cast<SceneWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Scene", nullptr, sceneWindow->mIsVisible))
							{
								sceneWindow->mIsVisible = !sceneWindow->mIsVisible;
							}
						}

						
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

#pragma endregion

			ImGui::End();
		}

		//Window Update Stuff
		for (auto& window : mWindows) 
		{

			ImGui::SetNextWindowSize(window->mSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(window->mPos, ImGuiCond_FirstUseEver);

			if(window->mIsVisible)
			{
				window->Update();

				// Check for messages
				if (window->mMessages.size() > 0)
				{
					// Go through any messages the window has
					for (auto msg : window->mMessages)
					{
						// send it 
						SendSysMessage(msg.get());
					}

					// Clear the messages after
					window->mMessages.clear();
				}
			}
		}

#pragma region Logic

		int counter = 1;
		if (counter++ % 10)
		{
			CM_CORE_INFO("Scene Size:");
			CM_CORE_INFO(std::to_string(Editor::mSceneHierarchy.size()));
			CM_CORE_INFO("UI Size: ");
			CM_CORE_INFO(std::to_string(Editor::mSceneUIHierarchy.size()));
		}


		if (mShowCloseConfirmation)
		{
			ImGui::OpenPopup("Close Confirmation");
		}


		if (HierarchyWindow::selectedGO != nullptr)
		{
			if (Input.IsKeyPressed(KEY_DELETE))
				gGOFactory->Destroy(HierarchyWindow::selectedGO->GetID());
		}

		if(Editor::sDroppedFilePaths.size() > 0)
		{
			for (const auto& file : Editor::sDroppedFilePaths)
			{

				if (AssetManager::GetInstance()->CopyAssetToAssetsFolder(file, AssetManager::GetInstance()->enConfig.assetLoc.c_str()) == AssetManager::ASSETCOPIED::FAILURE)
				{
					
					ImGui::OpenPopup("Unsupported");
					
				}

				if (AssetManager::GetInstance()->CopyAssetToAssetsFolder(file, AssetManager::GetInstance()->enConfig.assetLoc.c_str()) == AssetManager::ASSETCOPIED::MP3_ERROR)
				{
					ImGui::OpenPopup("MP3 Error");
				}
			}
			Editor::sDroppedFilePaths.clear();
			AssetManager::GetInstance()->fileWatcher.Update();
		}

		//NOT WORKING RN
		/*if (!AssetWindow::soundToPlay.empty())
		{
			PlaySFXMsg msg(AssetWindow::soundToPlay);
			SendSysMessage(&msg);
			AssetWindow::soundToPlay.clear();
		}*/

		if (SceneWindow::mChangeState)
		{
			RuntimeStartMessage msg;
			SendSysMessage(&msg);
			SceneWindow::mChangeState = false;
		}

#pragma endregion



#pragma region Pop-Up Handling
		if(ImGui::BeginPopup("Unsupported"))
		{
			ImVec2 currentPopupSize = ImGui::GetWindowSize();
			std::cout << currentPopupSize.x << "," << currentPopupSize.y << std::endl;
			ImGui::Dummy(ImVec2(popupSize.x -16.f, popupSize.y - 16.f));
			ImGui::SetCursorPos(ImVec2(popupSize.x - 30.f, 5.f));
			if(ImGui::Button("X"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetCursorPos(ImVec2((popupSize.x - ImGui::CalcTextSize("Unsupported File Format!").x) / 2.0f, (popupSize.y - ImGui::CalcTextSize("Unsupported File Format!").y) / 2.0f));
			ImGui::Text("Unsupported File Format!");
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("MP3 Error"))
		{
			ImVec2 currentPopupSize = ImGui::GetWindowSize();
			std::cout << currentPopupSize.x << "," << currentPopupSize.y << std::endl;
			ImGui::Dummy(ImVec2(popupSize.x - 16.f, popupSize.y - 16.f));
			ImGui::SetCursorPos(ImVec2(popupSize.x - 30.f, 5.f));
			if (ImGui::Button("X"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetCursorPos(ImVec2((popupSize.x - ImGui::CalcTextSize("MP3 not allow!").x) / 2.0f, (popupSize.y - ImGui::CalcTextSize("MP3 not allow!").y) / 2.0f));
			ImGui::Text("MP3 not allow!");
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Close Confirmation",NULL,ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to close the application?");
			ImGui::Separator();

			//Calculations for the buttons
			const float buttonWidth = 120.0f;
			const float buttonSpacing = ImGui::GetStyle().ItemSpacing.x;
			const float totalWidth = buttonWidth * 2 + buttonSpacing;

			// Center the buttons by adding a dummy region
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

			if (ImGui::Button("Yes", ImVec2(buttonWidth,0)))
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No",ImVec2(buttonWidth,0)))
			{
				mShowCloseConfirmation = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
#pragma endregion

	void Editor::Render(GLFWwindow* window)
	{
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(window);
		//glfwSwapBuffers(window);
	}

	void Editor::Exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::EntityDestroyed(Entity id)
	{
		for (auto& window : mWindows)
		{
			window->EntityDestroyed(id);
		}

		//Check if parent or not:
		bool isChild = false;

		GameObject& currentGO = gGOFactory->GetMIDToGO().at(id);

		Entity parentID = 0;

		if (currentGO.HasComponent<Transform>())
		{
			// Get the parent ID
			parentID = currentGO.GetComponent<Transform>().parent;
			if (parentID == gGOFactory->sceneGO.sceneID)
			{
				mSceneHierarchy.erase(std::remove_if(mSceneHierarchy.begin(), mSceneHierarchy.end(), [id](const auto& element)
					{return element == id; }), mSceneHierarchy.end());
			}
			//Its children
			else
			{
				auto childIt = std::find(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(), id);
				mChildrenHierarchy[parentID].erase(std::remove_if(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(), [id](const auto& element)
					{return element == id; }), mChildrenHierarchy[parentID].end());
			}
		}
		else if (currentGO.HasComponent<UITransform>())
		{
			parentID = currentGO.GetComponent<UITransform>().parent;

			if (parentID == gGOFactory->sceneGO.sceneID)
			{
				mSceneUIHierarchy.erase(std::remove_if(mSceneUIHierarchy.begin(), mSceneUIHierarchy.end(), [id](const auto& element)
					{return element == id; }), mSceneUIHierarchy.end());
			}
			//Its children
			else
			{
				auto childIt = std::find(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(), id);
				mChildrenHierarchy[parentID].erase(std::remove_if(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(), [id](const auto& element)
					{return element == id; }), mChildrenHierarchy[parentID].end());
			}
		}

		
		/*if(parentID == gGOFactory->sceneGO.sceneID)
		{
			mSceneHierarchy.erase(std::remove_if(mSceneHierarchy.begin(), mSceneHierarchy.end(), [id](const auto& element)
				{return element == id; }), mSceneHierarchy.end());
		}
		//Its children
		else 
		{
			auto childIt = std::find(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(), id);
			mChildrenHierarchy[parentID].erase(std::remove_if(mChildrenHierarchy[parentID].begin(), mChildrenHierarchy[parentID].end(),[id](const auto& element)
				{return element == id; }), mChildrenHierarchy[parentID].end());
		}*/

		
	}

	void Editor::EntityAdded(Entity id)
	{
	
	}

	void Editor::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_EDITORENTITY)
		{
			if (dynamic_cast<EditorEntityPicked*>(msg)->mEntityID != 0)
			{
				HierarchyWindow::selectedGO = nullptr;
				HierarchyWindow::selectedGO = &gGOFactory->FetchGO(dynamic_cast<EditorEntityPicked*>(msg)->mEntityID);
				HierarchyWindow::inspectedPrefab = nullptr;
				HierarchyWindow::mShowScene = true;
				AssetWindow::selectedPrefab = nullptr;
			}
		}
		if (msg->mMsgType == MSG_UPDATEHIERARCHY)
		{
			UpdateHierarchyMessage* castedMsg = dynamic_cast<UpdateHierarchyMessage*>(msg);

			//if Re-parenting to Scene
			if (castedMsg->mParentID == gGOFactory->sceneGO.sceneID)
			{
				//Find the Old Parent
				Entity oldParentID = 0;
				GameObject& currentGO = gGOFactory->GetMIDToGO().at(castedMsg->mEntityID);

				if (currentGO.HasComponent<Transform>())
				{
					// Get the parent ID
					oldParentID = currentGO.GetComponent<Transform>().parent;

					if (oldParentID != 0)
					{
						auto childIt = std::find(mChildrenHierarchy[oldParentID].begin(), mChildrenHierarchy[oldParentID].end(), castedMsg->mEntityID);

						if (childIt != mChildrenHierarchy[oldParentID].end())
						{
							mChildrenHierarchy[oldParentID].erase(childIt);
						}
						mSceneHierarchy.push_back(castedMsg->mEntityID);
					}
					else
					{
						mSceneHierarchy.push_back(castedMsg->mEntityID);
					}
				}
				else if (currentGO.HasComponent<UITransform>())
				{
					oldParentID = currentGO.GetComponent<UITransform>().parent;

					if (oldParentID != 0)
					{
						auto childIt = std::find(mChildrenHierarchy[oldParentID].begin(), mChildrenHierarchy[oldParentID].end(), castedMsg->mEntityID);

						if (childIt != mChildrenHierarchy[oldParentID].end())
						{
							mChildrenHierarchy[oldParentID].erase(childIt);
						}
						mSceneUIHierarchy.push_back(castedMsg->mEntityID);
					}
					else
					{
						mSceneUIHierarchy.push_back(castedMsg->mEntityID);
					}
				}
				

				//Remove from the respective mChildHierarchy (should only do this if its not already has parent id = 0)
				//if(oldParentID != 0)
				//{
				//	auto childIt = std::find(mChildrenHierarchy[oldParentID].begin(), mChildrenHierarchy[oldParentID].end(), castedMsg->mEntityID);

				//	if (childIt != mChildrenHierarchy[oldParentID].end())
				//	{
				//		mChildrenHierarchy[oldParentID].erase(childIt);
				//	}
				//	mSceneHierarchy.push_back(castedMsg->mEntityID);
				//}
				//else
				//{
				//	mSceneHierarchy.push_back(castedMsg->mEntityID);
				//}
			}
			//Children shenanigans
			else
			{
				GameObject& currentGO = gGOFactory->GetMIDToGO().at(castedMsg->mEntityID);
				//Checking if the oldParent is from mSceneHierarchy
				if(currentGO.HasComponent<Transform>())
				{
					auto it = std::find(mSceneHierarchy.begin(), mSceneHierarchy.end(), castedMsg->mEntityID);

					//Remove from mSceneHierarchy
					if (it != mSceneHierarchy.end())
					{
						mSceneHierarchy.erase(it);
					}

					//Its not a sceneGO object to begin with so time to find out where its parent is
					else
					{
						Entity oldParentID = 0;

						// Get the parent ID
						if (currentGO.HasComponent<Transform>())
						{
							oldParentID = currentGO.GetComponent<Transform>().parent;
						}
						else if (currentGO.HasComponent<UITransform>())
						{
							oldParentID = currentGO.GetComponent<UITransform>().parent;
						}


						//Remove from the respective mChildHierarchy
						auto childIt = std::find(mChildrenHierarchy[oldParentID].begin(), mChildrenHierarchy[oldParentID].end(), castedMsg->mEntityID);

						if (childIt != mChildrenHierarchy[oldParentID].end())
						{
							mChildrenHierarchy[oldParentID].erase(childIt);
						}
					}
				}

				else if(currentGO.HasComponent<UITransform>())
				{
					auto it = std::find(mSceneUIHierarchy.begin(), mSceneUIHierarchy.end(), castedMsg->mEntityID);

					//Remove from mSceneHierarchy
					if (it != mSceneUIHierarchy.end())
					{
						mSceneUIHierarchy.erase(it);
					}

					//Its not a sceneGO object to begin with so time to find out where its parent is
					else
					{
						Entity oldParentID = 0;

						// Get the parent ID
						if (currentGO.HasComponent<Transform>())
						{
							oldParentID = currentGO.GetComponent<Transform>().parent;
						}
						else if (currentGO.HasComponent<UITransform>())
						{
							oldParentID = currentGO.GetComponent<UITransform>().parent;
						}


						//Remove from the respective mChildHierarchy
						auto childIt = std::find(mChildrenHierarchy[oldParentID].begin(), mChildrenHierarchy[oldParentID].end(), castedMsg->mEntityID);

						if (childIt != mChildrenHierarchy[oldParentID].end())
						{
							mChildrenHierarchy[oldParentID].erase(childIt);
						}
					}
				}
				
				//Adding it to the mChildrenHierarchy as an entry under parentID
				mChildrenHierarchy[castedMsg->mParentID].push_back(castedMsg->mEntityID);
			}
		}
	}

	void Editor::DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		UNUSED(window);
		for (int i = 0; i < count; ++i)
		{
			std::string filePath = paths[i];
			size_t dotPos = filePath.find_last_of('.');
			UNUSED(dotPos);
			sDroppedFilePaths.push_back(filePath);
		}
	}

	void Editor::CloseCallback(GLFWwindow* window)
	{
		glfwSetWindowShouldClose(window, GLFW_FALSE);
		mShowCloseConfirmation = true;
	}

	void Editor::InitFullHierarchy()
	{

		//Section for Children Game Objects

		std::function<void(GameObject&)> childrenCheck = [&](GameObject& go)
			{
				if (go.HasComponent<Transform>())
				{
					const auto& GOChildren = go.GetComponent<Transform>().children;
					if (!GOChildren.empty())
					{
						mChildrenHierarchy[go.GetID()] = GOChildren;

						std::for_each(GOChildren.begin(), GOChildren.end(), [&](Entity childID)
							{
								childrenCheck(gGOFactory->GetMIDToGO().at(childID));
							});
					}
				}
				else if (go.HasComponent<UITransform>())
				{
					const auto& GOChildren = go.GetComponent<UITransform>().children;
					if (!GOChildren.empty())
					{
						mChildrenHierarchy[go.GetID()] = GOChildren;

						std::for_each(GOChildren.begin(), GOChildren.end(), [&](Entity childID)
							{
								childrenCheck(gGOFactory->GetMIDToGO().at(childID));
							});
					}
				}
			};

		//Loop through all scene-level objects
		std::for_each(gGOFactory->GetMIDToGO().begin(), gGOFactory->GetMIDToGO().end(), [&](auto& pair)
			{
				auto& gameObject = pair.second;
				childrenCheck(gameObject);
			});
	}
}
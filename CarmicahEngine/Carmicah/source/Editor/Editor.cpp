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
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_internal.h>
#include "../Input/InputSystem.h"
#include "../Systems/GOFactory.h"

namespace Carmicah
{

	std::vector<std::string> Editor::droppedFilePaths{};
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

		//Creating Windows
		//For Testing
		//mWindows.push_back(std::make_unique<EditorWindow>("B", ImVec2(100,100), ImVec2(200, 100)));
		mWindows.push_back(std::make_unique<SceneWindow>());
		mWindows.push_back(std::make_unique<CameraWindow>());
		mWindows.push_back(std::make_unique<HierarchyWindow>());
		mWindows.push_back(std::make_unique<DebugWindow>());
		mWindows.push_back(std::make_unique<AssetWindow>());
		mWindows.push_back(std::make_unique<InspectorWindow>());

	}

	void Editor::Update()
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		static bool sFirstTime = true;
		static ImVec2 popupSize(300, 150);
		float mainMenuHeight{};
		ImGuiWindowFlags dockingWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		

		// Begin full-screen window
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		if(ImGui::Begin("DockingWindow", nullptr, dockingWindowFlags))
		{

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

			//Main Menu Bar
			if (ImGui::BeginMenuBar())
			{
				mainMenuHeight = ImGui::GetFrameHeight();
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New"))
					{

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Window"))
				{
					for (const auto& window : mWindows)
					{
						//Toggle Heirarchy Window Visibility
						if (auto heirarchyWindow = dynamic_cast<HierarchyWindow*>(window.get()))
						{
							if (ImGui::MenuItem("Hierarchy", nullptr, heirarchyWindow->mIsVisible))
							{
								heirarchyWindow->mIsVisible = !heirarchyWindow->mIsVisible;
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
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
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
		if(Editor::droppedFilePaths.size() > 0)
		{
			for (const auto& file : Editor::droppedFilePaths)
			{

				if (!AssetManager::GetInstance()->CopyAssetToAssetsFolder(file, AssetManager::GetInstance()->enConfig.assetLoc.c_str()))
				{
					
					ImGui::OpenPopup("Unsupported");
					
				}
			}
			Editor::droppedFilePaths.clear();
			AssetManager::GetInstance()->fileWatcher.Update();
		}



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
			ImGui::SetCursorPos(ImVec2((popupSize.x - ImGui::CalcTextSize("Unsupported File Format!").x) / 2.0f, (popupSize.y - ImGui::CalcTextSize("Unsupported File Format").y) / 2.0f));
			ImGui::Text("Unsupported File Format!");
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
	}

	void Editor::ReceiveMessage(Message* msg)
	{
		if (msg->mMsgType == MSG_EDITORENTITY)
		{
			if (dynamic_cast<EditorEntityPicked*>(msg)->mEntityID != 0)
			{
				HierarchyWindow::selectedGO = &gGOFactory->FetchGO(dynamic_cast<EditorEntityPicked*>(msg)->mEntityID);
				HierarchyWindow::inspectedPrefab = nullptr;
				HierarchyWindow::mShowScene = true;
				AssetWindow::selectedPrefab = nullptr;
			}
		}
	}

	void Editor::DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		for (int i = 0; i < count; ++i)
		{
			std::string filePath = paths[i];
			size_t dotPos = filePath.find_last_of('.');

			droppedFilePaths.push_back(filePath);
		}
	}
}
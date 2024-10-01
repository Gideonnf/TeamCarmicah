#include "pch.h"
#include "Editor/Editor.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_internal.h>


namespace Carmicah
{


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
		io.IniFilename = nullptr;

		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init("#version 460");

		//Creating Windows
		//mWindows.push_back(std::make_unique<EditorWindow>("##",ImVec2(glfwGet)))
		mWindows.push_back(std::make_unique<EditorWindow>("A", ImVec2(200,100), ImVec2(100, 100)));
		mWindows.push_back(std::make_unique<EditorWindow>("B", ImVec2(100,100), ImVec2(100, 100)));
		mWindows.push_back(std::make_unique<HierarchyWindow>());
		mWindows.push_back(std::make_unique<DebugWindow>());

	}

	void Editor::Update(GLFWwindow* window)
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		static bool sFirstTime = true;
		float mainMenuHeight{};
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;



		// Begin full-screen window
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 padding = style.WindowPadding;
		if(ImGui::Begin("DockingWindow", nullptr, windowFlags))
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
				ImGuiID dockLeft = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.25f, nullptr, &dockMain);
				ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.25f, nullptr, &dockMain);
				// Dock your windows into the split areas
				ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
				ImGui::DockBuilderDockWindow("Debug", dockBottom);
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

		for (auto& window : mWindows) 
		{

			ImGui::SetNextWindowSize(window->mSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(window->mPos, ImGuiCond_FirstUseEver);

			if(window->mIsVisible)
			{
				window->Update();
			}
		}
	}

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
		glfwSwapBuffers(window);
	}

	void Editor::Exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
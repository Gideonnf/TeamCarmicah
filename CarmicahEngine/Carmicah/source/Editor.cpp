#include "pch.h"
#include "Editor/Editor.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>


namespace Carmicah
{


	Editor::Editor()
	{

	}

	Editor::~Editor()
	{

	}
	void Editor::Init(GLFWwindow* window, const char* glsl_version)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();  (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init("#version 460");

		//Creating Windows
		windows.push_back(std::make_unique <EditorWindow>("Testing",ImVec2(500,500), ImVec2(0,1080)));
		windows.push_back(std::make_unique<DebugWindow>());

	}

	void Editor::Update()
	{
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Main Menu Bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				for(const auto& window : windows)
				{
					if (auto debugWindow = dynamic_cast<DebugWindow*>(window.get()))
					{
						if (ImGui::MenuItem("Debug", nullptr, debugWindow->isVisible))
						{
							debugWindow->isVisible = !debugWindow->isVisible;
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		
		for (auto& window : windows) 
		{

			ImGui::SetNextWindowSize(window->size, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(window->pos, ImGuiCond_FirstUseEver);

			if(window->isVisible)
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
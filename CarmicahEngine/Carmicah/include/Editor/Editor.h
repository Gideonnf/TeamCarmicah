#ifndef EDITOR_H_
#define EDITOR_H_
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "Editor/DebugWindow.h"
#include "Editor/EditorWindow.h"
#include "Editor/HeirarchyWindow.h"

namespace Carmicah
{
	class Editor
	{
	public:

		Editor();

		~Editor();

		void Init(GLFWwindow* window);

		void Update();

		void Render(GLFWwindow* window);

		void Exit();

	private:

		std::vector<std::unique_ptr<EditorWindow>> windows;
	};

}

#endif

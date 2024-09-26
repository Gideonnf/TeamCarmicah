#ifndef GUI_H_
#define GUI_H_
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>

namespace Carmicah
{
	class Editor
	{
	public:

		Editor();

		~Editor();

		void Init(GLFWwindow* window, const char* version);

		void Update();

		void Render();

		void Exit();

	private:

	};

}

#endif

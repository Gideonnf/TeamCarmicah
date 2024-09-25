#ifndef GUI_H_
#define GUI_H_
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>

namespace Carmicah
{
	class GUI
	{
	public:

		GUI();

		~GUI();

		void Init(GLFWwindow* window);

		void Update();

		void Render();

		void Exit();

	private:
		ImGuiIO io;
	};

}

#endif

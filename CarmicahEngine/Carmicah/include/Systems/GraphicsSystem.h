#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"
#include <GLFW/glfw3.h>


namespace Carmicah
{
	class GraphicsSystem : public BaseSystem
	{
	private:

	public:
		void Init();

		void Update();

		void Render(GLFWwindow*&);

		void Exit();
	};
}

#endif
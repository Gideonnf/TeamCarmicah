#pragma once
#include "GLFW/glfw3.h"
#include "CarmicahCore.h"
namespace Carmicah
{
	class Application
	{

	public:
		Application();
		virtual ~Application();
		int run();
	};
}


#ifndef CARMICAH_CORE_H_
#define CARMICAH_CORE_H_
#include <GLFW/glfw3.h>
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



#endif
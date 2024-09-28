#ifndef CARMICAH_CORE_H_
#define CARMICAH_CORE_H_

namespace Carmicah
{
	class Application
	{

	public:
		int Width, Height;
		std::string defaultScene;
		std::string lastScene;

		Application();
		virtual ~Application();
		int run();
	};
}



#endif
#ifndef CARMICAH_CORE_H_
#define CARMICAH_CORE_H_

namespace Carmicah
{
	class Application
	{

	public:
		Application();
		virtual ~Application();
		int run();
		void Import();
		void Export();
	};
}



#endif
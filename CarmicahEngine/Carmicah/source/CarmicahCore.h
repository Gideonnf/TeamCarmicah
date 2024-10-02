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

		GLuint FBO;
		GLuint RBO;
		GLuint texture_id;

		void create_framebuffer(int width, int height);
		void bind_framebuffer();
		void unbind_framebuffer();
		void rescale_framebuffer(float width, float height);
	};
}



#endif
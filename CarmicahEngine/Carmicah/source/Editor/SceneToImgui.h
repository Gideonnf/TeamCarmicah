#ifndef SCENE_TO_IMGUI_H
#define SCENE_TO_IMGUI_H
#include "../Singleton.h"

namespace Carmicah
{

	class SceneToImgui : public Singleton<SceneToImgui>
	{
	public:
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
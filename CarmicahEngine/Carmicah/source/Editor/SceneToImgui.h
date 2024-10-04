/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SceneToImgui.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A bridge to map the graphics rendering to ImGui. Binds rendering to a texture that can be mapped to an image in ImGui

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
		std::string currentScene;

		void create_framebuffer(int width, int height);
		void bind_framebuffer();
		void unbind_framebuffer();
		void rescale_framebuffer(float width, float height);

	};
}


#endif
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
		enum SCENE_IMGUI
		{
			GAME_SCENE = 0,
			EDITOR_SCENE,
			MAX_SCENES
		};

		struct FBOScene
		{
			GLuint FBO{};
			GLuint RBO{};
			GLuint texture_id{};
			GLuint picker_id{};
		};

		FBOScene mScenes[MAX_SCENES]{};
		std::string currentScene{};
		bool IsHovering;

		void CreateFramebuffer(int width, int height);
		void UnloadFramebuffer();
		void BindFramebuffer(SCENE_IMGUI scene);
		void UnbindFramebuffer();
		GLuint GetTexture(SCENE_IMGUI scene);
		unsigned int IDPick(SCENE_IMGUI scene, const int& mouseX, const int& mouseY);
		void RescaleFramebuffer(float width, float height);

	};
}


#endif
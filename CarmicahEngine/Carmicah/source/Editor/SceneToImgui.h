/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SceneToImgui.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			A bridge to map the graphics rendering to ImGui. Binds rendering to a texture that can be mapped to an image in ImGui

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
			MAX_SCENES,
			NO_SCENE
		};
		struct FBOScene
		{
			GLuint FBO{};
			//GLuint RBO{};
			GLuint texture_id{}, picker_id{};
			GLuint depthTex{}, accumTex{}, revealTex{};
			bool isHovering{};
		};
	private:

		SCENE_IMGUI currDrawingScene = NO_SCENE;
		FBOScene mScenes[MAX_SCENES]{};
		unsigned int mIDHovered;

		/*!*************************************************************************
		brief
			Finds the ID of the selected in-game game object given the mouse coordinates (based on the framebuffer's width and height)
		param[scene]
			the framebuffer to select
		param[mouseX]
			the mouse X position
		param[mouseY]
			the mouse Y position
		***************************************************************************/
		unsigned int IDPick(SCENE_IMGUI scene, const int& mouseX, const int& mouseY);
	public:
		std::string currentScene{};

		void CreateScreenQuad();
		/*!*************************************************************************
		brief
			Creates multiple framebuffers based on the enum SCENE_IMGUI
		param[width]
			the internal width of the frame buffer object to create
		param[height]
			the internal width of the frame buffer object to create
		***************************************************************************/
		void CreateFramebuffer(int width, int height);

		/*!*************************************************************************
		brief
			Unloads all the Frame buffer (gpu components)
		***************************************************************************/
		void UnloadFramebuffer();

		/*!*************************************************************************
		brief
			Sets which frame buffer to start drawing to
		param[scene]
			the framebuffer to draw to
		***************************************************************************/
		void SelectFrameBuffer(SCENE_IMGUI scene);

		/*!*************************************************************************
		brief
			Gets the data of the current Specified framebuffer
		***************************************************************************/
		FBOScene GetCurrentFramebuffer();

		/*!*************************************************************************
		brief
			Unbinds the framebuffer
		***************************************************************************/
		void UnbindFramebuffer();

		/*!*************************************************************************
		brief
			Selects the texture of the frame buffer object
		param[scene]
			the framebuffer to select
		***************************************************************************/
		GLuint GetTexture(SCENE_IMGUI scene);

		/*!*************************************************************************
		brief
			Finds the ID of what the mouse is hovering
		***************************************************************************/
		void SelectMouseIDObjPick();

		/*!*************************************************************************
		brief
			Gets the already selected ID
		***************************************************************************/
		unsigned int GetIDObjPick();

		/*!*************************************************************************
		brief
			Sets the isHovering flag for the imgui rendering scene to know that the input should be handled by imgui side instead of glfw side
		param[scene]
			the framebuffer to select
		param[hoverState]
			the is hovering state to set
		***************************************************************************/
		void SetHovering(SCENE_IMGUI scene, bool hoverState);

		/*!*************************************************************************
		brief
			Gets which (the first found) IMGUI scene is being hovered over
		***************************************************************************/
		SCENE_IMGUI GetHovering();
	};
}


#endif
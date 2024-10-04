/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Graphics System handles the rendering of textured meshes onto the screen (using transform, camera reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class GraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		GLuint mCurrShader{};
	public:
		/*
		brief
			Sets which components the system references, and load the shader
		*/
		void Init();

		/*
		brief
			Sets the data of the camera based on screen size
		*/
		void SetScreenSize(GLuint camWidth, GLuint camHeight, Entity& cam);

		/*
		brief
			Renders the Debug line of entities
		param[cam]
			camera dimensions and position to reference
		*/
		void Render(Entity& cam);

	};
}

#endif
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			WorldGraphicsSystem.h

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
	class WorldGraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		/*!*************************************************************************
		brief
			Force sets the data of the camera based on screen size
		***************************************************************************/
		void SetScreenSize(GLuint camWidth, GLuint camHeight, Entity& cam);


		void EntityDestroyed(Entity id);

		/*!*************************************************************************
		brief
			Updates the entities position in buffer
		***************************************************************************/
		void Update();

	};
}

#endif
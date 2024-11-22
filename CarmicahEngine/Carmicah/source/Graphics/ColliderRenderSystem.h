/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ColliderRenderSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles rendering the collision boxes of gameobjects with the collider2D component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef COLLIDER_RENDERER_SYSTEM_H
#define COLLIDER_RENDERER_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class ColliderRenderSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		std::string primitive;
	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		void EntityDestroyed(Entity id);

		/*!*************************************************************************
		brief
			Renders the Debug line of entities
		param[cam]
			camera dimensions and position to reference
		***************************************************************************/
		void Update();
	};
}

#endif
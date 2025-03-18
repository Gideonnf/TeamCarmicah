/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RigidbodyGraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Rigidbody Graphics System handles rendering the rigidbody parts such as velocity of gameobjects with the rigidbody component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RIGIDBODY_GRAPHICS_SYSTEM_H
#define RIGIDBODY_GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class RigidbodyGraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		std::string primitive;
	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		/*!*************************************************************************
		brief
			Remove the entity from the buffer when it is destroyed
		param[id]
			the entity id being destroyed
		***************************************************************************/
		void EntityDestroyed(Entity id);

		/*!*************************************************************************
		brief
			Renders the Debug line of entities
		***************************************************************************/
		void Update();
	};
}

#endif
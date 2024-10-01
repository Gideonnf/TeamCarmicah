/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AnimationSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles the switching and behaviour of gameobjects with the animator and renderer component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class AnimationSystem : public BaseSystem
	{
	public:
		void Init();

		void Update();
	};
}

#endif
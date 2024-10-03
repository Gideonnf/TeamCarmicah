/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        PhysicsSystem.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the Class declaration of the Physics System as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class PhysicsSystem : public BaseSystem
	{
	public:
		void Init();

		void Update();

		void Exit();

		void ApplyForce(Entity& obj);

		void UpdatePosition(Entity& obj);

		bool mDebugPhysics;
		bool mToggleUpdate;
	};
}


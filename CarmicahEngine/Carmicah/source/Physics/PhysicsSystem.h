/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        PhysicsSystem.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the Class declaration of the Physics System as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
#pragma once
#include "ECS/BaseSystem.h"

//berif:         This class is a system that handles the physics of all the game objects
namespace Carmicah
{
	class PhysicsSystem : public BaseSystem
	{
	public:
		void Init();

		void Update();

		void Exit();

		void Integrate(Entity& obj);

		void UpdatePosition(Entity& obj);

		bool mDebugPhysics;
		bool mToggleUpdate;
	};
}
#endif
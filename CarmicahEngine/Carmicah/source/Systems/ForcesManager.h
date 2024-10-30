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
#ifndef FORCES_MANAGER_H
#define FORCES_MANAGER_H

#include "Math/Vec2.h"
#include "Components/Forces.h"
#include <vector>


//berif:         This class is a system that handles the physics of all the game objects
namespace Carmicah
{
	class ForcesManager 
	{
		void UpdateForces(Entity& obj, float deltaTime);

		void ApplyForces(Entity& obj, Vector2D<float>& velocity);
	};
}

#endif


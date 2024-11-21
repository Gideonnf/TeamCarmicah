/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:         CollisionSystem.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the Class declaration of the Collision System as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class CollisionSystem : public BaseSystem
	{
	public:
		void PrintEntities();

		void UpdateAABB(Entity& obj);

		void GetOBBVertices(std::vector<Vec2f>& vertices, Entity& obj) const;

		bool CollisionIntersect(Entity& obj1, Entity& obj2, float firstTimeOfCollision);

		void CollisionResponse(Entity& obj1, Entity& obj2, float tFirst);

		void StaticDynamicCollisionCheck(Entity& obj1, Entity& obj2);

		void CollisionCheck();

		void Init();

		void Update();

		void Exit();
	};
}


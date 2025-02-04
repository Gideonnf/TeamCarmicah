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
#include "../Math/Vec2.h"

namespace Carmicah
{
	class CollisionSystem : public BaseSystem
	{
	public:
		void PrintEntities();

		void UpdateOBB(Entity& obj);

		void ComputeProjInterval(Entity& obj, Vec2f edgeNormal, float& min, float& max);

		void GetOBBVertices(Entity& obj);

		void CalculateEdges(Entity& obj);

		float CalculatePenetrationDepth(Entity& obj1, Entity& obj2);

		Vec2f CalculateCollisionNormal(Entity& obj1, Entity& obj2);

		void ResolvePenetration(Entity& obj1, Entity& obj2);
		
		int WhichSide(std::vector<Vec2f>& otherVertices, Vec2f& point, Vec2f& normal);

		bool TestIntersection(Entity& obj1, Entity& obj2);

		void CollisionResponse(Entity& obj1, Entity& obj2);

		void CollisionCheck();

		void Init();

		void Update();

		void Exit();
	};
}


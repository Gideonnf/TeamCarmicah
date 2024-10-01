#pragma once
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class CollisionSystem : public BaseSystem
	{
	public:
		void PrintEntities();

		void UpdateAABB(Entity& obj);

		bool CollisionIntersect(Entity& obj1, Entity& obj2, float tFirst);

		void CollisionResponse(Entity& obj1, Entity& obj2, float tFirst);

		void CollisionCheck();

		void Init();

		void Update();

		void Exit();
	};
}


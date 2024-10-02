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

		void CollisionResponse(Entity& obj1, Entity& obj2, float firstTimeOfCollision);
	};
}


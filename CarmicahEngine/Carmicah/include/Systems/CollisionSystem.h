#pragma once
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class CollisionSystem : public BaseSystem
	{
	public:
		void PrintEntities();

		void Init();

		void Update();

		void Exit();
	};
}


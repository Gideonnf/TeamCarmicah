#pragma once
#include "BaseSystem.h"
class CollisionSystem : public BaseSystem
{
public:
	void PrintEntities();

	void Init();

	void Update();

	void Exit() ;
};


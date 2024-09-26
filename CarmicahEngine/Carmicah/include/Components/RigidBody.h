#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Math/Vec2.h>

namespace Carmicah 
{
	struct RigidBody
	{
		Vector2D<float> velocity;
		Vector2D<float> acceleration;
		float posPrev;

		bool isKinematic;
		bool isStatic;
	};
}

#endif
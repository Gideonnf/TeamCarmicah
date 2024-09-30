#pragma once
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Math/Vec2.h>

namespace Carmicah 
{
	enum ObjectType
	{
		KINEMATIC,
		STATIC,
		DYNAMIC
	};

	struct RigidBody
	{
		Vector2D<float> velocity;
		Vector2D<float> acceleration;
		Vector2D<float> posPrev; //Putting it here first but may move it to transform

		float zposPrev;

		ObjectType objectType;
		
	};
}

#endif
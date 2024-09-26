#pragma once
#ifndef BODY_H
#define BODY_H

#include <Math/Vec2.h>

namespace Carmicah 
{
	struct Body
	{
		Vector2D<float> velocity;
		Vector2D<float> acceleration;
		float mass;
	};
}

#endif
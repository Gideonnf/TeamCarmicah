#pragma once
#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#include <Math/Vec2.h>

namespace Carmicah
{
    struct Collider2D
    {
        Carmicah::Vector2D<float> min;
        Carmicah::Vector2D<float> max;
    };
}

#endif
#pragma once
#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#include <Math/Vec2.h>

namespace Carmicah
{
    struct Collider2D
    {
        Vector2D<float> min;
        Vector2D<float> max;
    };
}

#endif
#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

namespace Carmicah
{
    struct Transform
    {
        float xPos;
        float yPos;
        float zPos;

        float rot;

        float xScale;
        float yScale;

        bool isUpdated;
        glm::mat3 worldSpace;
        glm::mat3 camSpace;
    };
}

#endif
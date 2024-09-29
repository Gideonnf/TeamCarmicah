#pragma once
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include <string>
#include <glm/glm.hpp>

namespace Carmicah
{
    struct Renderer
    {
        std::string model;
        std::string texture;
        glm::mat3 texureMat;
    };
}

#endif
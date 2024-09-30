#ifndef TEXT_RENDERER_COMPONENT_H
#define TEXT_RENDERER_COMPONENT_H

#include <string>
#include <glm/glm.hpp>

namespace Carmicah
{
    struct TextRenderer
    {
        std::string model;
        std::string font;
        std::string txt;
        glm::vec3 color;
    };
}

#endif
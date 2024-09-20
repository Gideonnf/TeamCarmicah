#pragma once

namespace Carmicah
{
    struct Renderer
    {
        enum PRIMITIVE{
            SQUARE = 0,
            CIRCLE
        };
        PRIMITIVE primitiveType;
    };
}
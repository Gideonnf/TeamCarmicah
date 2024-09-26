#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

namespace Carmicah
{
    struct Animation
    {
        int currPiece;
        int xSlice;
        int ySlice;
        float time;
        float maxTime;
    };
}

#endif
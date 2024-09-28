#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include "BaseComponent.h"
namespace Carmicah
{
    struct Animation //: public BaseComponent<Animation>
    {
        int currPiece;
        int xSlice;
        int ySlice;
        float time;
        float maxTime;

        //virtual Animation DeserializeComponent(const rapidjson::Value& component) override
        //{

        //}
    };
}

#endif
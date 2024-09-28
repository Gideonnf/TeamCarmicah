#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include "BaseComponent.h"
namespace Carmicah
{
    struct Animation : public BaseComponent<Animation>
    {
        int currPiece;
        int xSlice;
        int ySlice;
        float time;
        float maxTime;

        Animation& DeserializeComponent(const rapidjson::Value& component) override
        {
            return *this;
        }

        void SerializeComponent(rapidjson::Writer<rapidjson::OStreamWrapper>& writer) override
        {

        }
    };
}

#endif
#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include "BaseComponent.h"
namespace Carmicah
{
    struct Animation : public BaseComponent<Animation>
    {
        bool notChangedAnim;
        int currPiece;
        int xSlice;
        int ySlice;
        float time;
        float maxTime;

        Animation& DeserializeComponent(const rapidjson::Value& component) override
        {
            maxTime = static_cast<float>(component["timeBetween"].GetDouble());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
			writer.String("timeBetween");
			writer.Double(maxTime);
        }
    };
}

#endif
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Animation.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of current Animation details

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
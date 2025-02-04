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
    protected:
    public:
        std::string animAtlas;
        enum class ANIM_CODE : char
        {
            ANIM_CHANGED= -2,
            STOP_ANIM   = -1,

            INF_LOOP    = 0,
            FINITE      = 1
        };

        ANIM_CODE animState = ANIM_CODE::ANIM_CHANGED;
        int currPiece, loopsLeft;
        float time, maxTime;

        std::string& AnimAtlas()
        {
            animState = ANIM_CODE::ANIM_CHANGED;
            return animAtlas;
        }

        const std::string& GetAnimAtlas() const
        {
            return animAtlas;
        }

        void ChangeAnim(const std::string& animName)
        {
            animAtlas = animName;
            animState = ANIM_CODE::ANIM_CHANGED;
        }

        Animation& DeserializeComponent(const rapidjson::Value& component) override
        {
            animAtlas = component["Atlas"].GetString();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("Atlas");
			writer.String(animAtlas.c_str());
        }
    };
}

#endif
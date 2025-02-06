/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			UITransform.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of UI Specific transforms
                uses (0,0) as bottom left, (WIDTH,HEIGHT) as top right
                position is based on pixels

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef UI_TRANSFORM_H
#define UI_TRANSFORM_H
#include "BaseTransform.h"

namespace Carmicah
{

    struct UITransform : BaseTransform<UITransform>
    {
        bool mouseEnter = false;

        UITransform& DeserializeComponent(const rapidjson::Value& component) override
        {
            BaseTransform::DeserializeComponentBuffer(component);
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            BaseTransform::SerializeComponent(writer);
        }
    };
}

#endif
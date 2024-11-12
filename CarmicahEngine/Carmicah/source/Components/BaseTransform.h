/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseTransform.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of transform data shared by both UI Objects, and WorldSpace Objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BASE_TRANSFORM_H
#define BASE_TRANSFORM_H

#include "BaseComponent.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    template <typename T>
    struct BaseTransform : BaseComponent<T>
    {
        Vec2f pos;
        Vec2f scale;
        float depth;

        bool notUpdated;

        Entity parent; // Hold 0 if no parent
        std::vector<Entity> children;
        unsigned int mPrefabRef; // Keep a ref to which prefab it was made from if any. If none then 0


        void DeserializeComponentBuffer(const rapidjson::Value& component)
        {
            pos.x = static_cast<float>(component["xPos"].GetDouble());
            pos.y = static_cast<float>(component["yPos"].GetDouble());
            depth = static_cast<float>(component["depth"].GetDouble());
            scale.x = static_cast<float>(component["xScale"].GetDouble());
            scale.y = static_cast<float>(component["yScale"].GetDouble());
        }

        virtual void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("xPos");
            writer.Double(pos.x);
            writer.String("yPos");
            writer.Double(pos.y);
            writer.String("depth");
            writer.Double(depth);
            writer.String("xScale");
            writer.Double(scale.x);
            writer.String("yScale");
            writer.Double(scale.y);

        }
    };
}


#endif
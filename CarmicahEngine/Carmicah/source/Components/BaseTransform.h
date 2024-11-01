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
#ifndef UI_TRANSFORM_H
#define UI_TRANSFORM_H
#include "BaseComponent.h"

namespace Carmicah
{
    struct UITransform : BaseComponent<UITransform>
    {
        float xPos;
        float yPos;

        //float rot;

        float xScale;
        float yScale;

        UITransform& DeserializeComponent(const rapidjson::Value& component) override
        {
            xPos = static_cast<float>(component["xPos"].GetDouble());
            yPos = static_cast<float>(component["yPos"].GetDouble());
            xScale = static_cast<float>(component["xScale"].GetDouble());
            yScale = static_cast<float>(component["yScale"].GetDouble());

            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("xPos");
            writer.Double(xPos);
            writer.String("yPos");
            writer.Double(yPos);
            writer.String("xScale");
            writer.Double(xScale);
            writer.String("yScale");
            writer.Double(yScale);

        }
    };
}

#endif
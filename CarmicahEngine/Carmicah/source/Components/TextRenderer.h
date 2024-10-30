/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextRenderer.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of Text specific details such as the model(quad), font, text(words), and color

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef TEXT_RENDERER_COMPONENT_H
#define TEXT_RENDERER_COMPONENT_H

#include <string>
#include "BaseComponent.h"
namespace Carmicah
{
    struct TextRenderer : BaseComponent<TextRenderer>
    {
        std::string model;
        std::string font;
        std::string txt;
        float colorR, colorG, colorB;

        TextRenderer& DeserializeComponent(const rapidjson::Value& component) override
        {
            model = component["model"].GetString();
            font = component["font"].GetString();
            txt = component["text"].GetString();
            colorR = component["colorR"].GetFloat();
            colorG = component["colorG"].GetFloat();
            colorB = component["colorB"].GetFloat();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("model");
            writer.String(model.c_str());
            writer.String("font");
            writer.String(font.c_str());
            writer.String("text");
            writer.String(txt.c_str());
            writer.String("colorR");
            writer.Double(colorR);
            writer.String("colorG");
            writer.Double(colorG);
            writer.String("colorB");
            writer.Double(colorB);

        }
    };
}

#endif
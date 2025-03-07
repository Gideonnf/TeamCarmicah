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
        enum TXTALIGN : unsigned short
        {
            TXT_LEFT    = 0x00,
            TXT_MIDDLE  = 0x10,
            TXT_RIGHT   = 0x20,
            TXT_CHECKX  = 0x30,
            TXT_BOT     = 0x00,
            TXT_CENTER  = 0x01,
            TXT_TOP     = 0x02,
            TXT_CHECKY  = 0x03
        };

        std::string font;
        std::string txt;
        std::string oldTxt;
        float colorR, colorG, colorB;
        float totalWidth;
        float totalHeight;
        unsigned short txtAlign;

        void ChangeTextAlignmentX(TXTALIGN lr)
        {
            txtAlign = (txtAlign & TXT_CHECKY) | lr;
        }
        void ChangeTextAlignmentY(TXTALIGN tb)
        {
            txtAlign = (txtAlign & TXT_CHECKX) | tb;
        }
        std::string TextAlignmentName()
        {
            switch (txtAlign)
            {
            case 0x00:
                return "Bottom Left";
                break;
            case 0x01:
                return "Center Left";
                break;
            case 0x02:
                return "Top Left";
                break;
            case 0x10:
                return "Bottom Middle";
                break;
            case 0x11:
                return "Center Middle";
                break;
            case 0x12:
                return "Top Middle";
                break;
            case 0x20:
                return "Bottom Right";
                break;
            case 0x21:
                return "Center Right";
                break;
            case 0x22:
                return "Top Right";
                break;
            }
        }

        TextRenderer& DeserializeComponent(const rapidjson::Value& component) override
        {
            font = component["font"].GetString();
            txt = component["text"].GetString();
            colorR = component["colorR"].GetFloat();
            colorG = component["colorG"].GetFloat();
            colorB = component["colorB"].GetFloat();
            if (component.HasMember("textAlign"))
                txtAlign = static_cast<unsigned short>(component["textAlign"].GetInt());
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
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
            writer.String("textAlign");
            writer.Int(static_cast<int>(txtAlign));

        }
    };
}

#endif
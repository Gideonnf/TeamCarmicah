/* file documentation -----------------------------------------------------------------------------
\file
\author     Micah Lim, (micahshengyao.lim)
\course     CSD 2400
\date

\brief

\functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/


#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include "BaseComponent.h"
#include "Renderer.h"
#include <string>
#include "Math/Vec2.h"
#include "Math/Matrix3x3.h"

namespace Carmicah
{
    struct Button : BaseComponent<Button>
    {
		std::string ButtonOG;   // stores the name of the original texture
		std::string ButtonPress;    // stores the name of the new image when its pressed
        bool isPressed{ false }; // track if button is currently pressed

        Button& DeserializeComponent(const rapidjson::Value& component) override
        {
            ButtonOG = component["ButtonOG"].GetString();
            ButtonPress = component["ButtonPress"].GetString();
			isPressed = false; // start as unpressed
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("ButtonOG");
            writer.String(ButtonOG.c_str());
            writer.String("ButtonPress");
            writer.String(ButtonPress.c_str());
        }

		// call this function when button is pressed
		void onPress()
		{
			isPressed = true; // set to pressed
            Renderer.texture = ButtonPress;

            // need to put logic to switch texture here
		}

		// call this function when button is released
        void onRelease()
        {
			isPressed = false; // set to unpressed
			Renderer.texture = ButtonOG;

            // revert to og texture here
        }
    };
}

#endif
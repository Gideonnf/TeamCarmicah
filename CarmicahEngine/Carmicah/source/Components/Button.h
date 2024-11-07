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
#include "../Components/Renderer.h"
#include <string>
#include <cmath>
#include "Math/Vec2.h"
#include "Math/Matrix3x3.h"

// use SceneWindow for the actual detection of clicks or input over the button. 
// SceneWindow can detect if the mouse is over the button and simply
// call button->OnPress() or button->OnRelease() as needed. 
// This keeps SceneWindow focused on input detection without delving into button state management.
// Button will manage its own state and texture switching.

namespace Carmicah
{
    struct Button : BaseComponent<Button>
    {
		std::string ButtonOG;   // stores the name of the original texture
		std::string ButtonPress;    // stores the name of the new image when its pressed
        bool isPressed{ false }; // track if button is currently pressed
        Vec2d position;  // New member for position
        Vec2d size;      // New member for size

        Button& DeserializeComponent(const rapidjson::Value& component) override
        {
            ButtonOG = component["ButtonOG"].GetString();
            ButtonPress = component["ButtonPress"].GetString();
			isPressed = false; // start as unpressed

            // Deserialize position and size from JSON
            position.x = component["xPos"].GetFloat();
            position.y = component["yPos"].GetFloat();
            size.x = component["xScale"].GetFloat();
            size.y = component["yScale"].GetFloat();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("ButtonOG");
            writer.String(ButtonOG.c_str());
            writer.String("ButtonPress");
            writer.String(ButtonPress.c_str());
        }

        Vec2d GetPosition() const { return position; }
        Vec2d GetSize() const { return size; }
    };
}

#endif
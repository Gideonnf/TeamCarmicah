/* file documentation -----------------------------------------------------------------------------
\file       Button.h
\author     Micah Lim (100%)
\course     CSD 2400
\date       071124

\brief      Defines the Button component, representing an interactive UI element with states for
            pressed and released, including its position and size.

\functions  - DeserializeComponent: Populates the Button component properties from JSON data.
            - SerializeComponent: Writes Button component properties to JSON data.
            - GetPosition: Retrieves the button's position.
            - GetSize: Retrieves the button's size.

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
		std::string ButtonImage;        // stores the name of the original texture
		std::string ButtonImagePressed; // stores the name of the new image when its pressed
        bool isPressed{ false };        // track if button is currently pressed

        Button& DeserializeComponent(const rapidjson::Value& component) override
        {
            ButtonImage = component["ButtonImage"].GetString();
            ButtonImagePressed = component["ButtonImagePressed"].GetString();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("ButtonImage");
            writer.String(ButtonImage.c_str());
            writer.String("ButtonImagePressed");
            writer.String(ButtonImagePressed.c_str());
        }
    };
}

#endif
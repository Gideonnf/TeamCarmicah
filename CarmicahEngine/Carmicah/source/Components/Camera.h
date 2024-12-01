/* File Documentation -----------------------------------------------------------------------------
file:           CameraComponent.h

author:		    Won Yu Xuan Rainne(100%)

email:			won.m@digipen.edu

brief:          This file defines the Camera structure, inheriting from BaseComponent. It provides
                functionality to serialize and deserialize camera-related data, specifically whether
                the camera is the main camera in the scene, represented by a boolean field.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H
#include "BaseComponent.h"


namespace Carmicah
{
	struct Camera : BaseComponent<Camera>
	{
        bool mainCamera;

        Camera& DeserializeComponent(const rapidjson::Value& component) override
        {
            mainCamera = component["mainCamera"].GetBool();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("mainCamera");
            writer.Bool(mainCamera);

        }
	};
}


#endif

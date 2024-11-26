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

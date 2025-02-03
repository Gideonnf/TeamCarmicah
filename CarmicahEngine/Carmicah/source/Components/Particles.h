/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Particles.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of Particles details

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PARTICLES_COMPONENT_H
#define PARTICLES_COMPONENT_H

#include <string>
#include "BaseComponent.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    struct Particles : BaseComponent<Particles>
    {
    public:
        std::string texture;
        Vec2f particleSpeed;
        float spawnPerSec, speedRange;
        bool drawOverUI, isBurst;

        Particles() : texture("Default"), particleSpeed(), spawnPerSec(), speedRange(), drawOverUI(), isBurst()
        {

        }

        Particles& DeserializeComponent(const rapidjson::Value& component) override
        {
            //texture = component["texture"].GetString();
            
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            //writer.String("texture");
            //writer.String(texture.c_str());
        }
    };
}

#endif
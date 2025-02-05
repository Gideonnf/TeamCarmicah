/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ParticleEmitter.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of Particles details

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PARTICLE_EMITTER_COMPONENT_H
#define PARTICLE_EMITTER_COMPONENT_H

#include <string>
#include "BaseComponent.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    struct ParticleEmitter : BaseComponent<ParticleEmitter>
    {
    public:
        std::string texture;
        Vec2f particleSpeed;    // Direction as well
        size_t spawnPerSec;     // Number of particles to spawn per sec
        float timePassed,       // Counting how long ago since a last particle spawn
            lifeTime,           // Particle lifetime
            speedRange,         // +-= Speed (equily on both x&y)
            angleRange;         // Rotate by +- angle/2
        bool isFade,             // Will fade to 0
            isShrink;           // Will shrink to 0

        ParticleEmitter() : texture("Default"),
            particleSpeed(),
            spawnPerSec(),
            timePassed(), lifeTime(), speedRange(), angleRange(),
            isFade(), isShrink()
        {

        }

        ParticleEmitter& DeserializeComponent(const rapidjson::Value& component) override
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
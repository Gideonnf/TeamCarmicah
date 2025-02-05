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
            texture         = component["texture"].GetString();
            particleSpeed.x = static_cast<float>(component["VelX"].GetDouble());
            particleSpeed.y = static_cast<float>(component["VelY"].GetDouble());
            spawnPerSec     = static_cast<size_t>(component["SpawnPSec"].GetInt());
            lifeTime        = static_cast<float>(component["Lifetime"].GetDouble());
            speedRange      = static_cast<float>(component["SpeedRange"].GetDouble());
            angleRange      = static_cast<float>(component["AngleRange"].GetDouble());
            isFade          = component["WillFade"].GetBool();
            isShrink        = component["WillShrink"].GetBool();
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("texture");
            writer.String(texture.c_str());
            writer.String("VelX");
            writer.Double(static_cast<double>(particleSpeed.x));
            writer.String("VelY");
            writer.Double(static_cast<double>(particleSpeed.y));
            writer.String("SpawnPSec");
            writer.Int(static_cast<int>(spawnPerSec));
            writer.String("Lifetime");
            writer.Double(static_cast<double>(lifeTime));
            writer.String("SpeedRange");
            writer.Double(static_cast<double>(speedRange));
            writer.String("AngleRange");
            writer.Double(static_cast<double>(angleRange));
            writer.String("WillFade");
            writer.Bool(isFade);
            writer.String("WillShrink");
            writer.Bool(isShrink);
        }
    };
}

#endif
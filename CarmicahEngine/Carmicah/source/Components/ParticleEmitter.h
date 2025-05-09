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
        enum EMITTER_TYPE : unsigned char
        {
            EMITTER_ACTIVE      = 0x01,
            EMITTER_BURST       = 0x02,
            PARTICLES_FADE      = 0x04,
            PARTICLES_SHRINK    = 0x08, // --TODO--
            PARTICLES_FRICTION  = 0x10,
            PARTICLES_GRAVITY   = 0x20,
            PARTICLES_RADIAL    = 0x40
        };

        std::string texture;
        Vec2f particleSpeed,    // Direction as well
            spawnRadius,        // Pos Spawn Radius
            scaleRange;         // Scale Range
        size_t spawnPerSec;     // Number of particles to spawn per sec
        float particleLifeTime, // Particle lifetime
            spawnBurstTime,     // How long to burst spawn
            speedRange,         // +-= Speed (equily on both x&y)
            angleRange;         // Rotate by +- angle/2
        float timePassed,       // Counting how long ago since a last particle spawn
            optAliveTime,       // If is burst, how long alive
            colR,
            colG,
            colB,
            colA;
        unsigned char emitterStatus;

        ParticleEmitter() : texture("Default"),
            particleSpeed(), spawnRadius(), scaleRange(1.f, 1.f),
            spawnPerSec(),
            particleLifeTime(), spawnBurstTime(), speedRange(), angleRange(),
            timePassed(), optAliveTime(), colR(1.f), colG(1.f), colB(1.f), colA(1.f),
            emitterStatus(EMITTER_ACTIVE)
        {
        }

        bool HasEmitterQualities(EMITTER_TYPE test)
        {
            return (emitterStatus & test) != 0;
        }

        void SetEmitterQualities(EMITTER_TYPE test, bool toggleTrue)
        {
            if (toggleTrue)
            {
                emitterStatus = emitterStatus | test;
                if (test & EMITTER_ACTIVE)
                    optAliveTime = spawnBurstTime;
            }
            else if (HasEmitterQualities(test))
                emitterStatus = emitterStatus ^ test;
        }

        ParticleEmitter& DeserializeComponent(const rapidjson::Value& component) override
        {
            texture         = component["texture"].GetString();
            particleSpeed.x = static_cast<float>(component["VelX"].GetDouble());
            particleSpeed.y = static_cast<float>(component["VelY"].GetDouble());
            spawnRadius.x   = static_cast<float>(component["RadiusX"].GetDouble());
            spawnRadius.y   = static_cast<float>(component["RadiusY"].GetDouble());
            scaleRange.x    = static_cast<float>(component["SizeRangeMin"].GetDouble());
            scaleRange.y    = static_cast<float>(component["SizeRangeMax"].GetDouble());
            spawnPerSec     = static_cast<size_t>(component["SpawnPSec"].GetInt());
            particleLifeTime= static_cast<float>(component["Lifetime"].GetDouble());
            optAliveTime = spawnBurstTime  = static_cast<float>(component["BurstTime"].GetDouble());
            speedRange      = static_cast<float>(component["SpeedRange"].GetDouble());
            angleRange      = static_cast<float>(component["AngleRange"].GetDouble());
            emitterStatus   = static_cast<unsigned char>(component["EmitterStatus"].GetInt());
            if(component.HasMember("ColR"))
                colR   = static_cast<float>(component["ColR"].GetDouble());
            if(component.HasMember("ColG"))
                colG = static_cast<float>(component["ColG"].GetDouble());
            if(component.HasMember("ColB"))
                colB = static_cast<float>(component["ColB"].GetDouble());
            if(component.HasMember("ColA"))
                colA = static_cast<float>(component["ColA"].GetDouble());
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
            writer.String("RadiusX");
            writer.Double(static_cast<double>(spawnRadius.x));
            writer.String("RadiusY");
            writer.Double(static_cast<double>(spawnRadius.y));
            writer.String("SizeRangeMin");
            writer.Double(static_cast<double>(scaleRange.x));
            writer.String("SizeRangeMax");
            writer.Double(static_cast<double>(scaleRange.y));
            writer.String("SpawnPSec");
            writer.Int(static_cast<int>(spawnPerSec));
            writer.String("Lifetime");
            writer.Double(static_cast<double>(particleLifeTime));
            writer.String("BurstTime");
            writer.Double(static_cast<double>(spawnBurstTime));
            writer.String("SpeedRange");
            writer.Double(static_cast<double>(speedRange));
            writer.String("AngleRange");
            writer.Double(static_cast<double>(angleRange));
            writer.String("EmitterStatus");
            writer.Int(static_cast<int>(emitterStatus));
            writer.String("ColR");
            writer.Double(static_cast<double>(colR));
            writer.String("ColG");
            writer.Double(static_cast<double>(colG));
            writer.String("ColB");
            writer.Double(static_cast<double>(colB));
            writer.String("ColA");
            writer.Double(static_cast<double>(colA));
        }
    };
}

#endif
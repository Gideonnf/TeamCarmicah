/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SoundSystem.h

 author:		YANG YUJIE(70%)
 co-author(s):	Won Yu Xuan Rainne(30%)

 email:			won.m@digipen.edu

 brief:			Sound Manager 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "FMOD/fmod.hpp"
#include <string>

namespace Carmicah
{
    class SoundSystem : public BaseSystem
    {
    private:
        std::unordered_map<std::string, FMOD::Sound*> soundMap;
        std::unordered_map<std::string, FMOD::Channel*> channelMap;
        FMOD::System* fmodSystem;

    public:
        SoundSystem();
        ~SoundSystem();

        void Init(bool playDefaultBGM = true);
        void Update();
        void Exit();

        void LoadSound(const std::string& soundName, const std::string& filePath, bool isLooping = false);
        void PlaySound(const std::string& soundName, float volume = 1.0f);
        void StopSound(const std::string& soundName);
        void SetVolume(const std::string& soundName, float volume);
        void StopAllSounds();
        void PauseResumeSound(const std::string& soundName);


        const char* defaultBGM{ "bouken" };
    };
}

#endif
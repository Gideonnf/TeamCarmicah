/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SoundSystem.h

 author:		YANG YUJIE(70%)
 co-author(s):	Won Yu Xuan Rainne(30%)

 email:			won.m@digipen.edu
                y.yujie@digipen.edu

 brief:	        This file contains the declaration of the SoundSystem class, which is responsible for playing sound effects and background music.
				it uses the FMOD library to handle sound effects and background music.
				it also contains the declaration of the SoundComponent class, which is responsible for storing the sound name of an entity.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "FMOD/fmod.hpp"
#include <string>


//breif:         This class is a system that handles the sound effects and background music of the game

namespace Carmicah
{
    class SoundSystem : public BaseSystem
    {
    private:
        std::unordered_map<std::string, FMOD::Channel*> channelMap;

    public:
        SoundSystem();
        ~SoundSystem();

        void Init(bool playDefaultBGM = true);
        void Update();
        void Exit();

        void PlaySound(const std::string& soundName, float volume = 1.0f);
        void StopSound(const std::string& soundName);
        void SetVolume(const std::string& soundName, float volume);
        void StopAllSounds();
        void PauseResumeSound(const std::string& soundName);
		void PauseResumeAllSounds();


        const char* defaultBGM{ "bouken" };
        const char* buttonBGM{ "buttonclick" };
    };
}

#endif
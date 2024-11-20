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
#include "pch.h"
#include "Systems/SoundSystem.h"
#include <ECS/ECSTypes.h>
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"


//brief:         This class is a system that handles the sound effects and background music of the game

namespace Carmicah
{
    SoundSystem::SoundSystem() {}

    SoundSystem::~SoundSystem()
    {
        Exit();
    }

    //This is the signature of the sound system which is used to determine if an entity should be processed by the system
    void SoundSystem::Init(bool playDefaultBGM)
    {

        // Play default BGM
        if (playDefaultBGM)
        {
            PlaySound(defaultBGM, 0.3f);
        }

        if (playDefaultBGM)
        {
            PlaySound(buttonBGM, 0.5f);
        }



        SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
    }


    //This function is used to play a sound file from the sound system
    void SoundSystem::PlaySound(const std::string& soundName, float volume)
    {
        auto it = AssetManager::GetInstance()->mSoundMap.find(soundName);
        if (it != AssetManager::GetInstance()->mSoundMap.end())
        {
            FMOD::Channel* channel;
            AssetManager::GetInstance()->mSoundSystem->playSound(it->second.sound, nullptr, false, &channel);
            channel->setVolume(volume);
            channelMap[soundName] = channel;
        }
    }

    //This function is used to stop a sound file from playing
    void SoundSystem::StopSound(const std::string& soundName)
    {
        auto it = channelMap.find(soundName);
        if (it != channelMap.end())
        {
            it->second->stop();
            channelMap.erase(it);
        }
    }

    //This function is used to set the volume of a sound file that is currently playing
    void SoundSystem::SetVolume(const std::string& soundName, float volume)
    {
        auto it = channelMap.find(soundName);
        if (it != channelMap.end())
        {
            it->second->setVolume(volume);
        }
    }

    //This function is used to stop all sound files from playing in the sound system
    void SoundSystem::StopAllSounds()
    {
        for (auto& pair : channelMap)
        {
            pair.second->stop();
        }
        channelMap.clear();
    }

    //This function is used to pause or resume a sound file that is currently playing in the sound system
    void SoundSystem::PauseResumeSound(const std::string& soundName)
    {
        auto it = channelMap.find(soundName);
        if (it != channelMap.end())
        {
            bool paused;
            it->second->getPaused(&paused);
            it->second->setPaused(!paused);
        }
    }

    /* function documentation--------------------------------------------------------------------------
	\brief      Used to pause/resume all sound files that are currently playing in the sound system.
    -------------------------------------------------------------------------------------------------*/
    void SoundSystem::PauseResumeAllSounds()
	{
		for (auto& pair : channelMap)
		{
			bool paused;
			pair.second->getPaused(&paused);
			pair.second->setPaused(!paused);
		}
	}

    //This function is used to update the sound system and remove any stopped channels
    void SoundSystem::Update()
    {
        AssetManager::GetInstance()->mSoundSystem->update();

        // Remove stopped channels
        for (auto it = channelMap.begin(); it != channelMap.end();)
        {
            // If channel stops playing, remove from list
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);
            if (!isPlaying)
            {
                it = channelMap.erase(it);
            }
            // If channel is still playing, increment iterator
            else
            {
                ++it;
            }
        }
    }

    //This function is used to exit the sound system and release all resources used by the sound system
    void SoundSystem::Exit()
    {
        // Stop all sounds
        StopAllSounds();
    }
}
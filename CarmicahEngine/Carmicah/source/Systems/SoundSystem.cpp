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

//namespace Carmicah
//{
//	void SoundSystem::Init(bool play)
//	{
//		// Set the signature of the system
//		//mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
//		// Update the signature of the system
//		SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
//		if(play)
//			PlayAudio(defaultBGM, true);
//	}
//
//	void SoundSystem::PlayAudio(const std::string& sound, bool isBgm)
//	{
//		FMOD::Channel* channel = NULL;
//		AssetManager::GetInstance()->soundSystem->playSound(AssetManager::GetInstance()->soundMap[sound].sound, NULL, false, &channel);
//		sfxList.push_back(channel);
//	}
//
//	void SoundSystem::Update()
//	{
//		for (auto& it = sfxList.begin(); it != sfxList.end();)
//		{
//			// If channel stops playing, remove from list
//			bool bIsPlaying = false;
//			(*it)->isPlaying(&bIsPlaying);
//			if (!bIsPlaying)
//			{
//				it = sfxList.erase(it);
//			}
//			else
//				++it;
//		}
//		AssetManager::GetInstance()->soundSystem->update();
//	}
//}


namespace Carmicah
{
    SoundSystem::SoundSystem() : fmodSystem(nullptr) {}

    SoundSystem::~SoundSystem()
    {
        Exit();
    }

    //This is the signature of the sound system which is used to determine if an entity should be processed by the system
    void SoundSystem::Init(bool playDefaultBGM)
    {
        FMOD::System_Create(&fmodSystem);
        fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);

        // Load default BGM
        LoadSound(defaultBGM, "../Assets/BGM/bouken.mp3", true);

        // Play default BGM
        if (playDefaultBGM)
        {
            PlaySound(defaultBGM, 0.5f);
        }

        SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
    }

    //This function is used to load a sound file into the sound system
    void SoundSystem::LoadSound(const std::string& soundName, const std::string& filePath, bool isLooping)
    {
        FMOD::Sound* sound;
        FMOD_MODE mode = isLooping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
        fmodSystem->createSound(filePath.c_str(), mode, nullptr, &sound);
        soundMap[soundName] = sound;
    }

    //This function is used to play a sound file from the sound system
    void SoundSystem::PlaySound(const std::string& soundName, float volume)
    {
        auto it = soundMap.find(soundName);
        if (it != soundMap.end())
        {
            FMOD::Channel* channel;
            fmodSystem->playSound(it->second, nullptr, false, &channel);
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

    //This function is used to update the sound system and remove any stopped channels
    void SoundSystem::Update()
    {
        fmodSystem->update();

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

        // Release all sounds in the sound map
        for (auto& pair : soundMap)
        {
            pair.second->release();
        }
        soundMap.clear();

        // Release the FMOD system
        if (fmodSystem)
        {
            fmodSystem->release();
            fmodSystem = nullptr;
        }
    }
}
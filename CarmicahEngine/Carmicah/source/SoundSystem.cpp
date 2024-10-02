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
#include "pch.h"
#include "Systems/SoundSystem.h"
#include <ECS/ECSTypes.h>
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace Carmicah
{

    void SoundSystem::Init(bool playDefaultBGM)
    {
        SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);

        // Load default BGM
        LoadSound(defaultBGM, "../Assets/BGM/bouken.mp3", true);

        if (playDefaultBGM)
        {
            PlaySound(defaultBGM, 0.5f);
        }

    }

    void SoundSystem::PlaySound(const std::string& soundName, float volume)
    {
        auto& it = AssetManager::GetInstance()->mSoundMap.find(soundName);
        if (it != AssetManager::GetInstance()->mSoundMap.end())
        {
            FMOD::Channel* channel;
            AssetManager::GetInstance()->mSoundSystem->playSound(it->second.sound, nullptr, false, &channel);
            channel->setVolume(volume);
            AssetManager::GetInstance()->mChannelMap[soundName] = channel;
        }
    }

    void SoundSystem::StopSound(const std::string& soundName)
    {
        auto& it = AssetManager::GetInstance()->mChannelMap.find(soundName);
        if (it != AssetManager::GetInstance()->mChannelMap.end())
        {
            it->second->stop();
            AssetManager::GetInstance()->mChannelMap.erase(it);
        }
    }

    void SoundSystem::SetVolume(const std::string& soundName, float volume)
    {
        auto& it = AssetManager::GetInstance()->mChannelMap.find(soundName);
        if (it != AssetManager::GetInstance()->mChannelMap.end())
        {
            it->second->setVolume(volume);
        }
    }

    void SoundSystem::StopAllSounds()
    {
        for (auto& pair : AssetManager::GetInstance()->mChannelMap)
        {
            pair.second->stop();
        }
        AssetManager::GetInstance()->mChannelMap.clear();
    }

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

    void SoundSystem::Update()
    {
        AssetManager::GetInstance()->mSoundSystem->update();

        // Remove stopped channels
        for (auto& it = AssetManager::GetInstance()->mChannelMap.begin(); it != AssetManager::GetInstance()->mChannelMap.end();)
        {
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);
            if (!isPlaying)
            {
                it = AssetManager::GetInstance()->mChannelMap.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void SoundSystem::Exit()
    {
        StopAllSounds();
    }
}
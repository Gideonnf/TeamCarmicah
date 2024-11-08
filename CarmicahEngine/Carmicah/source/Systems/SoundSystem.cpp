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

namespace Carmicah
{
    SoundSystem::SoundSystem()
    {
        for (int i = 0; i < 4; ++i)
        {
            mChannelVolumes[static_cast<SoundChannel>(i)] = 1.0f;
        }
    }

    SoundSystem::~SoundSystem()
    {
        Exit();
    }

    void SoundSystem::Init(bool playDefaultBGM)
    {
        if (playDefaultBGM)
        {
            PlaySound(defaultBGM, SoundChannel::BGM, 0.3f);
        }



        SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
    }

    void SoundSystem::PlaySound(const std::string& soundName, SoundChannel channel, float volume)
    {
        auto it = AssetManager::GetInstance()->mSoundMap.find(soundName);
        if (it != AssetManager::GetInstance()->mSoundMap.end())
        {
            FMOD::Channel* fmodChannel;
            AssetManager::GetInstance()->mSoundSystem->playSound(it->second.sound, nullptr, true, &fmodChannel);

            float actualVolume = (volume >= 0) ? volume : it->second.defaultVolume;
            actualVolume *= mChannelVolumes[channel];

            fmodChannel->setVolume(actualVolume);


            PlayingSound playingSound{ fmodChannel, actualVolume };
            mChannelMap[channel][soundName] = playingSound;
        }
    }

    void SoundSystem::StopSound(const std::string& soundName, SoundChannel channel)
    {
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            auto soundIt = channelIt->second.find(soundName);
            if (soundIt != channelIt->second.end())
            {
                soundIt->second.channel->stop();
                channelIt->second.erase(soundIt);
            }
        }
    }

    void SoundSystem::StopAllSoundsInChannel(SoundChannel channel)
    {
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            for (auto& soundPair : channelIt->second)
            {
                soundPair.second.channel->stop();
            }
            channelIt->second.clear();
        }
    }

    void SoundSystem::StopAllSounds()
    {
        for (auto& channelPair : mChannelMap)
        {
            for (auto& soundPair : channelPair.second)
            {
                soundPair.second.channel->stop();
            }
            channelPair.second.clear();
        }
    }

    void SoundSystem::SetSoundVolume(const std::string& soundName, SoundChannel channel, float volume)
    {
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            auto soundIt = channelIt->second.find(soundName);
            if (soundIt != channelIt->second.end())
            {
                soundIt->second.volume = volume;
                float actualVolume = volume * mChannelVolumes[channel];
                soundIt->second.channel->setVolume(actualVolume);
            }
        }
    }

    void SoundSystem::SetChannelVolume(SoundChannel channel, float volume)
    {
        mChannelVolumes[channel] = volume;
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            for (auto& soundPair : channelIt->second)
            {
                float actualVolume = soundPair.second.volume * volume;
                soundPair.second.channel->setVolume(actualVolume);
            }
        }
    }

    void SoundSystem::PauseResumeSound(const std::string& soundName, SoundChannel channel)
    {
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            auto soundIt = channelIt->second.find(soundName);
            if (soundIt != channelIt->second.end())
            {
                bool paused;
                soundIt->second.channel->getPaused(&paused);
                soundIt->second.channel->setPaused(!paused);
            }
        }
    }

    void SoundSystem::PauseResumeChannel(SoundChannel channel)
    {
        auto channelIt = mChannelMap.find(channel);
        if (channelIt != mChannelMap.end())
        {
            for (auto& soundPair : channelIt->second)
            {
                bool paused;
                soundPair.second.channel->getPaused(&paused);
                soundPair.second.channel->setPaused(!paused);
            }
        }
    }

    void SoundSystem::Update()
    {
        AssetManager::GetInstance()->mSoundSystem->update();

        for (auto& channelPair : mChannelMap)
        {
            for (auto it = channelPair.second.begin(); it != channelPair.second.end();)
            {
                bool isPlaying = false;
                it->second.channel->isPlaying(&isPlaying);
                if (isPlaying)
                {
                    it = channelPair.second.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    void SoundSystem::ReceiveMessage(Message* msg)
    {
        UNUSED(msg);
        //if (msg->mMsgType == MSG_PLAYSFX)
        //{
        //    dynamic_cast<PlaySFXMsg*>(msg)->fileName;
        //}
        //else if (msg->mMsgType == MSG_PLAYBGM)
        //{

        //}
    }

    void SoundSystem::Exit()
    {
        StopAllSounds();
    }
}
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

//namespace Carmicah
//{
//    SoundSystem::SoundSystem() : mSoundSystem(nullptr), mMasterVolume(1.0f) {}
//
//    SoundSystem::~SoundSystem()
//    {
//        Exit();
//    }
//
//    void SoundSystem::Init(bool playDefaultBGM)
//    {
//        FMOD_RESULT result = FMOD::System_Create(&mSoundSystem);
//        if (result != FMOD_OK) {
//            CM_CORE_ERROR("FMOD System creation failed");
//            return;
//        }
//
//        result = mSoundSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
//        if (result != FMOD_OK) {
//            CM_CORE_ERROR("FMOD System initialization failed");
//            return;
//        }
//
//        if (playDefaultBGM) {
//            PlaySound(defaultBGM, 0.5f);
//        }
//    }
//
//    void SoundSystem::Update()
//    {
//        if (mSoundSystem) {
//            mSoundSystem->update();
//
//            // Clean up stopped channels
//            for (auto it = channelMap.begin(); it != channelMap.end();) {
//                bool isPlaying = false;
//                it->second->isPlaying(&isPlaying);
//                if (!isPlaying) {
//                    it = channelMap.erase(it);
//                }
//                else {
//                    ++it;
//                }
//            }
//        }
//    }
//
//    void SoundSystem::Exit()
//    {
//        StopAllSounds();
//
//        if (mSoundSystem) {
//            mSoundSystem->release();
//            mSoundSystem = nullptr;
//        }
//    }
//
//    void SoundSystem::PlaySound(const std::string& soundName, float volume)
//    {
//        auto it = AssetManager::GetInstance()->mSoundMap.find(soundName);
//        if (it != AssetManager::GetInstance()->mSoundMap.end()) {
//            FMOD::Channel* channel;
//            mSoundSystem->playSound(it->second.sound, nullptr, false, &channel);
//            if (channel) {
//                channel->setVolume(volume * mMasterVolume);
//                channelMap[soundName] = channel;
//            }
//        }
//    }
//
//    void SoundSystem::StopSound(const std::string& soundName)
//    {
//        auto it = channelMap.find(soundName);
//        if (it != channelMap.end()) {
//            it->second->stop();
//            channelMap.erase(it);
//        }
//    }
//
//    void SoundSystem::StopAllSounds()
//    {
//        for (auto& [name, channel] : channelMap) {
//            if (channel) {
//                channel->stop();
//            }
//        }
//        channelMap.clear();
//    }
//
//    void SoundSystem::SetMasterVolume(float volume)
//    {
//        mMasterVolume = std::clamp(volume, 0.0f, 1.0f);
//        for (auto& [name, channel] : channelMap) {
//            if (channel) {
//                bool isPlaying;
//                channel->isPlaying(&isPlaying);
//                if (isPlaying) {
//                    float currentVol;
//                    channel->getVolume(&currentVol);
//                    channel->setVolume(currentVol * mMasterVolume);
//                }
//            }
//        }
//    }
//
//    void SoundSystem::PauseSound(const std::string& name)
//    {
//        auto it = channelMap.find(name);
//        if (it != channelMap.end()) {
//            it->second->setPaused(true);
//        }
//    }
//
//    void SoundSystem::ResumeSound(const std::string& name)
//    {
//        auto it = channelMap.find(name);
//        if (it != channelMap.end()) {
//            it->second->setPaused(false);
//        }
//    }
//
//    bool SoundSystem::IsSoundPlaying(const std::string& name)
//    {
//        auto it = channelMap.find(name);
//        if (it != channelMap.end()) {
//            bool isPlaying;
//            it->second->isPlaying(&isPlaying);
//            return isPlaying;
//        }
//        return false;
//    }
//
//    void SoundSystem::ReceiveMessage(Message* msg)
//    {
//        if (msg->mMsgType == MSG_PLAYSFX) {
//            auto* sfxMsg = dynamic_cast<PlaySFXMsg*>(msg);
//            PlaySound(sfxMsg->fileName, 0.5f);
//        }
//        else if (msg->mMsgType == MSG_PLAYBGM) {
//            auto* bgmMsg = dynamic_cast<PlayBGMMsg*>(msg);
//            PlaySound(bgmMsg->fileName, 0.3f);
//        }
//    }
//}


namespace Carmicah
{
    SoundSystem::SoundSystem()
        : mSoundSystem(nullptr)
        , mMasterVolume(1.0f)
        , mIsMuted(false)
    {
        // Initialize category volumes
        mCategoryVolumes[SoundCategory::SFX] = 1.0f;
        mCategoryVolumes[SoundCategory::BGM] = 1.0f;
        mCategoryVolumes[SoundCategory::VOICE] = 1.0f;
        mCategoryVolumes[SoundCategory::AMBIENT] = 1.0f;
        mCategoryVolumes[SoundCategory::UI] = 1.0f;
    }

    SoundSystem::~SoundSystem()
    {
        Exit();
    }

    void SoundSystem::Init(bool playDefaultBGM)
    {
        FMOD_RESULT result = FMOD::System_Create(&mSoundSystem);
        if (result != FMOD_OK) {
            CM_CORE_ERROR("FMOD System creation failed");
            return;
        }

        result = mSoundSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            CM_CORE_ERROR("FMOD System initialization failed");
            return;
        }

        if (playDefaultBGM) {
            PlaySound(defaultBGM, SoundCategory::BGM);
        }
    }

    void SoundSystem::Update()
    {
        if (mSoundSystem) {
            mSoundSystem->update();
            CleanupStoppedSounds();
        }
    }

    void SoundSystem::Exit()
    {
        StopAllSounds();
        mSoundTracks.clear();

        if (mSoundSystem) {
            mSoundSystem->release();
            mSoundSystem = nullptr;
        }
    }

    bool SoundSystem::LoadSound(const std::string& name, const std::string& filePath,
        SoundCategory category, bool isLoop, float defaultVolume)
    {
        if (mSoundTracks.find(name) != mSoundTracks.end()) {
            CM_CORE_WARN("Sound already exists: {}", name);
            return false;
        }

        auto track = std::make_unique<SoundTrack>();
        FMOD_MODE mode = FMOD_DEFAULT;
        if (isLoop) mode |= FMOD_LOOP_NORMAL;

        FMOD_RESULT result = mSoundSystem->createSound(filePath.c_str(), mode, nullptr, &track->sound);
        if (result != FMOD_OK) {
            CM_CORE_ERROR("Failed to load sound: {}", name);
            return false;
        }

        track->name = name;
        track->isLooping = isLoop;
        track->defaultVolume = defaultVolume;
        track->currentVolume = defaultVolume;

        mSoundTracks[name] = std::move(track);
        return true;
    }

    bool SoundSystem::PlaySound(const std::string& soundName, SoundCategory category, float volume)
    {
        auto it = AssetManager::GetInstance()->mSoundMap.find(soundName);
        if (it != AssetManager::GetInstance()->mSoundMap.end()) {
            FMOD::Channel* channel;
            mSoundSystem->playSound(it->second.sound, nullptr, false, &channel);
            if (channel) {
                float finalVolume = volume >= 0 ? volume : it->second.defaultVolume;
                finalVolume *= mCategoryVolumes[category] * mMasterVolume;
                channel->setVolume(finalVolume);

                // Store in tracks for management
                auto track = std::make_unique<SoundTrack>();
                track->sound = it->second.sound;
                track->channel = channel;
                track->currentVolume = finalVolume;
                track->name = soundName;
                mSoundTracks[soundName] = std::move(track);
                return true;
            }
        }
        return false;
    }

    bool SoundSystem::StopSound(const std::string& soundName)
    {
        auto it = mSoundTracks.find(soundName);
        if (it == mSoundTracks.end() || !it->second->channel) return false;

        it->second->channel->stop();
        it->second->channel = nullptr;
        return true;
    }

    void SoundSystem::StopAllSounds()
    {
        for (auto& pair : mSoundTracks) {
            if (pair.second->channel) {
                pair.second->channel->stop();
                pair.second->channel = nullptr;
            }
        }
    }

    bool SoundSystem::PauseSound(const std::string& soundName)
    {
        auto it = mSoundTracks.find(soundName);
        if (it == mSoundTracks.end() || !it->second->channel) return false;

        it->second->isPaused = true;
        it->second->channel->setPaused(true);
        return true;
    }

    bool SoundSystem::ResumeSound(const std::string& soundName)
    {
        auto it = mSoundTracks.find(soundName);
        if (it == mSoundTracks.end() || !it->second->channel) return false;

        it->second->isPaused = false;
        it->second->channel->setPaused(false);
        return true;
    }

    void SoundSystem::SetMasterVolume(float volume)
    {
        mMasterVolume = std::clamp(volume, 0.0f, 1.0f);

        // Update all active sound volumes
        for (auto& pair : mSoundTracks) {
            if (pair.second->channel) {
                UpdateSoundVolume(pair.second.get());
            }
        }
    }

    void SoundSystem::SetCategoryVolume(SoundCategory category, float volume)
    {
        mCategoryVolumes[category] = std::clamp(volume, 0.0f, 1.0f);

        // Update all sounds in this category
        for (auto& pair : mSoundTracks) {
            if (pair.second->channel) {
                UpdateSoundVolume(pair.second.get());
            }
        }
    }

    void SoundSystem::SetSoundVolume(const std::string& soundName, float volume)
    {
        auto it = mSoundTracks.find(soundName);
        if (it == mSoundTracks.end()) return;

        it->second->currentVolume = std::clamp(volume, 0.0f, 1.0f);
        if (it->second->channel) {
            UpdateSoundVolume(it->second.get());
        }
    }

    float SoundSystem::CalculateFinalVolume(const SoundTrack* track, SoundCategory category) const
    {
        if (mIsMuted) return 0.0f;
        return track->currentVolume * mCategoryVolumes.at(category) * mMasterVolume;
    }

    void SoundSystem::UpdateSoundVolume(SoundTrack* track)
    {
        if (!track->channel) return;
        float finalVolume = CalculateFinalVolume(track, SoundCategory::SFX); 
        track->channel->setVolume(finalVolume);
    }

    void SoundSystem::CleanupStoppedSounds()
    {
        for (auto& pair : mSoundTracks) {
            if (pair.second->channel) {
                bool isPlaying = false;
                pair.second->channel->isPlaying(&isPlaying);
                if (!isPlaying) {
                    pair.second->channel = nullptr;
                }
            }
        }
    }

    void SoundSystem::ReceiveMessage(Message* msg)
    {
        if (msg->mMsgType == MSG_PLAYSFX) {
            auto* sfxMsg = dynamic_cast<PlaySFXMsg*>(msg);
            PlaySound(sfxMsg->fileName, SoundCategory::SFX, 0.5f);
        }
        else if (msg->mMsgType == MSG_PLAYBGM) {
            auto* bgmMsg = dynamic_cast<PlayBGMMsg*>(msg);
            PlaySound(bgmMsg->fileName, SoundCategory::BGM, 0.3f);
        }
    }

}
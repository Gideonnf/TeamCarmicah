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

    void SoundSystem::Init()
    {
        mSoundSystem = AssetManager::GetInstance()->mSoundSystem;
    }

    //PlaySound(defaultBGM, SoundCategory::BGM);
    void SoundSystem::Update()
    {
        if (mSoundSystem) {
            mSoundSystem->update();
            CleanupStoppedSounds();

            UpdateFadeEffect();
        }
    }

    void SoundSystem::Exit()
    {
        StopAllSounds();
    }

    bool SoundSystem::PlaySoundThis(const std::string& soundName, SoundCategory category, INTSOUND internalCatergoy, bool isLoop, float volume)
    {
        if (!AssetManager::GetInstance()->AssetExist<FMOD::Sound*>(soundName))
        {
            CM_CORE_WARN("Non-existant Sound Tried playing: " + soundName);
            return false;
        }
        FMOD::Sound* sound = AssetManager::GetInstance()->GetAsset<FMOD::Sound*>(soundName);
        FMOD::Channel* channel;
        mSoundSystem->playSound(sound, nullptr, false, &channel);
        if (channel) {
            // Store in tracks for management
            auto track = std::make_unique<SoundTrack>();
            track->sound = sound;
            track->category = category;
            track->channel = channel;
            track->defaultVolume = (volume >= 0 ? volume : defaultVolume) * mCategoryVolumes[category];
            track->currentVolume = track->defaultVolume * mMasterVolume;

            if (!isLoop)
                channel->setLoopCount(0);
            channel->setVolume(track->currentVolume);
            mSoundTracks[internalCatergoy].emplace_back(std::move(track));
            return true;
        }
        return false;
    }

    void SoundSystem::SwitchSound(INTSOUND internalCatergoy, const std::string& newSoundName, SoundCategory category, bool isLoop, float volume, float fadeDuration)
    {
        if (mSoundTracks[internalCatergoy].empty())
        {
            return;
        }

        FMOD::Channel* currentChannel = mSoundTracks[internalCatergoy].back()->channel;
        if (!currentChannel)
        {
            return;
        }

        FMOD::DSP* volumeDSP = nullptr;

        mSoundSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &volumeDSP);


        if (volumeDSP)
        {

            currentChannel->addDSP(0, volumeDSP);
            volumeDSP->setParameterFloat(FMOD_DSP_FADER_GAIN, 1.0f);

            fadeStartTime = (float)CarmicahTime::GetInstance()->GetDeltaTime();
            fadingOut = true;
            fadeInNewSound = true;
            fadeDurationSeconds = fadeDuration;
            oldChannel = currentChannel;
            newSoundNamePending = newSoundName;
            newSoundCategory = category;
            newSoundInternalCatergoy = internalCatergoy;
            newSoundLoop = isLoop;
            newSoundVolume = volume;
            fadeInProgress = true;
            
        }
    }

    void SoundSystem::UpdateFadeEffect()
    {
        if (!fadeInProgress)
        {
            return;
        }

        float elapsedTime = (float)CarmicahTime::GetInstance()->GetDeltaTime() - fadeStartTime;
        float fadeProgress = elapsedTime / fadeDurationSeconds;

        if (fadingOut)
        {
            float newVolume = 1.0f - fadeProgress;

            oldChannel->setVolume(std::max(0.0f, newVolume));
            if (fadeProgress >= 1.0f)
            {
                oldChannel->stop();
                oldChannel = nullptr;
                fadingOut = false;

                StopSound(newSoundInternalCatergoy);

                PlaySoundThis(newSoundNamePending, newSoundCategory, newSoundInternalCatergoy,newSoundLoop, newSoundVolume);
                FMOD::Channel* newChannel = mSoundTracks[newSoundInternalCatergoy].back()->channel;
                if (newChannel) 
                {
                    newChannel->setVolume(0.0f);
                    newSoundChannel = newChannel;
                    fadeInNewSound = true;
                    fadeStartTime = (float)CarmicahTime::GetInstance()->GetDeltaTime(); // Restart timer
                }
            }
        }
        else if (fadeInNewSound)
        {
            float newVolume = fadeProgress * newSoundVolume;
            newSoundChannel->setVolume(std::min(newSoundVolume, newVolume));

            if (fadeProgress >= 1.0f) 
            {
                fadeInNewSound = false;
                fadeInProgress = false;
            }
        }
    }

    void SoundSystem::StopSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel)
                it->get()->channel->stop();
        }
        mSoundTracks[internalCatergoy].clear();
    }

    void SoundSystem::StopAllSounds()
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
            StopSound(static_cast<INTSOUND>(i));
    }

    void SoundSystem::PauseSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            it->get()->isPaused = true;
            it->get()->channel->setPaused(true);
        }
    }

    void SoundSystem::ResumeSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            it->get()->isPaused = false;
            it->get()->channel->setPaused(false);
        }
    }

    void SoundSystem::PauseAllSounds() 
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
            PauseSound(static_cast<INTSOUND>(i));
    }

    void SoundSystem::ResumeAllSounds() 
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
            ResumeSound(static_cast<INTSOUND>(i));
    }


    void SoundSystem::SetMasterVolume(float volume)
    {
        mMasterVolume = std::clamp(volume, 0.0f, 1.0f);

        // Update all active sound volumes
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
        {
            for (auto it = mSoundTracks[i].begin(); it != mSoundTracks[i].end(); ++it)
            {
                UpdateSoundVolume(it->get());
            }
        }
    }

    void SoundSystem::SetCategoryVolume(SoundCategory category, INTSOUND internalCatergoy, float volume)
    {
        mCategoryVolumes[category] = std::clamp(volume, 0.0f, 1.0f);

        // Update all sounds in this category
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel && it->get()->category == category)
                UpdateSoundVolume(it->get());
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
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
        {
            for (auto it = mSoundTracks[i].begin(); it != mSoundTracks[i].end();)
            {
                if (it->get()->channel)
                {
                    bool isPlaying = false;
                    it->get()->channel->isPlaying(&isPlaying);
                    if (!isPlaying)
                    {
                        it->get()->channel = nullptr;
                        it = mSoundTracks[i].erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
    }

    void SoundSystem::ReceiveMessage(Message* msg)
    {
        if (msg->mMsgType == MSG_PLAYSFX) {
            auto* sfxMsg = dynamic_cast<PlaySFXMsg*>(msg);
            PlaySoundThis(sfxMsg->fileName, SoundCategory::SFX, INTSOUND::SOUND_INGAME, false, 0.5f);
        }
        else if (msg->mMsgType == MSG_PLAYBGM) {
            auto* bgmMsg = dynamic_cast<PlayBGMMsg*>(msg);
            PlaySoundThis(bgmMsg->fileName, SoundCategory::BGM, INTSOUND::SOUND_BGM, true, 0.3f);
        }
    }

}

/*
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

    track->isLooping = isLoop;
    track->defaultVolume = defaultVolume;
    track->currentVolume = defaultVolume;

    mSoundTracks[name] = std::move(track);
    return true;
}
*/
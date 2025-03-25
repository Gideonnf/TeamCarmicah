/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			SoundSystem.h

 author:		YANG YUJIE(30%)
 co-author(s):	Won Yu Xuan Rainne(30%)
                Lee Yong Yee(40%)

 email:			won.m@digipen.edu
                y.yujie@digipen.edu
                l.yongyee@digipen.edu

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
        mCategoryVolumes[SoundCategory::EDITOR] = 1.0f;
    }

    /**
     * @brief Initializes the SoundSystem by retrieving the SoundSystem instance from the AssetManager.
     *
     * This function sets up the SoundSystem by accessing the `mSoundSystem` from the `AssetManager`
     * and assigning it to the member variable `mSoundSystem`.
     *
     * @return void
     */
    void SoundSystem::Init()
    {
        mSoundSystem = AssetManager::GetInstance()->mSoundSystem;
    }

    /**
     * @brief Updates the sound system by processing any active sounds and managing cleanup or fade effects.
     *
     * This function calls the `update()` method of the SoundSystem to process any ongoing sounds.
     * It also manages cleanup of stopped sounds and updates any fade effects that may be in progress.
     *
     * @return void
     */
    void SoundSystem::Update()
    {
        if (mSoundSystem) {
            mSoundSystem->update();
            CleanupStoppedSounds();

            UpdateFadeEffect();
        }
    }

    /**
     * @brief Stops all sounds and performs necessary cleanup before shutting down the sound system.
     *
     * This function stops all currently playing sounds by calling `StopAllSounds()`, ensuring that
     * all active sounds are properly stopped and cleaned up before the sound system is shut down.
     *
     * @return void
     */
    void SoundSystem::Exit()
    {
        StopAllSounds();
    }

    /**
     * @brief Plays a sound with the specified parameters and stores it for future management.
     *
     * This function attempts to play a sound from the asset manager, with the specified properties.
     * If the sound exists, it is played and associated with various settings like volume, category, and low-pass filtering.
     * The sound is then stored in the `mSoundTracks` list for future management.
     *
     * @param soundName The name of the sound to be played.
     * @param category The category of the sound (e.g., BGM, SFX).
     * @param internalCatergoy Internal categorization used for tracking the sound.
     * @param isLoop Boolean flag indicating if the sound should loop.
     * @param volume The volume at which the sound should be played.
     * @param id The ID of the entity that the sound is associated with (e.g., an entity in a game world).
     *
     * @return True if the sound was successfully played and added to the tracking list, false otherwise.
     */
    bool SoundSystem::PlaySoundThis(const std::string& soundName, SoundCategory category, INTSOUND internalCatergoy, bool isLoop, float volume, unsigned int id)
    {
        if (!AssetManager::GetInstance()->AssetExist<FMOD::Sound*>(soundName))
        {
            CM_CORE_WARN("Non-existant Sound Tried playing: " + soundName);
            return false;
        }
        FMOD::Sound* sound = AssetManager::GetInstance()->GetAsset<FMOD::Sound*>(soundName);

        FMOD::Channel* channel;
        mSoundSystem->playSound(sound, nullptr, false, &channel);

        FMOD::DSP* lowPass;

        if (channel) {
            // Store in tracks for management
            auto track = std::make_unique<SoundTrack>();
            track->sound = sound;
            track->category = category;
            track->channel = channel;
            track->defaultVolume = (volume >= 0 ? volume : defaultVolume) * mCategoryVolumes[category];
            track->currentVolume = track->defaultVolume * mMasterVolume;
            mSoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &lowPass);
            track->channel->addDSP(0, lowPass);
            track->lowPassDSP = lowPass;
            track->lowPassDSP->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 9000.0f);
            track->lowPassDSP->setBypass(true);
            track->muffle = false;
            track->entityID = id;

            if (!isLoop)
            {
                channel->setLoopCount(0);

            }
            /*else 
            {
                channel->setLoopCount(-1);
            }*/
            channel->setVolume(track->currentVolume);
            mSoundTracks[internalCatergoy].emplace_back(std::move(track));
            return true;
        }
        return false;
    }

    /**
     * @brief Switches the currently playing sound to a new sound, optionally applying a fade effect.
     *
     * This function initiates a fade-out for the current sound, and once the fade-out is complete,
     * it either fades in the new sound or immediately plays the new sound, based on the fade settings.
     *
     * @param internalCatergoy The internal category used to track the sound.
     * @param newSoundName The name of the new sound to be played.
     * @param category The category of the new sound (e.g., BGM, SFX).
     * @param isLoop Boolean flag indicating if the new sound should loop.
     * @param volume The volume at which the new sound should be played.
     * @param fadeTimer The time duration in seconds for the fade-out effect.
     * @param fadeDuration The time duration in seconds for the fade-in effect.
     * @param fadeInNext Boolean flag indicating if the next sound should fade in after the current sound fades out.
     *
     * @return void
     */
    void SoundSystem::SwitchSound(INTSOUND internalCatergoy, const std::string& newSoundName, SoundCategory category, bool isLoop, float volume, float fadeTimer, float fadeDuration, bool fadeInNext)
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

        float currentVol = 1.0f;
        currentChannel->getVolume(&currentVol);
        currentVolume = currentVol; // Store for fade-out

        FMOD::DSP* volumeDSP = nullptr;

        mSoundSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &volumeDSP);


        if (volumeDSP)
        {

            currentChannel->addDSP(0, volumeDSP);
            volumeDSP->setParameterFloat(FMOD_DSP_FADER_GAIN, 1.0f);

            fadeTimerSeconds = fadeTimer;
            fadeDurationSeconds = fadeDuration;
            fadingOut = true;
            fadeOutDSP = volumeDSP;
            oldChannel = currentChannel;
            switchBGM = true;
            newSoundNamePending = newSoundName;
            newSoundCategory = category;
            newSoundInternalCatergoy = internalCatergoy;
            newSoundLoop = isLoop;
            newSoundVolume = volume;
            fadingInNewSound = false;
            fadeInNextValue = fadeInNext;
            
            
        }
    }

    /**
     * @brief Updates the fade effect for currently playing sounds, handling fade-in and fade-out progress.
     *
     * This function is responsible for updating the fade effect, reducing the volume for fade-out and
     * increasing the volume for fade-in, based on the remaining fade timer.
     *
     * @return void
     */
    void SoundSystem::UpdateFadeEffect()
    {
        if (fadeTimerSeconds <= 0.0f)
        {
            return;
        }

        // Decrease fadeTimer
        fadeTimerSeconds -= (float)CarmicahTime::GetInstance()->GetDeltaTime();

        if (fadingOut && fadeOutDSP)
        {
            // Fade-out progress: Starts at 1.0 and decreases to 0.0
            float fadeOutProgress = 1.0f - (fadeTimerSeconds / fadeDurationSeconds);
            float newVolume = currentVolume * (1.0f - fadeOutProgress);
            oldChannel->setVolume(std::max(0.0f, newVolume));
            //fadeOutDSP->setParameterFloat(FMOD_DSP_FADER_GAIN, std::max(0.0f, newVolume));

            if (fadeTimerSeconds <= 0.0f)
            {
                /*oldChannel->stop();
                oldChannel = nullptr;*/
                fadingOut = false;

                //fadeOutDSP->remove();

                StopSound(newSoundInternalCatergoy);

                if (fadeInNextValue)
                {
                    PlaySoundThis(newSoundNamePending, newSoundCategory, newSoundInternalCatergoy, newSoundLoop, 0.0f);
                    FMOD::Channel* newChannel = mSoundTracks[newSoundInternalCatergoy].back()->channel;
                    if (newChannel)
                    {
                        newChannel->setVolume(0.0f); // Start at 0 volume
                        newSoundChannel = newChannel;
                        fadingInNewSound = true;
                        fadeTimerSeconds = 1.0f; // Restart timer for fade-in
                    }

                }
                else
                {
                    PlaySoundThis(newSoundNamePending, newSoundCategory, newSoundInternalCatergoy, newSoundLoop, newSoundVolume);
                    fadeTimerSeconds = 0.0f;
                }
            }
        }
        else if (fadingInNewSound)
        {
            // Fade-in progress: Starts at 0.0 and increases to 1.0
            float fadeInProgress = 1.0f - (fadeTimerSeconds / fadeDurationSeconds);
            float newVolume = fadeInProgress * newSoundVolume;
            newSoundChannel->setVolume(std::min(newSoundVolume, newVolume) * mCategoryVolumes[newSoundCategory]);

            if (fadeTimerSeconds <= 0.0f)
            {
                fadingInNewSound = false;
                //fadeInProgress = false;
                fadeTimerSeconds = 0.0f; // Reset timer
            }
        }
    }

    /**
     * @brief Stops the sound associated with the given internal category, applying a fade-out effect.
     *
     * This function initiates a fade-out effect on the currently playing sound, reducing its volume to zero
     * over the specified fade duration. The sound is stopped once the fade-out is complete.
     *
     * @param internalCatergoy The internal category used to track the sound to be stopped.
     * @param fadeTimer The time duration in seconds for the fade-out effect.
     * @param fadeDuration The time duration in seconds for the fade-out.
     *
     * @return void
     */
    void SoundSystem::StopSoundWithFade(INTSOUND internalCatergoy, float fadeTimer, float fadeDuration)
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

        float currentVol = 1.0f;
        currentChannel->getVolume(&currentVol);
        currentVolume = currentVol; // Store for fade-out

        FMOD::DSP* volumeDSP = nullptr;

        mSoundSystem->createDSPByType(FMOD_DSP_TYPE_FADER, &volumeDSP);


        if (volumeDSP)
        {

            currentChannel->addDSP(0, volumeDSP);
            volumeDSP->setParameterFloat(FMOD_DSP_FADER_GAIN, 1.0f);

            fadeTimerSeconds = fadeTimer;
            fadeDurationSeconds = fadeDuration;
            fadingOut = true;
            //fadeInNewSound = false;
            oldChannel = currentChannel;
            fadeOutDSP = volumeDSP;
            switchBGM = false;
            newSoundInternalCatergoy = internalCatergoy;

        }
    }

    /**
     * @brief Stops the sound associated with the given internal category immediately.
     *
     * This function stops the sound immediately without any fade-out effect. All sound tracks associated
     * with the given internal category are cleared from the sound system.
     *
     * @param internalCatergoy The internal category used to track the sound to be stopped.
     *
     * @return void
     */
    void SoundSystem::StopSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel)
            {
                it->get()->channel->stop();

              
            }
                //it->get()->channel->setMode(FMOD_LOOP_OFF);
            
        }

        
        mSoundTracks[internalCatergoy].clear();
    }

    /**
     * @brief Stops the sound effect associated with the given internal category.
     *
     * This function stops the first sound in the specified sound category (SFX) if it is currently playing.
     * It only stops the first sound track in the list.
     *
     * @param internalCatergoy The internal category used to track the sound effect to be stopped.
     *
     * @return void
     */
    void SoundSystem::StopSoundSFX(INTSOUND internalCatergoy, unsigned int id)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel)
            {
                if (id == 0 || id == it->get()->entityID)
                {

                    it->get()->channel->stop();
                    break;
                }
            }
            
        }
    }

    /*void SoundSystem::StopAllSoundSFX(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel)
            {
                it->get()->channel->stop();
                
            }
        }

        mSoundTracks[internalCatergoy].clear();
    }*/
        
    /**
     * @brief Stops all currently playing sounds.
     *
     * This function stops all sound effects and background music by iterating over all sound categories
     * and stopping the sounds in each category.
     *
     * @return void
     */
    void SoundSystem::StopAllSounds()
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
        {
            
            StopSound(static_cast<INTSOUND>(i));
        }

        
    }

    /**
     * @brief Pauses the sound effect associated with the given internal category.
     *
     * This function pauses all sounds in the specified category. The paused status is applied to all
     * sounds within the category.
     *
     * @param internalCatergoy The internal category used to track the sound effect to be paused.
     *
     * @return void
     */
    void SoundSystem::PauseSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            it->get()->isPaused = true;
            it->get()->channel->setPaused(true);
        }
    }

    /**
     * @brief Resumes the sound effect associated with the given internal category.
     *
     * This function resumes all paused sounds in the specified category.
     *
     * @param internalCatergoy The internal category used to track the sound effect to be resumed.
     *
     * @return void
     */
    void SoundSystem::ResumeSound(INTSOUND internalCatergoy)
    {
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            it->get()->isPaused = false;
            it->get()->channel->setPaused(false);
        }
    }

    /**
     * @brief Pauses all currently playing sounds across all categories.
     *
     * This function pauses all sound effects and background music across all categories.
     *
     * @return void
     */
    void SoundSystem::PauseAllSounds() 
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
            PauseSound(static_cast<INTSOUND>(i));
    }

    /**
     * @brief Resumes all currently paused sounds across all categories.
     *
     * This function resumes all paused sound effects and background music across all categories.
     *
     * @return void
     */
    void SoundSystem::ResumeAllSounds() 
    {
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
            ResumeSound(static_cast<INTSOUND>(i));
    }

    /**
     * @brief Sets the master volume for all sounds.
     *
     * This function sets the master volume for all sounds in the system. The volume is clamped between 0.0f and 1.0f.
     * It updates the volume of all active sound tracks accordingly.
     *
     * @param volume The new master volume value, clamped between 0.0f and 1.0f.
     *
     * @return void
     */
    void SoundSystem::SetMasterVolume(float volume)
    {
        mMasterVolume = std::clamp(volume, 0.0f, 1.0f);

        // Update all active sound volumes
        for (int i{}; i < INTSOUND::SOUND_MAX_SOUNDS; ++i)
        {
            for (auto it = mSoundTracks[i].begin(); it != mSoundTracks[i].end(); ++it)
            {
                UpdateSoundVolume(it->get(), SoundCategory::SFX);
            }
        }
    }

    /**
     * @brief Sets the volume for sounds in a specific category.
     *
     * This function sets the volume for all sounds in the specified category. The volume is clamped between 0.0f and 1.0f.
     * It updates the volume of all sound tracks in the given category.
     *
     * @param category The category of sounds whose volume is to be set.
     * @param internalCatergoy The internal category used to track the sounds.
     * @param volume The new volume for the category, clamped between 0.0f and 1.0f.
     *
     * @return void
     */
    void SoundSystem::SetCategoryVolume(SoundCategory category, INTSOUND internalCatergoy, float volume)
    {
        mCategoryVolumes[category] = std::clamp(volume, 0.0f, 1.0f);

        // Update all sounds in this category
        for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
        {
            if (it->get()->channel && it->get()->category == category)
                UpdateSoundVolume(it->get(), category);
        }
    }

    float SoundSystem::GetCategoryVolume(SoundCategory category)
    {
        return mCategoryVolumes[category];
    }

    /**
     * @brief Toggles the muffle effect on a specific sound or group of sounds.
     *
     * This function applies a muffle effect to the sound associated with the given internal category. If `toMuffle` is true,
     * it applies a low-pass filter effect to the sound, reducing high frequencies. It also adjusts the volume to 50% of the original volume.
     *
     * @param internalCatergoy The internal category used to track the sound.
     * @param toMuffle Boolean flag indicating whether to apply the muffle effect or remove it.
     * @param id The ID of the specific sound track to muffle (optional, used for specific sound IDs).
     *
     * @return void
     */
    void SoundSystem::ToggleMuffle(INTSOUND internalCatergoy, bool toMuffle, unsigned int id)
    {
        FMOD::Channel* currentChannel = nullptr;
        //FMOD::DSP* dsp = nullptr;
        if (id == 0 && internalCatergoy == SOUND_BGM)
        {
            currentChannel = mSoundTracks[internalCatergoy].back()->channel;
        }
        else
        {
            for (auto it = mSoundTracks[internalCatergoy].begin(); it != mSoundTracks[internalCatergoy].end(); ++it)
            {
                if (id == it->get()->entityID)
                {
                    currentChannel = it->get()->channel;
                    if (toMuffle == false)
                    {
                        it->get()->channel->setVolume((it->get()->currentVolume)*0.5f);
                    }
                    else
                    {
                        it->get()->channel->setVolume(it->get()->currentVolume);
                    }

                    it->get()->muffle = toMuffle;
                    it->get()->lowPassDSP->setBypass(it->get()->muffle);
                }
            }

        }

    }

    /**
     * @brief Calculates the final volume for a given sound track based on various factors.
     *
     * This function computes the final volume of a sound track by factoring in whether the system is muted, the track's current volume,
     * the category-specific volume, and the master volume. The final volume is returned as a float.
     *
     * @param track The sound track for which the final volume is to be calculated.
     * @param category The category of the sound (e.g., SFX, BGM) to adjust the volume accordingly.
     *
     * @return The final calculated volume of the sound track.
     */
    float SoundSystem::CalculateFinalVolume(const SoundTrack* track, SoundCategory category) const
    {
        if (mIsMuted) return 0.0f;
        return track->currentVolume * mCategoryVolumes.at(category) * mMasterVolume;
    }

    /**
     * @brief Updates the volume of a sound track based on the final calculated volume.
     *
     * This function updates the volume of the specified sound track's channel by calculating the final volume using
     * `CalculateFinalVolume` and then applying that volume to the track's channel.
     *
     * @param track The sound track whose volume is to be updated.
     *
     * @param category The Category to update
     * 
     * @return void
     */
    void SoundSystem::UpdateSoundVolume(SoundTrack* track, SoundCategory category)
    {
        if (!track->channel) return;
        float finalVolume = CalculateFinalVolume(track, category);
        track->channel->setVolume(finalVolume);
    }

    /**
     * @brief Cleans up stopped sounds by removing tracks that are no longer playing.
     *
     * This function iterates over all sound tracks and removes any track that has finished playing. It ensures that the system
     * only retains active sound tracks, cleaning up resources from tracks that are no longer in use.
     *
     * @return void
     */
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

    /**
     * @brief Receives and processes incoming messages related to sound playback.
     *
     * This function processes messages that instruct the sound system to play a sound effect (SFX) or background music (BGM).
     * The function will trigger the appropriate `PlaySoundThis` function based on the message type (`MSG_PLAYSFX` or `MSG_PLAYBGM`).
     *
     * @param msg The message to be processed, which contains information about the sound to be played.
     *
     * @return void
     */
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
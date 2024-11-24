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
#include <memory>
#include <unordered_map>


//breif:         This class is a system that handles the sound effects and background music of the game


namespace Carmicah
{
    // Track information for each sound
    struct SoundTrack {
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;
        float defaultVolume = 1.0f;
        float currentVolume = 1.0f;
        bool isLooping = false;
        bool isPaused = false;
        std::string name;
    };

    // Categories for different types of sounds
    enum class SoundCategory {
        SFX,        // Sound effects
        BGM,        // Background music
        VOICE,      // Voice/dialogue
        AMBIENT,    // Ambient sounds
        UI         // UI sounds
    };

    class SoundSystem : public BaseSystem
    {
    private:
        FMOD::System* mSoundSystem;
        std::unordered_map<std::string, std::unique_ptr<SoundTrack>> mSoundTracks;
        std::unordered_map<SoundCategory, float> mCategoryVolumes;
        float mMasterVolume;
        const int MAX_CHANNELS = 256;
        bool mIsMuted;

    public:
        SoundSystem();
        ~SoundSystem();

        void Init(bool playDefaultBGM = true);
        void Update();
        void Exit();

        // Enhanced playback controls
        bool PlaySound(const std::string& soundName, SoundCategory category = SoundCategory::SFX, float volume = -1.0f);
        bool StopSound(const std::string& soundName);
        bool PauseSound(const std::string& soundName);
        bool ResumeSound(const std::string& soundName);
        void StopAllSounds();
        void PauseAllSounds();
        void ResumeAllSounds();

        // Volume controls
        void SetMasterVolume(float volume);
        void SetCategoryVolume(SoundCategory category, float volume);
        void SetSoundVolume(const std::string& soundName, float volume);
        float GetSoundVolume(const std::string& soundName) const;
        float GetCategoryVolume(SoundCategory category) const;

        // Sound state queries
        bool IsSoundPlaying(const std::string& soundName) const;
        bool IsSoundPaused(const std::string& soundName) const;
        bool IsSoundLooping(const std::string& soundName) const;

        // Muting controls
        void MuteAll(bool mute);
        void MuteCategory(SoundCategory category, bool mute);
        void MuteSound(const std::string& soundName, bool mute);

        // Load and unload sounds
        bool LoadSound(const std::string& name, const std::string& filePath,
            SoundCategory category = SoundCategory::SFX, bool isLoop = false, float defaultVolume = 1.0f);
        void UnloadSound(const std::string& name);

        void ReceiveMessage(Message* msg) override;

        // Default load sound 
        static constexpr const char* defaultBGM{ "BGM_MainMenu_Mix1" };
        static constexpr const char* defaultSFX{ "buttonclick" };

    private:
        void UpdateSoundVolume(SoundTrack* track);
        float CalculateFinalVolume(const SoundTrack* track, SoundCategory category) const;
        void CleanupStoppedSounds();
    };
}

#endif
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
#include <vector>
#include <unordered_map>


//breif:         This class is a system that handles the sound effects and background music of the game


namespace Carmicah
{
    // Categories for different types of sounds
    enum class SoundCategory {
        SFX,        // Sound effects
        BGM,        // Background music
        VOICE,      // Voice/dialogue
        AMBIENT,    // Ambient sounds
        UI         // UI sounds
    };

    // Track information for each sound
    struct SoundTrack {
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;
        float defaultVolume = 1.0f;
        float currentVolume = 1.0f;
        SoundCategory category = SoundCategory::SFX;
        bool isLooping = false;
        bool isPaused = false;
    };


    class SoundSystem : public BaseSystem
    {
    public:
        enum INTSOUND
        {
            SOUND_INGAME,
            SOUND_INMENU,
            SOUND_BGM,
            SOUND_MAX_SOUNDS
        };

    private:
        FMOD::System* mSoundSystem; // Just a copy, not handled here
        std::vector<std::unique_ptr<SoundTrack>> mSoundTracks[SOUND_MAX_SOUNDS];
        std::unordered_map<SoundCategory, float> mCategoryVolumes;
        const float defaultVolume = 1.0f;
        float mMasterVolume;
        bool mIsMuted;

    public:
        SoundSystem();

        void Init();
        void Update();
        void Exit();

        // Enhanced playback controls
        bool PlaySoundThis(const std::string& soundName, SoundCategory category = SoundCategory::SFX, INTSOUND internalCatergoy = SOUND_INGAME, float volume = -1.0f);
        void StopSound(INTSOUND internalCatergoy);
        void PauseSound(INTSOUND internalCatergoy);
        void ResumeSound(INTSOUND internalCatergoy);
        void StopAllSounds();
        void PauseAllSounds();
        void ResumeAllSounds();

        // Volume controls
        void SetMasterVolume(float volume);
        void SetCategoryVolume(SoundCategory category, INTSOUND internalCatergoy, float volume);
        float GetCategoryVolume(SoundCategory category) const;

        // Muting controls
        void MuteAll(bool mute);
        void MuteCategory(SoundCategory category, INTSOUND internalCatergoy, bool mute);
        void MuteSound(INTSOUND internalCatergoy, bool mute);

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
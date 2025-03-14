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
#include "../CarmicahTime.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../Components/Sound.h"


//breif:         This class is a system that handles the sound effects and background music of the game


namespace Carmicah
{
    // Categories for different types of sounds
    enum class SoundCategory {
        SFX,        // Sound effects
        BGM,        // Background music
        VOICE,      // Voice/dialogue
        AMBIENT,    // Ambient sounds
        UI,         // UI sounds
        EDITOR      // For Editor Use
    };

    // Track information for each sound
    struct SoundTrack {
        FMOD::Sound* sound = nullptr;
        FMOD::Channel* channel = nullptr;
        FMOD::DSP* lowPassDSP = nullptr;
        FMOD::DSP* lowPassResonanceDSP = nullptr;
        float defaultVolume = 1.0f;
        float currentVolume = 1.0f;
        SoundCategory category = SoundCategory::SFX;
        bool isLooping = false;
        bool isPaused = false;
        bool muffle = true;
        unsigned int entityID = 0;
    };

    class SoundSystem : public BaseSystem
    {
    public:
        enum INTSOUND
        {
            SOUND_INGAME,
            SOUND_INMENU,
            SOUND_BGM,
            SOUND_EDITOR,
            SOUND_MAX_SOUNDS
        };

    private:
        FMOD::System* mSoundSystem; // Just a copy, not handled here
        std::vector<std::unique_ptr<SoundTrack>> mSoundTracks[SOUND_MAX_SOUNDS];
        std::unordered_map<SoundCategory, float> mCategoryVolumes;
        const float defaultVolume = 1.0f;
        float mMasterVolume;
        bool mIsMuted;
        float currentVolume;
        FMOD::DSP* fadeOutDSP;
        bool fadingOut = false;
        bool fadeInNextValue = false;
        bool fadingInNewSound = false;
        
        float fadeTimerSeconds = 0.0f; 
        float fadeDurationSeconds = 0.0f;
        FMOD::Channel* oldChannel = nullptr; 
        FMOD::Channel* newSoundChannel = nullptr; 
        std::string newSoundNamePending; 
        SoundCategory newSoundCategory;
        INTSOUND newSoundInternalCatergoy;
        bool newSoundLoop = false;
        bool switchBGM = false;
        float newSoundVolume = 0.0f;

    public:
        SoundSystem();

        void Init();
        void Update();
        void Exit();

        // Enhanced playback controls
        bool PlaySoundThis(const std::string& soundName, SoundCategory category = SoundCategory::SFX, INTSOUND internalCatergoy = SOUND_INGAME, bool isLoop = false, float volume = -1.0f, unsigned int id = 0);
        void SwitchSound(INTSOUND internalCatergoy, const std::string& newSoundName, SoundCategory category, bool isLoop, float volume, float fadeTimer, float fadeDuration, bool fadeOut);
        void UpdateFadeEffect();
        //void StopSound(INTSOUND internalCatergoy);
        void StopSoundSFX(INTSOUND internalCatergoy);
        void StopSound(INTSOUND internalCatergoy);
        void StopSoundWithFade(INTSOUND internalCatergoy, float fadeTimer, float fadeDuration);
        void PauseSound(INTSOUND internalCatergoy);
        void ResumeSound(INTSOUND internalCatergoy);
        void StopAllSounds();
        void StopAllSoundSFX(INTSOUND internalCatergoy);
        void PauseAllSounds();
        void ResumeAllSounds();

        // Volume controls
        void SetMasterVolume(float volume);
        void SetCategoryVolume(SoundCategory category, INTSOUND internalCatergoy, float volume);
        float GetCategoryVolume(SoundCategory category) const;

        // Soundeffect controls
        void ToggleMuffle(INTSOUND internalCatergoy, bool toMuffle, unsigned int id);

        // Muting controls
        void MuteAll(bool mute);
        void MuteCategory(SoundCategory category, INTSOUND internalCatergoy, bool mute);
        void MuteSound(INTSOUND internalCatergoy, bool mute);

        void ReceiveMessage(Message* msg) override;

        // Default load sound 
        //static constexpr const char* defaultBGM{ "BGM_SetupPhase_Mix1" };
        static constexpr const char* defaultSFX{ "pop" };

    private:
        void UpdateSoundVolume(SoundTrack* track, SoundCategory category);
        float CalculateFinalVolume(const SoundTrack* track, SoundCategory category) const;
        void CleanupStoppedSounds();
    };
}

#endif
#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "FMOD/fmod.hpp"
#include <unordered_map>
#include <string>

//namespace Carmicah
//{
//	class SoundSystem : public BaseSystem
//	{
//	private:
//		std::vector<FMOD::Channel*> sfxList;
//		const char* defaultBGM{ "bouken" };
//
//	public:
//		void Init(bool play);
//
//		void Update();
//
//		void PlayAudio(const std::string& sound, bool isBgm);
//	};
//}
//#endif


namespace Carmicah
{
    class SoundSystem : public BaseSystem
    {
    private:
        std::unordered_map<std::string, FMOD::Sound*> soundMap;
        std::unordered_map<std::string, FMOD::Channel*> channelMap;
        FMOD::System* fmodSystem;

    public:
        SoundSystem();
        ~SoundSystem();

        void Init(bool playDefaultBGM = true);
        void Update();
        void Exit();

        void LoadSound(const std::string& soundName, const std::string& filePath, bool isLooping = false);
        void PlaySound(const std::string& soundName, float volume = 1.0f);
        void StopSound(const std::string& soundName);
        void SetVolume(const std::string& soundName, float volume);
        void StopAllSounds();

        const char* defaultBGM{ "bouken" };
    };
}
#endif
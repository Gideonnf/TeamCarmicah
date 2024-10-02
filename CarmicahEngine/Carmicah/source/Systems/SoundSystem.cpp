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

    void SoundSystem::Init(bool playDefaultBGM)
    {
        FMOD::System_Create(&fmodSystem);
        fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);

        // Load default BGM
        LoadSound(defaultBGM, "../Assets/BGM/bouken.mp3", true);

        if (playDefaultBGM)
        {
            PlaySound(defaultBGM, 0.5f);
        }

        SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
    }

    void SoundSystem::LoadSound(const std::string& soundName, const std::string& filePath, bool isLooping)
    {
        FMOD::Sound* sound;
        FMOD_MODE mode = isLooping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
        fmodSystem->createSound(filePath.c_str(), mode, nullptr, &sound);
        soundMap[soundName] = sound;
    }

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

    void SoundSystem::StopSound(const std::string& soundName)
    {
        auto it = channelMap.find(soundName);
        if (it != channelMap.end())
        {
            it->second->stop();
            channelMap.erase(it);
        }
    }

    void SoundSystem::SetVolume(const std::string& soundName, float volume)
    {
        auto it = channelMap.find(soundName);
        if (it != channelMap.end())
        {
            it->second->setVolume(volume);
        }
    }

    void SoundSystem::StopAllSounds()
    {
        for (auto& pair : channelMap)
        {
            pair.second->stop();
        }
        channelMap.clear();
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
        fmodSystem->update();

        // Remove stopped channels
        for (auto it = channelMap.begin(); it != channelMap.end();)
        {
            bool isPlaying = false;
            it->second->isPlaying(&isPlaying);
            if (!isPlaying)
            {
                it = channelMap.erase(it);
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

        for (auto& pair : soundMap)
        {
            pair.second->release();
        }
        soundMap.clear();

        if (fmodSystem)
        {
            fmodSystem->release();
            fmodSystem = nullptr;
        }
    }
}
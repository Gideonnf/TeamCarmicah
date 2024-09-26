#include "pch.h"
#include "Systems/SoundSystem.h"
#include <ECS/ECSTypes.h>
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace Carmicah
{
	void SoundSystem::Init(bool play)
	{
		// Set the signature of the system
		//mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);
		if(play)
			PlayAudio(defaultBGM, true);
	}

	void SoundSystem::PlayAudio(const std::string& sound, bool isBgm)
	{
		FMOD::Channel* channel = NULL;
		AssetManager::GetInstance()->soundSystem->playSound(AssetManager::GetInstance()->soundMap[sound].sound, NULL, false, &channel);
		sfxList.push_back(channel);
	}

	void SoundSystem::Update()
	{
		for (auto& it = sfxList.begin(); it != sfxList.end();)
		{
			// If channel stops playing, remove from list
			bool bIsPlaying = false;
			(*it)->isPlaying(&bIsPlaying);
			if (!bIsPlaying)
			{
				it = sfxList.erase(it);
			}
			else
				++it;
		}
		AssetManager::GetInstance()->soundSystem->update();
	}
}
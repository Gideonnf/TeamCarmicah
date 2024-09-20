#include "pch.h"
#include "Systems/SoundSystem.h"
#include <ECS/ECSTypes.h>
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

namespace Carmicah
{
	void SoundSystem::Init(bool playBgm)
	{
		// Set the signature of the system
		//mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<SoundSystem>(mSignature);

		if (FMOD::System_Create(&mpSystem) != FMOD_OK)
			return;
		mpSystem->init(32, FMOD_INIT_NORMAL, NULL);
		FMOD::Sound* sound = nullptr;
		FMOD::Channel* channel = NULL;
		if (mpSystem->createSound("../Assets/Audio/bouken.mp3", FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
			return;
		sound->setMode(FMOD_LOOP_OFF);
		if(playBgm)
			mpSystem->playSound(sound, NULL, false, &channel);
	}

	void SoundSystem::Update()
	{
		mpSystem->update();
	}

	void SoundSystem::Exit()
	{
		sound->release();
		if (mpSystem != NULL)
			mpSystem->release();
	}
}
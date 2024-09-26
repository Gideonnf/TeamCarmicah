#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "FMOD/fmod.hpp"

namespace Carmicah
{
	class SoundSystem : public BaseSystem
	{
	private:
		std::vector<FMOD::Channel*> sfxList;
		const char* defaultBGM{ "bouken" };

	public:
		void Init(bool play);

		void Update();

		void PlayAudio(const std::string& sound, bool isBgm);
	};
}
#endif
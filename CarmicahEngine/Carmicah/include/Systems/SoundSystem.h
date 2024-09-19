#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "ECS/BaseSystem.h"
#include <FMOD/fmod.hpp>

namespace Carmicah
{
	class SoundSystem : public BaseSystem
	{
	private:
		FMOD::System* mpSystem;
		FMOD::Sound* sound;


	public:
		void Init();

		void Update();

		void Exit();
	};
}
#endif
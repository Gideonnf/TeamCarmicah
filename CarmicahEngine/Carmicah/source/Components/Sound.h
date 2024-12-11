#ifndef SOUND_H
#define SOUND_H
#include "BaseComponent.h"
#include <FMOD/fmod.hpp>
#include <string>
namespace Carmicah
{
	struct Sound : BaseComponent<Sound>
	{
		enum SoundState
		{
			IS_IDLE,
			IS_PLAYING,
			IS_PAUSED
		};
		std::string soundName;
		bool isLooping;
		float vol;
		float pitch;
		SoundState soundState;
		FMOD::Channel* soundChannel;
		FMOD::Sound* sound;


		Sound& DeserializeComponent(const rapidjson::Value& component) override
		{
			soundName = GetVariable<std::string>(component, "soundName");
			isLooping = GetVariable<bool>(component, "isLooping");
			vol = GetVariable<float>(component, "volume");
			pitch = GetVariable<float>(component, "pitch");
			soundState = (SoundState)GetVariable<int>(component, "soundState");
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{

		}
	};
}


#endif
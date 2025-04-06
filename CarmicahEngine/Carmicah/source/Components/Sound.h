#ifndef SOUND_H
#define SOUND_H
#include "BaseComponent.h"
#include <FMOD/fmod.hpp>
#include <string>
namespace Carmicah
{
	enum SoundType
	{
		SOUND_BGM,
		SOUND_SFX
	};
	struct BaseSound
	{
		std::string soundName;
		bool isLooping;
		float vol;
		SoundType type;

	};

	struct Sound : BaseComponent<Sound>
	{
		//enum SoundState
		//{
		//	IS_IDLE,
		//	IS_PLAYING,
		//	IS_PAUSED
		//};
		//std::unordered_map<std::string, std::string> soundMap;
		//bool isLooping;
		//float vol;
		//float pitch;
		////SoundState soundState;
		//FMOD::Channel* soundChannel;
		//FMOD::Sound* sound;

		std::unordered_map<std::string, BaseSound> soundMap;


		Sound& DeserializeComponent(const rapidjson::Value& component) override
		{
			if (component.HasMember("soundMap"))
			{
				const rapidjson::Value& soundList = component["soundMap"];
				for (const auto& soundObj : soundList.GetArray())
				{
					BaseSound newSound;
					newSound.soundName = soundObj["soundName"].GetString();
					newSound.isLooping = soundObj["isLooping"].GetBool();
					newSound.vol = soundObj["vol"].GetFloat();
					newSound.type = (SoundType)soundObj["soundType"].GetInt();

					soundMap[newSound.soundName] = newSound;
				}
			}

			return *this;
			//soundName = GetVariable<std::string>(component, "soundName");
			//isLooping = GetVariable<bool>(component, "isLooping");
			//vol = GetVariable<float>(component, "volume");
			//pitch = GetVariable<float>(component, "pitch");
			//soundState = (SoundState)GetVariable<int>(component, "soundState");

			//return *this;
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{
			writer.String("soundMap");
			writer.StartArray();
			for (const auto& [name, sound] : soundMap)
			{
				writer.StartObject();
				writer.String("soundName");
				writer.String(sound.soundName.c_str());
				writer.String("isLooping");
				writer.Bool(sound.isLooping);
				writer.String("vol");
				writer.Double(sound.vol);
				writer.String("soundType");
				writer.Int((int)sound.type);
				writer.EndObject();
			}
			writer.EndArray();
			//writer.String("soundName");
			//writer.String(soundName.c_str());
			//writer.String("isLooping");
			//writer.Bool(isLooping);
			//writer.String("volume");
			//writer.Double((float)vol);
			//writer.String("pitch");
			//writer.Double((float)pitch);
			////writer.String("soundState");
		//	writer.Int((int)soundState);
		}
	};
}


#endif
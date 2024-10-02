#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "SceneSystem.h"
#include "SoundSystem.h"

namespace Carmicah
{
	// Simulate game logic code
	// i.e any key press to change scenes, fetching game objects to adjust
	// Creating game objects 
	class GameLogic
	{
	private:
		std::shared_ptr<SoundSystem> soundSystemRef;
	public:
		void Init();
		void Update();
	};
}

#endif

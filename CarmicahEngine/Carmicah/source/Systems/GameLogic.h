#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "SceneSystem.h"
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "../ECS/GameObject.h"

namespace Carmicah
{
	// Simulate game logic code
	// i.e any key press to change scenes, fetching game objects to adjust
	// Creating game objects 
	class GameLogic
	{
	private:
		std::shared_ptr<SoundSystem> soundSystemRef;
		std::shared_ptr<PhysicsSystem> physicsRef;
		GameObject mainCharacter;
		GameObject wall;
		GameObject FPSText;

	public:
		void Init();
		void Update();
	};
}

#endif

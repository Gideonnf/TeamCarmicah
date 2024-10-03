#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "SceneSystem.h"
#include "SoundSystem.h"
#include "PhysicsSystem.h"
#include "../ECS/GameObject.h"
#include <GLFW/glfw3.h>
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	// Simulate game logic code
	// i.e any key press to change scenes, fetching game objects to adjust
	// Creating game objects 
	class GameLogic : public BaseSystem
	{
	private:
		std::shared_ptr<SoundSystem> soundSystemRef;
		std::shared_ptr<PhysicsSystem> physicsRef;
		GameObject mainCharacter;
		GameObject FPSText;

	public:
		void Init();
		void Update(GLFWwindow* window);

		void EntityDestroyed(Entity id) override;
	};
}

#endif

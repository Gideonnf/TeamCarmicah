/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GameLogic.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			A system to handle game logic coding. Controling of players and any game play related examples to show in engine

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "SceneSystem.h"
#include "SoundSystem.h"
#include "../Physics/PhysicsSystem.h"
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
		GameObject GameObject2;
		GameObject FPSText;

	public:
		void Init();
		void Update(GLFWwindow* window);

		void EntityDestroyed(Entity id) override;

		void ReceiveMessage(Message* msg) override;
	};
}

#endif

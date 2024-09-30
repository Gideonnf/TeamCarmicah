#ifndef GAME_CORE_H
#define GAME_CORE_H
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	//enum SceneState
	//{
	//	NONE,
	//	INITIALISING,
	//	RUNTIME,
	//	EXIT
	//};

	class SceneSystem : public BaseSystem
	{
	private:
		std::string mScene;

	public:
		//GameCore(std::string scene) : mScene(scene) {}
		void Init(std::string scene);
		void Update();
		void Exit();
		void ReceiveMessage(Message* msg) override;

	};
}

#endif

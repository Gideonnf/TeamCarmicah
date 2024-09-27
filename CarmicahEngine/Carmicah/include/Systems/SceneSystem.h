#ifndef GAME_CORE_H
#define GAME_CORE_H
#include "ECS/BaseSystem.h"

namespace Carmicah
{
	enum SceneState
	{
		BLANKSTATE,
		INITIALISING,
		CHANGESCENE,
		RUNTIME,
		RELOAD,
		EXIT
	};

	class SceneSystem : public BaseSystem
	{
	private:
		std::string mCurrScene;
		std::string mNextScene;
	public:
		SceneState mCurrState;
		SceneState mNextState;

		//GameCore(std::string scene) : mScene(scene) {}
		void Init();
		void ChangeScene(std::string nextScene);
		void Exit();
		void SetScene(std::string scene);
		void ReceiveMessage(Message* msg) override;

	};
}

#endif

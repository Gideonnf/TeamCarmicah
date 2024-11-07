/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SceneSystem.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A system to handle changing of scenes within the engine. Allows for any exit calls or init calls when changing between scenes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GAME_CORE_H
#define GAME_CORE_H
#include "ECS/BaseSystem.h"


//brief:         This class is a system
//                 that handles the changing of scenes within the engine.
//                 It allows for any exit calls or init calls when changing between scenes
namespace Carmicah
{
	enum SceneState
	{
		BLANKSTATE,
		INITIALISING,
	//	CHANGESCENE,
		ONSTART,
		EDITOR,
		RUNTIME,
		//RELOAD,
		EXIT
	};

	class SceneSystem : public BaseSystem
	{
	private:
		std::string mCurrScene;
		std::string mNextScene;
	public:
		SceneSystem();

		SceneState mCurrState;
		SceneState mNextState;
		bool mRuntime;

		//GameCore(std::string scene) : mScene(scene) {}
		void Init();
		void ChangeScene(std::string nextScene);
		void Exit();
		void SetScene(std::string scene);
		void ReceiveMessage(Message* msg) override;
		std::string GetCurrScene();
	};
}

#endif

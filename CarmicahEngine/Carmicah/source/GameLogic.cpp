#include "pch.h"
#include "Systems/GameLogic.h"
#include "Systems/InputSystem.h"
#include "ECS/SystemManager.h"
#include "Systems/GOFactory.h"
#include "Systems/SoundSystem.h"
namespace Carmicah
{
	void GameLogic::Init()
	{
        soundSystemRef = SystemManager::GetInstance()->GetSystem<SoundSystem>();
	}

	void GameLogic::Update()
	{
        if (Input.IsKeyPressed(Keys::KEY_SPACEBAR))
        {
            SystemManager::GetInstance()->ChangeScene("Scene2");
        }

        if (Input.IsKeyPressed(Keys::KEY_1))
        {
            GameObject duckObj = gGOFactory->FetchGO("Duck");
            duckObj.Destroy();
        }

        if (Input.IsKeyPressed(Keys::KEY_P))
        {
            soundSystemRef->PauseResumeSound(soundSystemRef->defaultBGM);
           // SystemManager::GetInstance()->GetSystem<SoundSystem>()->PauseResumeSound
        }
        //if (Input.IsKeyPressed(Keys::KEY_SPACEBAR))
        //{
        //    sceneSys->ChangeScene(scene2Name);
        //}

        //if (Input.IsKeyPressed(Keys::KEY_1))
        //{
        //    GameObject duckObj = gGOFactory->FetchGO("Duck");
        //    duckObj.Destroy();
        //}

        //if (Input.IsKeyPressed(Keys::KEY_P))
        //{
        //    souSystem->PauseResumeSound(souSystem->defaultBGM);
        //}
	}
}
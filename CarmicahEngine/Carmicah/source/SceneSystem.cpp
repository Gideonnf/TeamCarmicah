#include "pch.h"
#include "Systems/SceneSystem.h"
#include "Systems/GOFactory.h"
#include "log.h"
namespace Carmicah
{

	void SceneSystem::Init()
	{
		//mCurrScene = scene;

		// If its changing to a new scene
		//if (mCurrScene != mNextScene)
		//{
		//	mCurrScene = mNextScene;
		//}
		//else
		//{
		//	// Not changing to a new scene
		//	CM_CORE_WARN("Not changing scene");
		//}
		mCurrScene = mNextScene;
		gGOFactory->ImportGOs(mCurrScene);
		mNextState = mCurrState = SceneState::RUNTIME;
	}

	/// <summary>
	/// Only used initial setting of starting scene when engine is ran
	/// Can be read from 
	/// </summary>
	/// <param name="scene"></param>
	void SceneSystem::SetScene(std::string scene)
	{
		mNextScene = scene;
	}

	void SceneSystem::ChangeScene(std::string nextScene)
	{
		if (nextScene == mCurrScene)
		{
			mNextState = RELOAD;
		}
		else
		{
			// Change scene
			mNextState = CHANGESCENE;

			mNextScene = nextScene;
		}
	}

	void SceneSystem::Exit()
	{
		// initialize the next scene
		if (mNextState == CHANGESCENE || mNextState == RELOAD)
			mNextState = INITIALISING;
		else
			mNextState = EXIT;

		//mState = EXIT;
		//gGOFactory->ExportGOs(mCurrScene); // Dont save objects for now
		gGOFactory->DestroyAll();
		SystemManager::GetInstance()->UpdateDestroyed();
	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}
}
#include "pch.h"
#include "Systems/SceneSystem.h"
#include "Systems/GOFactory.h"
#include "log.h"
namespace Carmicah
{

	void SceneSystem::Init(std::string nextScene)
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

		gGOFactory->ImportGOs(nextScene);
		mState = SceneState::RUNTIME;
	}

	void SceneSystem::SetScene(std::string scene)
	{
		mCurrScene = scene;
	}

	void SceneSystem::ChangeScene(std::string nextScene)
	{
		if (nextScene == mCurrScene)
		{
			mState = RELOAD;
		}
		else
		{
			// Change scene
			mState = CHANGESCENE;

			mNextScene = nextScene;
		}
	}

	void SceneSystem::Exit()
	{
		// initialize the next scene
		if (mState == CHANGESCENE || mState == RELOAD)
			mState = INITIALISING;

		//mState = EXIT;
		gGOFactory->ExportGOs(mCurrScene);
		gGOFactory->DestroyAll();

	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}
}
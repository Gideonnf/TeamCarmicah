#include "pch.h"
#include "Systems/SceneSystem.h"
#include "Systems/GOFactory.h"
#include "log.h"
#include "AssetManager.h"
#include "Systems/SerializerSystem.h"

namespace Carmicah
{
	//const char* sceneLoc{ "../Assets/Scene/" };

	void SceneSystem::Init()
	{
		mCurrScene = mNextScene;
		
		//gGOFactory->ImportGOs(mCurrScene);
		SerializerSystem::GetInstance()->DeserializeScene(mCurrScene);
		mNextState = mCurrState = SceneState::RUNTIME;
	}

	/// <summary>
	/// Only used initial setting of starting scene when engine is ran
	/// Can be read from 
	/// </summary>
	/// <param name="scene"></param>
	void SceneSystem::SetScene(std::string scene)
	{
		std::string sceneFile;
		if (AssetManager::GetInstance()->GetScene(scene, sceneFile))
			mNextScene = sceneFile;
		else
			CM_CORE_ERROR("Unable to set scene.");
	}

	void SceneSystem::ChangeScene(std::string nextScene)
	{
		std::string sceneFile;
		if (AssetManager::GetInstance()->GetScene(nextScene, sceneFile))
		{
			if (sceneFile == mCurrScene)
			{
				mNextState = RELOAD;
			}
			else
			{
				// Change scene
				mNextState = CHANGESCENE;

				mNextScene = sceneFile;
			}
		}
		else
		{
			CM_CORE_ERROR("Unable to change scene.");
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
		//gGOFactory->ExportGOs(sceneLoc + mCurrScene + ".json"); // Dont save objects for now
		gGOFactory->DestroyAll();
		gGOFactory->UpdateDestroyed();
	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}

	std::string SceneSystem::GetCurrScene()
	{
		return mCurrScene;
	}
}
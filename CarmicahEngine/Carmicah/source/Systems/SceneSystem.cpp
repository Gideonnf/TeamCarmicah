/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SceneSystem.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A system to handle changing of scenes within the engine. Allows for any exit calls or init calls when changing between scenes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "Systems/SceneSystem.h"
#include "Systems/GOFactory.h"
#include "log.h"
#include "Systems/AssetManager.h"
#include "Systems/SerializerSystem.h"
#include "../Editor/SceneToImgui.h"

namespace Carmicah
{
	//const char* sceneLoc{ "../Assets/Scene/" };

	void SceneSystem::Init()
	{
		mCurrScene = mNextScene;
		std::string sceneFile;

		if (!AssetManager::GetInstance()->GetScene(mCurrScene, sceneFile))
		{
			// Loading scene failed, open default scene
			CM_CORE_ERROR("Can't get next scene file, opening default scene");
			AssetManager::GetInstance()->GetScene("DefaultScene", sceneFile);
			mCurrScene = mNextScene = "DefaultScene";
		}

		
		//gGOFactory->ImportGOs(mCurrScene);
		if (SerializerSystem::GetInstance()->DeserializeScene(sceneFile))
		{
			mNextState = mCurrState = SceneState::RUNTIME;
		}
		else
		{
			// deserializing scene failed so use default scene
			CM_CORE_ERROR("Can't get deserialize scene file, opening default scene");

			AssetManager::GetInstance()->GetScene("DefaultScene", sceneFile);
			mCurrScene = mNextScene = "DefaultScene";
			SerializerSystem::GetInstance()->DeserializeScene(sceneFile);
		}
		// For transferring curr scene to imgui for now. TODO: Better implementation
		SceneToImgui::GetInstance()->currentScene = mCurrScene;
	}

	/// <summary>
	/// Only used initial setting of starting scene when engine is ran
	/// Can be read from 
	/// </summary>
	/// <param name="scene">name of the file</param>
	void SceneSystem::SetScene(std::string scene)
	{
		mNextScene = scene;
	}

	void SceneSystem::ChangeScene(std::string nextScene)
	{
		// technically dont have to check here since i check in init
		std::string sceneFile;
		if (AssetManager::GetInstance()->GetScene(nextScene, sceneFile))
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
		if (mCurrScene != "DefaultScene")
		{
			std::string sceneFile;
			AssetManager::GetInstance()->GetScene(mCurrScene, sceneFile);
			//SerializerSystem::GetInstance()->SerializeScene(sceneFile);
		}
		//gGOFactory->ExportGOs(sceneLoc + mCurrScene + ".json"); // Dont save objects for now
		gGOFactory->DestroyAll();
		gGOFactory->UpdateDestroyed();
	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		UNUSED(msg);
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}

	std::string SceneSystem::GetCurrScene()
	{
		return mCurrScene;
	}
}
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

	SceneSystem::SceneSystem() : mNextState{ SceneState::INITIALISING }, mRuntime{ false } {}

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

		
		if (mNextState == INITIALISING)
		{
			//gGOFactory->ImportGOs(mCurrScene);
			if (SerializerSystem::GetInstance()->DeserializeScene(sceneFile))
			{
				mNextState = mCurrState = SceneState::EDITOR;
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
		else if (mNextState == RELOAD)
		{
			if (SerializerSystem::GetInstance()->DeserializeScene(mCurrScene + "_temp"))
			{
				mNextState = mCurrState = SceneState::EDITOR;
				remove((mCurrScene + "_temp").c_str());
			}
			// if theres no temp file means it was a scene change from a previous scene
			else
			{
				if (SerializerSystem::GetInstance()->DeserializeScene(sceneFile))
				{
					mNextState = mCurrState = SceneState::EDITOR;
				}
			}
		}
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

	bool SceneSystem::ChangeScene(std::string nextScene)
	{
		// technically dont have to check here since i check in init
		mNextState = INITIALISING;

		std::string sceneFile;
		// only check for a new scene file if we're changing to a new scene
		if ((nextScene != mCurrScene) && AssetManager::GetInstance()->GetScene(nextScene, sceneFile))
		{
			mNextScene = nextScene;
			return true;
		}
		else
		{
			return false;
			CM_CORE_ERROR("Unable to change scene.");
		}

		//if (AssetManager::GetInstance()->GetScene(nextScene, sceneFile))
		//{
		//	if (nextScene == mCurrScene)
		//	{
		//		mNextState = RELOAD;
		//	}
		//	else
		//	{
		//		// Change scene
		//		mNextState = CHANGESCENE;

		//		mNextScene = nextScene;
		//	}
		//}

	}

	void SceneSystem::ResetScene()
	{
		mNextState = INITIALISING;
		
	}

	void SceneSystem::Exit()
	{
		// initialize the next scene
		//if (mNextState == CHANGESCENE || mNextState == RELOAD)
		//	mNextState = INITIALISING;
		//else
		//	mNextState = EXIT;

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

		//mNextState = mCurrState = SceneState::RUNTIME;
	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		//UNUSED(msg);
		// Check what time of msg
		if (msg->mMsgType == MSG_RUNTIME)
		{
			// Cast it
		
			// currently playing but going to stop
			if (mRuntime)
			{
				mRuntime = false;
				mNextState = SceneState::RELOAD;
				//ResetScene();
				//ChangeScene(mCurrScene);
			}
			else
			{
				mRuntime = true;
				mNextState = SceneState::ONSTART;
				// Serialize the temp scene
				// Currently this is writing into editor folder for some reason
				// i mean i know why cause the path isnt the full asset folder path
				// but might be able to change to using just jsonwriter without making a file to store temporary scenes
				SerializerSystem::GetInstance()->SerializeScene(mCurrScene + "_temp");
			}
		}

		// Forward it to editor or button depending on current state
		if (msg->mMsgType == MSG_ENTITYPICKED)
		{
			if (mRuntime)
			{
				ButtonClicked newMsg(dynamic_cast<EntityPickedMessage*>(msg)->mEntityID);
				SendSysMessage(&newMsg);
				EditorEntityPicked pickedMsg(dynamic_cast<EntityPickedMessage*>(msg)->mEntityID);
				SendSysMessage(&pickedMsg);
			}
			else
			{
				EditorEntityPicked newMsg(dynamic_cast<EntityPickedMessage*>(msg)->mEntityID);
				SendSysMessage(&newMsg);
			}
		}
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}

	std::string SceneSystem::GetCurrScene()
	{
		return mCurrScene;
	}

	bool SceneSystem::IsRuntime() const
	{
		return mRuntime;
	}
}
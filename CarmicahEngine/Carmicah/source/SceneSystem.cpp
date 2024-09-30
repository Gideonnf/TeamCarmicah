#include "pch.h"
#include "Systems/SceneSystem.h"
#include "Systems/GOFactory.h"

namespace Carmicah
{

	void SceneSystem::Init(std::string scene)
	{
		mScene = scene;
		gGOFactory->ImportGOs(mScene);
	}

	void SceneSystem::Update()
	{

	}

	void SceneSystem::Exit()
	{
		gGOFactory->ExportGOs(mScene);
		gGOFactory->DestroyAll();
	}

	void SceneSystem::ReceiveMessage(Message* msg)
	{
		// Can use this for scene changing
		// Call exit
		// Call init to import new GOs
		
	}
}
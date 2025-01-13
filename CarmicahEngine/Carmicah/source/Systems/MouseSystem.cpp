/* file documentation -----------------------------------------------------------------------------
\file       ButtonSystem.cpp
\author     Gideon Francis
\course     CSD 2400
\date       14/01/25

\brief      Implementation of the Mouse System class, which manages objects that have a script, transform
				and collider component. Calls script functions when mouse is entering, hovering and exiting any objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-------------------------------------------------------------------------------------------------*/


#include "pch.h"
#include <ECS/ECSTypes.h>
#include <ECS/GameObject.h>
#include <Components/Transform.h>
#include <Components/Collider2D.h>
#include <Components/Script.h>
#include "MouseSystem.h"

namespace Carmicah
{
	void MouseSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Script>());

		SystemManager::GetInstance()->SetSignature<MouseSystem>(mSignature);

	}

	void MouseSystem::Update()
	{
		// TODO: I dont know if its better to use the input system's color picking
		// or to check here for mouse entering, hovering and exiting
		for (auto& entity : mEntitiesSet)
		{
			// Check 
		}
	}

	void MouseSystem::Exit()
	{

	}

	void MouseSystem::OnEnter(Entity entityID)
	{

	}

	void MouseSystem::OnExit(Entity entityID)
	{

	}

	void MouseSystem::OnHover(Entity entityID)
	{

	}
}
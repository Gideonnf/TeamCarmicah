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
#include "../Input/InputSystem.h"

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
		float xPos = Input.GetMouseX();
		float yPos = Input.GetMouseY();
		// nvm i need to use the object picking method from input system to deal with this
		// ill finish it tmr 
		for (auto& entity : mEntitiesSet)
		{
			Collider2D collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			if (xPos > collider.min.x && xPos < collider.max.x)
			{
				if (yPos > collider.min.y && yPos < collider.max.y)
				{
					if (!collider.mouseEnter)
					{
						OnEnter(entity);
						collider.mouseEnter = true;
					}
					else if (collider.mouseEnter)
					{
						OnHover(entity);
					}

					// Go next entity
					continue;
				}
			}

			// if it reaches here, means its not hovering/entering
			// If it already entered, means its exiting right?
			if (collider.mouseEnter)
			{
				OnExit(entity);
				collider.mouseEnter = false;
			}
		}
	}

	void MouseSystem::Exit()
	{

	}

	void MouseSystem::ReceiveMessage(Message* msg)
	{

	}

	void MouseSystem::OnEnter(Entity entityID)
	{
		CM_CORE_INFO("OnEnter");
	}

	void MouseSystem::OnExit(Entity entityID)
	{
		CM_CORE_INFO("Exit");

	}

	void MouseSystem::OnHover(Entity entityID)
	{
		CM_CORE_INFO("OnHover");

	}
}
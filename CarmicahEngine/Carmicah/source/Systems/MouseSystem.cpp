/* file documentation -----------------------------------------------------------------------------
\file       MouseSystem.cpp
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
#include "../Editor/SceneToImgui.h"
#include "Systems/AssetManager.h"
#include "Systems/GOFactory.h"

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
		// NOTE: I dont know if i should do this here, or send a message from input system to here
		// probably send msg but if it works it works
		
		Vec2i mousePosI = { static_cast<int>(Input.GetMousePosition().x), 1080 - static_cast<int>(Input.GetMousePosition().y) };
		//float ndcX = (2.0f * Input.GetMousePosition().x) / AssetManager::GetInstance()->enConfig.Width - 1.0f;
		//float ndcY = 1.0f - (2.0f * Input.GetMousePosition().y) / AssetManager::GetInstance()->enConfig.Height;
		Vec2f worldMousePos = Input.GetMouseWorldPosition();
	

		//CM_CORE_INFO("Mouse Position {}, {}", ndcX, ndcY);
//#ifndef CM_INSTALLER
//		SceneToImgui::SCENE_IMGUI currScene = SceneToImgui::GetInstance()->GetHovering();
//		if (currScene == SceneToImgui::NO_SCENE)
//		{
//			return;
//		}
//		EntityPickedMessage msg(SceneToImgui::GetInstance()->IDPick(static_cast<SceneToImgui::SCENE_IMGUI>(SceneToImgui::GetInstance()->GetHovering()), mousePosI.x, mousePosI.y));
//#else
//		EntityPickedMessage msg(SceneToImgui::GetInstance()->IDPick(SceneToImgui::GAME_SCENE, mousePosI.x, mousePosI.y));
//#endif
		//currEntity = SceneToImgui::GetInstance()->IDPick(static_cast<SceneToImgui::SCENE_IMGUI>(SceneToImgui::GetInstance()->GetHovering()), mousePosI.x, mousePosI.y);
		for (auto& entity : mEntitiesSet)
		{
			Collider2D& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			if (worldMousePos.x > collider.min.x && worldMousePos.x < collider.max.x)
			{
				if (worldMousePos.y > collider.min.y && worldMousePos.y < collider.max.y)
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

					continue;
				}
			}

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
		UNUSED(msg);
	}

	void MouseSystem::OnEnter(Entity entityID)
	{
		//CM_CORE_INFO("Enter " + std::to_string(entityID));
		OnMouseMsg newMsg(MSG_MOUSEENTER, entityID);
		SendSysMessage(&newMsg);
	}

	void MouseSystem::OnExit(Entity entityID)
	{
		//CM_CORE_INFO("Exit" + std::to_string(entityID));
		OnMouseMsg newMsg(MSG_MOUSEEXIT, entityID);
		SendSysMessage(&newMsg);

	}

	void MouseSystem::OnHover(Entity entityID)
	{
		//CM_CORE_INFO("OnHover");
		OnMouseMsg newMsg(MSG_MOUSEHOVER, entityID);
		SendSysMessage(&newMsg);

	}
}
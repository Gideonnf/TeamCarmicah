/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RenderTransformSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Transform System. Header file that contains the function declarations of the Render Transform System.
				The system only handles changing the internal scale calculations for Images when texture changes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "RenderTransformSystem.h"

#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Components/UITransform.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"


namespace Carmicah
{
	void RenderTransformSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());

		SystemManager::GetInstance()->SetSignature<RenderTransformSystem>(mSignature);
	}

	void RenderTransformSystem::Update()
	{
		for (auto& e : mEntitiesSet)
		{
			auto& render = ComponentManager::GetInstance()->GetComponent<Renderer>(e);
			if (render.Updated())
			{
				const Texture& t = AssetManager::GetInstance()->GetAsset<Texture>(ComponentManager::GetInstance()->GetComponent<Renderer>(e).Texture());
				if(ComponentManager::GetInstance()->HasComponent<Transform>(e))
					ComponentManager::GetInstance()->GetComponent<Transform>(e).InternalScale((AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * t.mtx.m00) - 1.f, (AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * t.mtx.m11) - 1.f);
				else
					ComponentManager::GetInstance()->GetComponent<UITransform>(e).InternalScale((AssetManager::GetInstance()->enConfig.maxTexSize * t.mtx.m00) - 1.f, (AssetManager::GetInstance()->enConfig.maxTexSize * t.mtx.m11) - 1.f);
			}

		}
	}
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AnimationSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles the switching and behaviour of gameobjects with the animator and renderer component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ECS/ECSTypes.h>
#include "Graphics/AnimationSystem.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"
#include "Systems/AssetManager.h"

namespace Carmicah
{
	void AnimationSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Animation>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<AnimationSystem>(mSignature);
	}

	void AnimationSystem::Update()
	{
		for (auto& entity : mEntitiesSet)
		{
			auto& animation = ComponentManager::GetInstance()->GetComponent<Animation>(entity);

			animation.time += static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
			if (animation.time > animation.maxTime)
			{
				AnimAtlas& a{ AssetManager::GetInstance()->GetAsset<AnimAtlas>(animation.animAtlas) };
				auto& rend = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);

				if (ComponentManager::GetInstance()->HasComponent<Transform>(entity))
					ComponentManager::GetInstance()->GetComponent<Transform>(entity).Update();

				if (animation.currPiece > a.anim.size())
					animation.currPiece = 0;

				rend.Texture(a.anim[animation.currPiece].second);
				animation.maxTime = a.anim[animation.currPiece].first;

				if (++animation.currPiece >= a.anim.size())
					animation.currPiece = 0;
				animation.time = 0.f;
			}
		}
	}
}
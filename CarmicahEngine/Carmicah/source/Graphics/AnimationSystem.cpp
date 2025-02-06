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
		float dt = static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());

		for (auto& entity : mEntitiesSet)
		{
			auto& animation = ComponentManager::GetInstance()->GetComponent<Animation>(entity);

			if (animation.animState == Animation::ANIM_CODE::STOP_ANIM)
				continue;
			else if (animation.animState == Animation::ANIM_CODE::ANIM_CHANGED)
			{
				AnimAtlas& a{ AssetManager::GetInstance()->GetAsset<AnimAtlas>(animation.GetAnimAtlas()) };
				auto& rend = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
				if (ComponentManager::GetInstance()->HasComponent<Transform>(entity))
					ComponentManager::GetInstance()->GetComponent<Transform>(entity).Update();
				animation.currPiece = 0;
				rend.Texture(a.anim[animation.currPiece].second);
				animation.time = 0.f;
				animation.maxTime = a.anim[animation.currPiece].first;
				if (a.numLoops == 0)
					animation.animState = Animation::ANIM_CODE::INF_LOOP;
				else
				{
					animation.animState = Animation::ANIM_CODE::FINITE;
					animation.loopsLeft = a.numLoops;
				}
			}


			// Animation loop
			animation.time += dt;
			if (animation.time > animation.maxTime)
			{
				AnimAtlas& a{ AssetManager::GetInstance()->GetAsset<AnimAtlas>(animation.GetAnimAtlas()) };
				auto& rend = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);

				if (ComponentManager::GetInstance()->HasComponent<Transform>(entity))
					ComponentManager::GetInstance()->GetComponent<Transform>(entity).Update();

				// Add Counter + check loop
				if (++animation.currPiece >= a.anim.size())
				{
					// Only need check if Finite loops
					if (animation.animState == Animation::ANIM_CODE::FINITE && --animation.loopsLeft == 0)
						animation.animState = Animation::ANIM_CODE::STOP_ANIM;
					animation.currPiece = 0;
				}

				// Updates texture
				if (animation.currPiece <= a.anim.size() - 1)
				{
					rend.Texture(a.anim[animation.currPiece].second);
					animation.maxTime = a.anim[animation.currPiece].first;
				}
				animation.time = 0.f;
			}
		}
	}
}
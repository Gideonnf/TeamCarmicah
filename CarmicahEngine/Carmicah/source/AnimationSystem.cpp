#include "pch.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Systems/AnimationSystem.h"
#include "Components/Renderer.h"
#include "Components/Animation.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "CarmicahTime.h"
#include "AssetManager.h"

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
			auto& anim = ComponentManager::GetInstance()->GetComponent<Animation>(entity);
			auto& rend = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);

			if (!anim.notChangedAnim)
			{
				auto& tex = AssetManager::GetInstance()->textureMaps.find(rend.texture);
				if (tex != AssetManager::GetInstance()->textureMaps.end())
				{
					anim.xSlice = tex->second.xSlices + 1;
					anim.ySlice = tex->second.ySlices + 1;
					anim.time = anim.maxTime;
					anim.currPiece = anim.xSlice * anim.ySlice;
				}
				anim.notChangedAnim = true;
			}

			anim.time += static_cast<float>(CarmicahTimer::GetDt());
			if (anim.time > anim.maxTime)
			{
				if (++anim.currPiece >= anim.xSlice * anim.ySlice)
					anim.currPiece = 0;
				anim.time = 0.f;

				float xMulti = static_cast<float>(anim.currPiece % anim.xSlice);
				float yMulti = static_cast<float>(anim.ySlice - (anim.currPiece / anim.xSlice));

				
				// Animation translates
				rend.texureMat = glm::mat3(1);
				glm::vec2 animScale = glm::vec2{ 1.f / static_cast<float>(anim.xSlice), 1.f / static_cast<float>(anim.ySlice) };
				glm::vec2 animTranslate = glm::vec2{ xMulti * animScale.x , yMulti * animScale.y};
				rend.texureMat = glm::translate(rend.texureMat, animTranslate);
				rend.texureMat = glm::scale(rend.texureMat, animScale);
			}
		}
	}
}
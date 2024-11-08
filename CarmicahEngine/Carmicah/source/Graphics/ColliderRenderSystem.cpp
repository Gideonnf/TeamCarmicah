/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ColliderRenderSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles rendering the collision boxes of gameobjects with the collider2D component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/ColliderRenderSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"

namespace Carmicah
{
	void ColliderRenderSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ColliderRenderSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.debugShader);

		primitive = &AssetManager::GetInstance()->GetAsset<BasePrimitive>("DebugSquare");
		GenDebugBatch(*primitive);
	}

	void ColliderRenderSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
			DeleteBatchData(id, test->second.posInMemory, false, 4);
	}



	void ColliderRenderSystem::Render(Entity& cam)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);

		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx3x3f camSpace{};
			camSpace.scaleThis(camTrans.scale.x, camTrans.scale.y).rotDegThis(-camTrans.rot).translateThis(-camTrans.pos.x, -camTrans.pos.y);
			GLint uniformLoc{};
			if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
		}

		for (auto& entity : mEntityBufferLoc)
		{
			if (!entity.second.isActive)
				continue;

			auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity.first);

			Mtx3x3f trans{};
			trans.translateThis((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f)
				.scaleThis(collider.max.x - collider.min.x, collider.max.y - collider.min.y);

			EditDebugBatchData(entity.first, entity.second.posInMemory, *primitive, trans, true, DEBUG_LAYER);
		}


		// Add new Data
		if (mEntityBufferLoc.size() != mEntitiesSet.size())
		{

			for (auto& entity : mEntitiesSet)
			{
				auto e{ mEntityBufferLoc.find(entity) };
				if (e != mEntityBufferLoc.end())
					continue;
				EntityData ed{};
				ed.isActive = true;
				ed.posInMemory = mEntityBufferIDTrack++;

				auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
				Mtx3x3f trans{};
				trans.translateThis((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f)
					.scaleThis(collider.max.x - collider.min.x, collider.max.y - collider.min.y);


				EditDebugBatchData(entity, ed.posInMemory, *primitive, trans, true, DEBUG_LAYER);
				mEntityBufferLoc.emplace(entity, ed);
			}
		}


		BatchDebugRender();

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
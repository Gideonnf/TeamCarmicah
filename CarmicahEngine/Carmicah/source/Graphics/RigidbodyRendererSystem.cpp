/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RigidbodyRendererSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Rigidbody Render System handles rendering the rigidbody parts such as velocity of gameobjects with the rigidbody component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <limits>
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/RigidbodyRendererSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"


namespace Carmicah
{
	void RigidbodyRendererSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<RigidbodyRendererSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.debugShader);

		primitive = &AssetManager::GetInstance()->GetAsset<BasePrimitive>("DebugLine");
		GenDebugBatch(*primitive);

	}

	void RigidbodyRendererSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
			DeleteBatchData(id, test->second.posInMemory, false, 2);
	}



	void RigidbodyRendererSystem::Render(Entity& cam)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);

		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx3x3f camSpace{};
			camSpace.scaleThis(camTrans.Scale()).rotDegThis(-camTrans.Rot()).translateThis(-camTrans.Pos());
			GLint uniformLoc{};
			if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
		}


		for (auto& entity : mEntityBufferLoc)
		{
			if (!entity.second.isActive)
				continue;

			auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity.first);
			if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
				fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
				continue;

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity.first);


			// Get rotation
			float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
			// Get scale multi
			//float biggerVel = fmaxf(fabs(rigidbody.velocity.x), fabs(rigidbody.velocity.y));
			//trans.translateThis(transform.pos.x, transform.pos.y).scaleThis(biggerVel, biggerVel).rotRadThis(rot);

			Mtx3x3f trans{};
			trans.translateThis(transform.Pos()).rotRadThis(rot);

			EditDebugBatchData(entity.first, entity.second.posInMemory, *primitive, trans, true, DEBUG_LAYER);
		}


		// Add new Data
		if (mActiveEntityCount != mEntitiesSet.size())
		{
			for (auto& entity : mEntitiesSet)
			{
				auto e{ mEntityBufferLoc.find(entity) };
				if (e != mEntityBufferLoc.end())
				{
					if (!e->second.isActive)
					{
						ToggleActiveEntity(e->second, true);
					}
					continue;
				}

				auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity);
				if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
					fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
					continue;

				EntityData ed{};
				ToggleActiveEntity(ed, true);
				ed.posInMemory = mEntityBufferIDTrack++;

				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);


				// Get rotation
				float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
				Mtx3x3f trans{};
				trans.translateThis(transform.Pos()).rotRadThis(rot);


				EditDebugBatchData(entity, ed.posInMemory, *primitive, trans, true, DEBUG_LAYER);
				mEntityBufferLoc.emplace(entity, ed);
			}
		}


		BatchDebugRender();

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
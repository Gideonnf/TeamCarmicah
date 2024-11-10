/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			UIGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			UI Graphics System handles the rendering of textured meshes onto the screen (using transform, screen reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/UIGraphicsSystem.h"

#include <ECS/ECSTypes.h>
#include "Systems/GOFactory.h"

#include "Components/Transform.h"
#include "Components/Renderer.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

#include "Systems/AssetManager.h"
#include "log.h"

namespace Carmicah
{
	void UIGraphicsSystem::Init(const float& screenWidth, const float& screenHeight)
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<UIGraphicsSystem>(mSignature);
		BaseGraphicsSystem::Init("");
		screenMtx.translateThis(-1.f, -1.f).scaleThis(2 / screenWidth, 2 / screenHeight);

		GenBatch(AssetManager::GetInstance()->GetAsset<Primitive>("Square"));
	}

	void UIGraphicsSystem::EntityDestroyed(Entity id)
	{
		auto test = mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
			DeleteBatchData(id, test->second.posInMemory, false, 4);
	}



	void UIGraphicsSystem::Render()
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);

		GLint uniformLoc{};
		if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, screenMtx.m);


		for (auto& entity : mEntityBufferLoc)
		{
			if (!entity.second.isActive)
				continue;

			auto& transform = ComponentManager::GetInstance()->GetComponent<UITransform>(entity.first);

			if (!transform.Updated())
				continue;

			EditBatchData(entity.first, entity.second.posInMemory, false, UI_LAYER);
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
				EntityData ed{};
				ToggleActiveEntity(ed, true);
				ed.posInMemory = mEntityBufferIDTrack++;

				EditBatchData(entity, ed.posInMemory, false, UI_LAYER);
				mEntityBufferLoc.emplace(entity, ed);
			}

		}
	

		BatchRender();

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
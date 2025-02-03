/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ParticleSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Graphics System handles the rendering of textured meshes onto the screen (using transform, camera reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/ParticlesSystem.h"

#include <ECS/ECSTypes.h>
#include "Systems/GOFactory.h"

#include "Components/Transform.h"
#include "Components/Particles.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

#include "Systems/AssetManager.h"
#include "log.h"

namespace Carmicah
{
	void ParticlesSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Particles>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ParticlesSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);
	}

	void ParticlesSystem::EntityDestroyed(Entity id)
	{
		auto test =  mEntityBufferLoc.find(id);
		if (test != mEntityBufferLoc.end())
		{
			DeleteBatchData(id);
			mEntityBufferLoc.erase(id);
		}
	}

	void ParticlesSystem::Update()
	{
		// Generate particles
		for (std::unordered_map<unsigned int, EntityData>::iterator entity = mEntityBufferLoc.begin(); entity != mEntityBufferLoc.end();)
		{
			auto& particles = ComponentManager::GetInstance()->GetComponent<Particles>(entity->first);



			++entity;
		}
		
		// Batch Render Particles

		//EditBatchData(entity->first, true, BASE_LAYER);
		
		//SetNewEntity(entity, ComponentManager::GetInstance()->GetComponent<Renderer>(entity).model, 0, true, false);
		//EditBatchData(entity, true, BASE_LAYER);
	}

}
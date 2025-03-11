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
#include "Components/ParticleEmitter.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Graphics/RenderHelper.h"

#include "Systems/AssetManager.h"
#include "log.h"

namespace Carmicah
{
	void ParticlesSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<ParticleEmitter>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ParticlesSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);

		BaseGraphicsSystem::GenBatch("Square", 2, true, false);	// World Space
		BaseGraphicsSystem::GenBatch("Square", 2, false, false);// UI Space
	}

	// Uses basic.vert/frag
	// Unwrites id buffer
	void ParticlesSystem::Update()
	{
		float dt = static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());

		// Delete dead particles First
		for (size_t i{}; i < 2; ++i)
		{
			size_t eraseCounter{}, arrSize{ mParticles[i].size() };
			for (size_t it{}; it < mParticles[i].size();)
			{
				particle& par = mParticles[i][it];
				// Swap this with the last particle, continue (don't advance)
				if ((par.timeLeft -= dt) < 0.f)
				{
					std::swap(par, mParticles[i][arrSize - (++eraseCounter)]);
					if (eraseCounter == arrSize - 1)
						break;
					continue;
				}
				++it;
			}
			// Remove non-active particles from the list
			if (eraseCounter > 0)
			{
				if (eraseCounter >= arrSize - 1)
					mParticles[i].clear();
				else
					mParticles[i].erase(mParticles[i].begin() + (arrSize - eraseCounter), mParticles[i].end());
			}

		}

		// Generate particles
		for (auto& e : mEntitiesSet)
		{
			// Emitter Qualities
			auto& emitter = ComponentManager::GetInstance()->GetComponent<ParticleEmitter>(e);

			if (!emitter.HasEmitterQualities(ParticleEmitter::EMITTER_ACTIVE) || emitter.particleLifeTime < dt)
				continue;

			if (emitter.HasEmitterQualities(ParticleEmitter::EMITTER_BURST))
			{
				emitter.optAliveTime -= dt;
				if (emitter.optAliveTime < 0)
				{
					emitter.SetEmitterQualities(ParticleEmitter::EMITTER_ACTIVE, false);
					continue;
				}
			}

			emitter.timePassed += dt;
			float spawnTime = 1.f / static_cast<float>(emitter.spawnPerSec);

			// Spawn Loop
			if (emitter.timePassed > spawnTime)
			{
				float emitterSpeed = emitter.particleSpeed.length();
				float emitterDir = emitter.particleSpeed.angleRad();

				// World Space == array 0
				if (ComponentManager::GetInstance()->HasComponent<Transform>(e))
				{
					auto& tf = ComponentManager::GetInstance()->GetComponent<Transform>(e);
					do
					{
						particle par;

						par.texture = emitter.texture;
						par.timeLeft = emitter.particleLifeTime;
						par.mtx = tf.rotTrans;
						float scaleMod{ CarmicahTime::GetInstance()->GenerateRandFloat(std::max(0.f, emitter.scaleRange.x), std::max(emitter.scaleRange.x, emitter.scaleRange.y)) };
						par.mtx.scaleThis(tf.accumulatedScale.x * scaleMod, tf.accumulatedScale.y * scaleMod);
						float xAddition = CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.spawnRadius.x, emitter.spawnRadius.x);
						par.mtx.m[6] += xAddition;
						par.mtx.m[7] += CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.spawnRadius.y, emitter.spawnRadius.y);
						if (emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_RADIAL))
						{
							float percentage = xAddition / emitter.spawnRadius.x;
							par.vel = Vector2DGenerateFromAngleRad(emitterDir + (emitter.angleRange / 2.f * percentage) * PI / 180.f);
						}
						else
							par.vel = Vector2DGenerateFromAngleRad(emitterDir + CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.angleRange / 2.f, emitter.angleRange / 2.f) * PI / 180.f);
						
						par.vel *= emitterSpeed + CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.speedRange / 2.f, emitter.speedRange / 2.f);
						par.alpha.x = 1.f;
						if (emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_FADE))
							par.alpha.y = 1.f / par.timeLeft;
						//if (emitter.isShrink)
						//	par.scaling = 

						par.hasFriction = emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_FRICTION);
						par.hasGrav = emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_GRAVITY);

						mParticles[0].emplace_back(par);
						emitter.timePassed -= spawnTime;
					} while (emitter.timePassed > spawnTime);
				}
				// UI Space == array 1
				else
				{
					auto& tf = ComponentManager::GetInstance()->GetComponent<UITransform>(e);
					const Texture& texture = AssetManager::GetInstance()->GetAsset<Texture>(ComponentManager::GetInstance()->GetComponent<Renderer>(e).Texture());
					Vec2f intTexScale{ (AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * texture.mtx.m00) - 1.f, (AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * texture.mtx.m11) - 1.f };
					do
					{
						particle par;

						par.texture = emitter.texture;
						par.timeLeft = emitter.particleLifeTime;
						float scaleMod{ CarmicahTime::GetInstance()->GenerateRandFloat(std::max(0.f, emitter.scaleRange.x), std::max(emitter.scaleRange.x, emitter.scaleRange.y))};
						par.mtx = tf.rotTrans;
						par.mtx.scaleThis(intTexScale.x * tf.accumulatedScale.x * scaleMod, intTexScale.y * tf.accumulatedScale.y * scaleMod);
						float xAddition = CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.spawnRadius.x, emitter.spawnRadius.x);
						par.mtx.m[6] += xAddition;
						par.mtx.m[7] += CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.spawnRadius.y, emitter.spawnRadius.y);
						if (emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_RADIAL))
						{
							float percentage = xAddition / emitter.spawnRadius.x;
							par.vel = Vector2DGenerateFromAngleRad(emitterDir + (emitter.angleRange / 2.f * percentage) * PI / 180.f);
						}
						else
							par.vel = Vector2DGenerateFromAngleRad(emitterDir + CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.angleRange / 2.f, emitter.angleRange / 2.f) * PI / 180.f);

						par.vel *= emitterSpeed + CarmicahTime::GetInstance()->GenerateRandFloat(-emitter.speedRange / 2.f, emitter.speedRange / 2.f);
						par.alpha.x = 1.f;
						if (emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_FADE))
							par.alpha.y = 1.f / par.timeLeft;

						par.hasFriction = emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_FRICTION);
						par.hasGrav = emitter.HasEmitterQualities(ParticleEmitter::PARTICLES_GRAVITY);


						mParticles[1].emplace_back(par);
						emitter.timePassed -= spawnTime;
					} while (emitter.timePassed > spawnTime);
				}
			}
		}

		// Gets ready data
		auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>("Square")};

		
		for (size_t i{}; i < 2; ++i)// WorldSpace, UISpace
		{
			float depth{ CalcDepth(mNearestDepth, (i == 0 ? BASE_LAYER : UI_LAYER)) };
			BufferCPUSide& vtxBuf = RenderHelper::GetInstance()->mParticleData[i];
			vtxBuf.vtxSize = mParticles[i].size();
			RenderHelper::GetInstance()->ReserveFontBuffer(vtxBuf, mParticles[i].size());

			// Loops through all active particles and updates
			for (size_t it{}; it < mParticles[i].size();++it)
			{
				particle& par = mParticles[i][it];
				// Update the particle
				if (par.hasGrav)
					par.vel.y -= mGrav * dt;
				if (par.hasFriction)
					par.vel *= mFriction;


				par.mtx.m[6] += par.vel.x * dt;
				par.mtx.m[7] += par.vel.y * dt;
				//par.mtx.scaleThis(par.scaling * dt, par.scaling * dt);
				par.alpha.x -= par.alpha.y * dt;

				// Store the particle data into 
				auto& texture = AssetManager::GetInstance()->GetAsset<Texture>(par.texture);
				for (unsigned int ii{}; ii < p.vtx.size(); ++ii)
				{
					vtxTexd2D& vtx = vtxBuf.vtx[it * 4 + ii];
					vtx.pos = par.mtx * p.vtx[ii];
					//tt.ids[0] = entity;		// Used for id-picking
					vtx.ids[1] = texture.t;
					vtx.color[0] = 1.f;
					vtx.color[1] = 1.f;
					vtx.color[2] = 1.f;
					vtx.color[3] = par.alpha.x;

					vtx.depth = depth;
					vtx.uv = texture.mtx * p.texCoord[ii];
				}
			}			
		}
	}

}
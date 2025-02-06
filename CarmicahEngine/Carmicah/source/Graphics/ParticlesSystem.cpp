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
	ParticlesSystem::vtxTexd2D ParticlesSystem::mClearData[100]{};

	void ParticlesSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<ParticleEmitter>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ParticlesSystem>(mSignature);
		BaseGraphicsSystem::Init(AssetManager::GetInstance()->enConfig.defaultShader);
	}

	// Uses basic.vert/frag
	// Unwrites id buffer
	void ParticlesSystem::Update()
	{
		float dt = static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
		// Generate particles
		for (auto& e : mEntitiesSet)
		{
			auto& emitter = ComponentManager::GetInstance()->GetComponent<ParticleEmitter>(e);

			if (emitter.HasEmitterQualities(ParticleEmitter::EMITTER_BURST))
			{
				emitter.optAliveTime -= dt;
				if (emitter.optAliveTime < 0)
					emitter.SetEmitterQualities(ParticleEmitter::EMITTER_ACTIVE, false);
			}
			if (!emitter.HasEmitterQualities(ParticleEmitter::EMITTER_ACTIVE))
				continue;

			emitter.timePassed += dt;
			float spawnTime = 1.f / static_cast<float>(emitter.spawnPerSec);
			if (emitter.timePassed > spawnTime)
			{
				float emitterSpeed = emitter.particleSpeed.length();
				float emitterDir = emitter.particleSpeed.angleRad();

				// World Space == array 0
				if (ComponentManager::GetInstance()->HasComponent<Transform>(e))
				{
					do
					{
						if (mParticles[0].size() + mParticles[1].size() >= mMaxParticles)
						{
							emitter.timePassed = 0.0f;
							break;
						}

						particle par;
						auto& tf = ComponentManager::GetInstance()->GetComponent<Transform>(e);

						par.texture = emitter.texture;
						par.timeLeft = emitter.particleLifeTime;
						par.mtx = tf.worldSpace;
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
					do
					{
						if (mParticles[0].size() + mParticles[1].size() >= mMaxParticles)
						{
							emitter.timePassed = 0.0f;
							break;
						}

						particle par;
						auto& tf = ComponentManager::GetInstance()->GetComponent<UITransform>(e);

						par.texture = emitter.texture;
						par.timeLeft = emitter.particleLifeTime;
						par.mtx.translateThis(tf.Pos()).scaleThis(tf.Scale());
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
		mParticlesData[0].clear();
		mParticlesData[0].reserve(mParticles[0].size() * 4);
		mParticlesData[1].clear();
		mParticlesData[1].reserve(mParticles[1].size() * 4);
		auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>("Square")};

		
		for (size_t i{}; i < 2; ++i)// WorldSpace, UISpace
		{
			size_t eraseCounter{}, arrSize{ mParticles[i].size() };
			float depth{};
			if(i == 0)
				depth = CalcDepth(mNearestDepth, BASE_LAYER);
			else if(i == 1)
				depth = CalcDepth(mNearestDepth, UI_LAYER);

			// Loops through all active particles and updates
			for (size_t it{}; it < mParticles[i].size();)
			{
				particle& par = mParticles[i][it];
				// Swap this with the last particle, continue (don't advance)
				if ((par.timeLeft -= dt) < 0.f)
				{
					std::swap(par, mParticles[i][arrSize - (++eraseCounter)]);
					if (eraseCounter == arrSize)
						break;
					continue;
				}
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
					vtxTexd2D vtx;
					vtx.pos = par.mtx * p.vtx[ii];
					//tt.ids[0] = entity;		// Used for id-picking
					vtx.ids[1] = texture.t;
					vtx.color[0] = 1.f;
					vtx.color[1] = 1.f;
					vtx.color[2] = 1.f;
					vtx.color[3] = par.alpha.x;

					vtx.depth = depth;
					vtx.uv = texture.mtx * p.texCoord[ii];
					mParticlesData[i].emplace_back(vtx);
				}
				++it;
			}


			// Remove non-active particles from the list
			if(eraseCounter > 0)
				mParticles[i].erase(mParticles[i].begin() + (arrSize - eraseCounter), mParticles[i].end());
			

			int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;

			// Batch push data
			if (i == 0)
			{
				while (mParticlesData[i].size() > mParticlesBufferSize[i])
				{
					GenBatch("Square", mParticleBufferID, true, false, true);
					mParticlesBufferSize[i] += static_cast<size_t>(mBatchSize);
				}
				RenderHelper::BufferID bufferID(AssetManager::GetInstance()->GetAsset<BasePrimitive>("Square").uid, mCurrShader, true, mParticleBufferID);
				if (RenderHelper::GetInstance()->mBufferMap.find(bufferID) != RenderHelper::GetInstance()->mBufferMap.end())
				{
					BatchBuffer& bb = RenderHelper::GetInstance()->mBufferMap.find(bufferID)->second;

					// Clear Data
					int bufferNum{};
					for (int numVtx{}; numVtx < mParticlesBufferSize[i]; numVtx += mBatchSize)
						glNamedBufferSubData(bb.buffer[bufferNum++].vbo, 0, sizeof(vtxTexd2D) * p.vtx.size() * mBatchSize, mClearData);
					// Write Data
					bufferNum = 0;
					for (int numVtx{ static_cast<int>(mParticlesData[i].size()) }; numVtx > 0; numVtx -= mBatchSize)
					{
						glNamedBufferSubData(bb.buffer[bufferNum++].vbo, 0, sizeof(vtxTexd2D) * p.vtx.size() * std::min(numVtx, mBatchSize), mParticlesData[i].data() + (mParticlesData[i].size() - numVtx));
					}
				}
			}
			else if (i == 1)
			{
				while (mParticlesData[i].size() > mParticlesBufferSize[i])
				{
					GenBatch("Square", mParticleBufferID, false, false, true);
					mParticlesBufferSize[i] += static_cast<size_t>(mBatchSize);
				}

				RenderHelper::BufferID bufferID(AssetManager::GetInstance()->GetAsset<BasePrimitive>("Square").uid, mCurrShader, false, mParticleBufferID);
				if (RenderHelper::GetInstance()->mBufferMap.find(bufferID) != RenderHelper::GetInstance()->mBufferMap.end())
				{
					BatchBuffer& bb = RenderHelper::GetInstance()->mBufferMap.find(bufferID)->second;
					// Clear Data
					int bufferNum{};
					for (int numVtx{}; numVtx < mParticlesBufferSize[i]; numVtx += mBatchSize)
						glNamedBufferSubData(bb.buffer[bufferNum++].vbo, 0, sizeof(vtxTexd2D) * p.vtx.size() * mBatchSize, mClearData);
					bufferNum = 0;
					for (int numVtx{ static_cast<int>(mParticlesData[i].size()) }; numVtx > 0; numVtx -= mBatchSize)
					{
						glNamedBufferSubData(bb.buffer[bufferNum++].vbo, 0, sizeof(vtxTexd2D) * p.vtx.size() * std::min(numVtx, mBatchSize), mParticlesData[i].data() + (mParticlesData[i].size() - numVtx));
					}
				}
			}
		}
	}

}
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
		auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>("debug");
		mCurrShader = shdrRef.s;
	/*	auto shdrRef = AssetManager::GetInstance()->mShaderPgms.find("debug");
		if (shdrRef != AssetManager::GetInstance()->mShaderPgms.end())
			mCurrShader = shdrRef->second;
		else
			CM_CORE_ERROR("RigidbodyRendererSystem failed to load Shader");*/
	}

	void RigidbodyRendererSystem::Render(Entity& cam)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		auto& tryPrimitive{ AssetManager::GetInstance()->GetAsset<Primitive>(modelName) };
		Primitive* p;
		p = &tryPrimitive;
	/*	auto tryPrimitive{ AssetManager::GetInstance()->mPrimitiveMaps.find(modelName) };
		Primitive* p;
		if (tryPrimitive == AssetManager::GetInstance()->mPrimitiveMaps.end())
		{
			std::stringstream ss;
			ss << "Renderer Model not found: " << modelName << std::endl;
			CM_CORE_ERROR(ss.str());
			return;
		}
		else
			p = &tryPrimitive->second;*/

		auto& camera = ComponentManager::GetInstance()->GetComponent<Transform>(cam);

		for (auto& entity : mEntitiesSet)
		{
			auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity);
			if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
				fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
				continue;

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			Matrix3x3<float> trans{}, rotMtx{};
			Mtx33Identity(trans);
			Mtx33Identity(rotMtx);

			// Get rotation
			float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
			// Get scale multi
			float biggerVel = fmaxf(fabs(rigidbody.velocity.x), fabs(rigidbody.velocity.y));

			Mtx33Translate(trans, transform.xPos, transform.yPos);
			Mtx33RotRad(trans, rot);
			trans *= rotMtx;
			Mtx33Scale(trans, biggerVel, biggerVel);
			trans = camera.camSpace * trans;


			Matrix3x3<float> invMat{};
			Mtx33Transpose(invMat, trans);


			GLint uniformLoc;
			if (uniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, invMat.m);

			glBindVertexArray(p->vaoid);
			switch (p->drawMode)
			{
			case GL_LINE_LOOP:
				glLineWidth(2.f);
				glDrawArrays(GL_LINE_LOOP, 0, p->drawCnt);
				break;
			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
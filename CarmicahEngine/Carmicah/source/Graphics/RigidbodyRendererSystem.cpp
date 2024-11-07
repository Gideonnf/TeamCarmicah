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
		BaseGraphicsSystem::Init(true);
	}

	void RigidbodyRendererSystem::Render(Entity& cam)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);
		auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(modelName) };

		{
			auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			Mtx3x3f camSpace{};
			camSpace.scaleThis(camTrans.scale.x, camTrans.scale.y).rotDegThis(-camTrans.rot).translateThis(-camTrans.pos.x, -camTrans.pos.y);
			GLint uniformLoc{};
			if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity);
			if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
				fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
				continue;

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			Matrix3x3<float> trans{};

			// Get rotation
			float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
			// Get scale multi
			//float biggerVel = fmaxf(fabs(rigidbody.velocity.x), fabs(rigidbody.velocity.y));
			//trans.translateThis(transform.pos.x, transform.pos.y).scaleThis(biggerVel, biggerVel).rotRadThis(rot);

			trans.translateThis(transform.pos.x, transform.pos.y).rotRadThis(rot);

			GLint uniformLoc;
			if (UniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, trans.m);

			if (UniformExists(mCurrShader, "uDepth", uniformLoc))
				glUniform1f(uniformLoc, CalcDepth(transform.depth, RENDER_LAYERS::DEBUG_LAYER));

			//RenderPrimitive(p);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Graphics/RigidbodyRendererSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"


namespace Carmicah
{
	void RigidbodyRendererSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<RigidBody>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<RigidbodyRendererSystem>(mSignature);

		auto shdrRef = AssetManager::GetInstance()->mShaderPgms.find("debug");
		if (shdrRef != AssetManager::GetInstance()->mShaderPgms.end())
			mCurrShader = shdrRef->second;
	}

	void RigidbodyRendererSystem::Render(Entity& cam)
	{
		glUseProgram(mCurrShader);
		Primitive& p{ AssetManager::GetInstance()->mPrimitiveMaps[modelName] };
		auto& camera = ComponentManager::GetInstance()->GetComponent<Transform>(cam);

		for (auto& entity : mEntitiesSet)
		{
			auto& rigidbody = ComponentManager::GetInstance()->GetComponent<RigidBody>(entity);
			if (fabs(rigidbody.velocity.x) < std::numeric_limits<float>::epsilon() &&
				fabs(rigidbody.velocity.y) < std::numeric_limits<float>::epsilon())
				continue;


			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			glm::mat3 trans{ 1 };

			// Get rotation
			float rot = std::atan2f(rigidbody.velocity.y, rigidbody.velocity.x);
			// Get scale multi
			float biggerVel = fmaxf(rigidbody.velocity.x, rigidbody.velocity.y);

			trans = glm::translate(trans, glm::vec2(transform.xPos, transform.yPos));
			trans = glm::rotate(trans, rot);
			trans = glm::scale(trans, glm::vec2(biggerVel, biggerVel));
			trans = camera.camSpace * trans;

			GLint uniform_var_loc0 = glGetUniformLocation(mCurrShader, "uModel_to_NDC");
			if (uniform_var_loc0 >= 0)
			{
				glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE,
					glm::value_ptr(trans));
			}
			else
			{
				std::cout << "Uniform variable dosen't exist!!!\n";
				continue;
			}

			glBindVertexArray(p.vaoid);
			switch (p.drawMode)
			{
			case GL_LINE_LOOP:
				glLineWidth(2.f);
				glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
				break;
			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}
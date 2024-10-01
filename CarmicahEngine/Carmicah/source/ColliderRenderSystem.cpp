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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Systems/ColliderRenderSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace Carmicah
{
	void ColliderRenderSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ColliderRenderSystem>(mSignature);

		auto shdrRef = AssetManager::GetInstance()->mShaderPgms.find("debug");
		if (shdrRef != AssetManager::GetInstance()->mShaderPgms.end())
			mCurrShader = shdrRef->second;
	}

	void ColliderRenderSystem::Render(Entity& cam)
	{
		glUseProgram(mCurrShader);

		for (auto entity : mEntitiesSet)
		{
			auto& camera = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			Primitive p{ AssetManager::GetInstance()->mPrimitiveMaps[collider.shape] };

			glm::mat3 trans{1};
			trans = glm::translate(trans, glm::vec2((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f));
			trans = glm::scale(trans, glm::vec2(collider.max.x - collider.min.x, collider.max.y - collider.min.y));
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
				std::exit(EXIT_FAILURE);
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
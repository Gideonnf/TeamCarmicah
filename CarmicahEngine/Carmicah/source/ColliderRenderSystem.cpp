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

namespace
{
	GLuint currShader{};
}

namespace Carmicah
{
	void ColliderRenderSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ColliderRenderSystem>(mSignature);

		auto shdrRef = AssetManager::GetInstance()->shaderPgms.find("debug");
		if (shdrRef != AssetManager::GetInstance()->shaderPgms.end())
			currShader = shdrRef->second;
	}

	void ColliderRenderSystem::Render(Entity& cam)
	{
		glUseProgram(currShader);

		for (auto entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			Primitive p{ AssetManager::GetInstance()->primitiveMaps[collider.shape] };

			GLint uniform_var_loc0 = glGetUniformLocation(currShader, "uModel_to_NDC");
			if (uniform_var_loc0 >= 0)
			{
				glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE,
					glm::value_ptr(transform.camSpace));
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
#include "pch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Systems/GraphicsSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace Carmicah
{
	void GraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);

		auto& shdrRef = AssetManager::GetInstance()->shaderPgms.find(shaderName);
		if (shdrRef != AssetManager::GetInstance()->shaderPgms.end())
			currShader = shdrRef->second;
	}

	void GraphicsSystem::Render(Entity& cam)
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(currShader);

		// Handle Camera Transform
		auto& currCam = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		currCam.isUpdated = true;
		if (currCam.isUpdated)
		{
			//mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
			currCam.camSpace = glm::mat3(1);
			auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			glm::vec2 camEye = glm::vec2{ camTrans.xPos, camTrans.yPos };
			glm::vec2 camScale = glm::vec2{ camTrans.xScale, camTrans.yScale };
			currCam.camSpace = glm::translate(currCam.camSpace, camEye);
			currCam.camSpace = glm::scale(currCam.camSpace, camScale);
		}

		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			Renderer& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
			Primitive p{ AssetManager::GetInstance()->primitiveMaps[renderer.model] };

			// Handle Entities transform
			if (transform.isUpdated)
			{
				transform.worldSpace = glm::mat3(1.f);
				transform.worldSpace = glm::translate(transform.worldSpace, glm::vec2{ transform.xPos, transform.yPos});
				transform.worldSpace = glm::rotate(transform.worldSpace, glm::radians(transform.rot));
				transform.worldSpace = glm::scale(transform.worldSpace, glm::vec2{ transform.xScale, transform.yScale});
				transform.camSpace = currCam.camSpace * transform.worldSpace;
			}
			else if (currCam.isUpdated)
				transform.camSpace = currCam.camSpace * transform.worldSpace;


			GLint uniform_var_loc0 = glGetUniformLocation(currShader, "uModel_to_NDC");
			if (uniform_var_loc0 >= 0)
				glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE, glm::value_ptr(transform.camSpace));
			else
			{
				std::cout << "Uniform variable dosen't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

			uniform_var_loc0 = glGetUniformLocation(currShader, "uTex2d");
			if (uniform_var_loc0 >= 0)
				glUniform1i(uniform_var_loc0, 0);
			else
			{
				std::cout << "Uniform variable dosen't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

			glBindVertexArray(p.vaoid);
			glBindTextureUnit(0, AssetManager::GetInstance()->textureMaps[renderer.texture]);
			switch (p.drawMode)
			{
			case GL_LINE_LOOP:
				glLineWidth(2.f);
				glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
				break;
			case GL_TRIANGLES:
				glDrawElements(GL_TRIANGLES, p.drawCnt, GL_UNSIGNED_SHORT, NULL);
				break;
			case GL_TRIANGLE_FAN:
				glDrawArrays(GL_TRIANGLE_FAN, 0, p.drawCnt);
				break;
			}
		}

		currCam.isUpdated = false;

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}
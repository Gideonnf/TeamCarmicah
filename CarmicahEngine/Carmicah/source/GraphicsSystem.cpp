#include "pch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <ECS/ECSTypes.h>
#include "Systems/GraphicsSystem.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "AssetManager.h"

namespace
{
	struct Camera 
	{
		glm::vec2 eye{};					// Location of the camera
		glm::vec2 scale{};					// Converts NDC based on window size
		glm::mat3 mtx{};					// The computed camera matrix
	};
	Camera mainCam{};
	GLuint currShader{};
}

namespace Carmicah
{
	void GraphicsSystem::Init(const unsigned int& width, const unsigned int& height)
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);

		mainCam.scale = glm::vec2{ 1.0 / static_cast<float>(width), 1.0 / static_cast<float>(height) };
		mainCam.mtx = glm::mat3(1);
		mainCam.mtx = glm::translate(mainCam.mtx, mainCam.eye);
		mainCam.mtx = glm::scale(mainCam.mtx, mainCam.scale);

		auto shdrRef = AssetManager::GetInstance()->shaderPgms.find("basic");
		if (shdrRef != AssetManager::GetInstance()->shaderPgms.end())
			currShader = shdrRef->second;
	}

	void GraphicsSystem::Update()
	{


	}

	void GraphicsSystem::Render()
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(currShader);

		int i = 0;
		Primitive p{};

		for (auto entity : mEntitiesSet)
		{
			if (i++ == 0)
				p = AssetManager::GetInstance()->primitiveMaps["Square"];
			else
				p = AssetManager::GetInstance()->primitiveMaps["Circle"];

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			glm::mat3 mat = glm::mat3(1.f);
			mat = glm::translate(mat, glm::vec2{ transform.xPos, transform.yPos});
			mat = glm::rotate(mat, glm::radians(transform.rot));
			mat = glm::scale(mat, glm::vec2{ transform.xScale, transform.yScale});
			mat = mainCam.mtx * mat;

			GLint uniform_var_loc0 = glGetUniformLocation(currShader, "uModel_to_NDC");
			if (uniform_var_loc0 >= 0)
			{
				glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE,
					glm::value_ptr(mat));
			}
			else
			{
				std::cout << "Uniform variable dosen't exist!!!\n";
				std::exit(EXIT_FAILURE);
			}

			uniform_var_loc0 = glGetUniformLocation(currShader, "isDebug");
			if (uniform_var_loc0 >= 0)
				glUniform1i(uniform_var_loc0, p.drawMode == GL_LINE_LOOP);

			GLint uniform_var_loc1 = glGetUniformLocation(currShader, "uTex2d");
			if (uniform_var_loc1 >= 0)
				glUniform1i(uniform_var_loc1, 0);

			glBindVertexArray(p.vaoid);
			glBindTextureUnit(0, AssetManager::GetInstance()->textureMaps["Duck"]);
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

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void GraphicsSystem::Exit()
	{

	}
}
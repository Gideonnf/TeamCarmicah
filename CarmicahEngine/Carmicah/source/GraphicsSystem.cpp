#include "pch.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <ECS/ECSTypes.h>
#include "Systems/GraphicsSystem.h"
#include "Components/Transform.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

namespace
{
	GLuint vboid{};
	GLuint vaoid{};

	GLuint shaderPgm;
}

namespace Carmicah
{
	void setUpQuad()
	{
		std::ifstream vertShaderFile("../Assets/Shaders/basic.vert", std::ifstream::in | std::ios::binary);
		if (!vertShaderFile)
		{
			std::cerr << "Unable to open Vertex Shader File";
			return;
		}
		std::string vertShaderSource;
		vertShaderFile.seekg(0, std::ios::end);
		vertShaderSource.resize(vertShaderFile.tellg());
		vertShaderFile.seekg(0, std::ios::beg);
		vertShaderFile.read(&vertShaderSource[0], vertShaderSource.size());
		vertShaderFile.close();
		GLchar const* vert_shader_code[] = { vertShaderSource.c_str() };
		
		std::ifstream fragShaderFile("../Assets/Shaders/basic.frag", std::ifstream::in | std::ios::binary);
		if (!fragShaderFile)
		{
			std::cerr << "Unable to open Fragment Shader File";
			return;
		}
		std::string fragShaderSource;
		fragShaderFile.seekg(0, std::ios::end);
		fragShaderSource.resize(fragShaderFile.tellg());
		fragShaderFile.seekg(0, std::ios::beg);
		fragShaderFile.read(&fragShaderSource[0], fragShaderSource.size());
		fragShaderFile.close();
		GLchar const* frag_shader_code[] = { fragShaderSource.c_str() };


		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, vert_shader_code, nullptr);
		glCompileShader(vertShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
			std::cerr << "Unable to compile vertex shader:" << infoLog << std::endl;
			return;
		}
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, frag_shader_code, nullptr);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			std::cerr << "Unable to compile fragment shader:" << infoLog << std::endl;
			return;
		}
		shaderPgm = glCreateProgram();
		glAttachShader(shaderPgm, vertShader);
		glAttachShader(shaderPgm, fragShader);
		glLinkProgram(shaderPgm);

		glGetProgramiv(shaderPgm, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderPgm, 512, nullptr, infoLog);
			std::cerr << "Link / Compile Failed:" << infoLog << std::endl;
			return;
		}
		glValidateProgram(shaderPgm);
		glGetProgramiv(shaderPgm, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderPgm, 512, nullptr, infoLog);
			std::cerr << "Validate Failed:" << infoLog << std::endl;
			return;
		}

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		float vtx[] =
		{
			0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
			0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f
		};

		glCreateBuffers(1, &vboid);
		glNamedBufferStorage(vboid, 4 * (2 + 3 + 2) * 4, vtx, GL_DYNAMIC_STORAGE_BIT);
		//glNamedBufferSubData(vbo_hdl, color_data_offset,
		//	color_data_size, initQuadColor.data());
		glCreateVertexArrays(1, &vaoid);
		glEnableVertexArrayAttrib(vaoid, 0); // VAO's vertex attribute index is 0 (vert)

		glVertexArrayVertexBuffer(vaoid, 0, // vertex buffer binding point
			vboid, 0, 4 * 2);
		glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);

		glVertexArrayAttribBinding(vaoid, 0, 0);

		// Color
		glEnableVertexArrayAttrib(vaoid, 1);
		glVertexArrayVertexBuffer(vaoid, 1, vboid, 4 * 2 * 4, 4 * 3);
		glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vaoid, 1, 1);

		// Texture
		glEnableVertexArrayAttrib(vaoid, 1);
		glVertexArrayVertexBuffer(vaoid, 2, vboid, 4 * (2 + 3) * 4, 4 * 2);
		glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vaoid, 2, 2);

		GLushort idx[]
		{
			0, 1, 2,
			2, 3, 0
		};
		
		GLuint eboid;
		glCreateBuffers(1, &eboid);
		glNamedBufferStorage(eboid, sizeof(GLushort) * 6, idx, GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(vaoid, eboid);

		glBindVertexArray(0);
	}

	void GraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);

		setUpQuad();
	}

	void GraphicsSystem::Update()
	{
	}

	void GraphicsSystem::Render(GLFWwindow*& window)
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderPgm);
		glBindVertexArray(vaoid);

		const glm::mat3 aObj{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.5f, 0.5f, 1.f
		};
		const glm::mat3 bObj{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			-0.7f, 0.f, 1.f
		};

		GLint uniform_var_loc0 = glGetUniformLocation(shaderPgm, "uModel_to_NDC");
		if (uniform_var_loc0 >= 0)
		{
			glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE,
				glm::value_ptr(aObj));
		}
		else
		{
			std::cout << "Uniform variable dosen't exist!!!\n";
			std::exit(EXIT_FAILURE);
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
		glUniformMatrix3fv(uniform_var_loc0, 1, GL_FALSE, glm::value_ptr(bObj));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);


		glBindVertexArray(0);
		glUseProgram(0);
		//for (auto entity : mEntitiesSet)
		//{
		//	auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
		//	transform.yPos += 1;
		//}
		glfwSwapBuffers(window);
	}

	void GraphicsSystem::Exit()
	{

	}
}
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

namespace
{
	// Structs
	struct Primitive
	{
		GLuint vaoid{};
		GLuint vboid{};
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct Camera 
	{
		glm::vec2 eye{};					// Location of the camera
		glm::mat3 mtx{};					// The computed camera matrix
	};

	// Data
	std::unordered_map<std::string, GLuint> shaderPgms{};
	std::unordered_map<std::string, Primitive> primitiveMaps;
	std::unordered_map<std::string, GLuint> textureMaps;
	Camera mainCam;
	GLuint currShader;

	// Load Functions
	GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile)
	{
		// Shader Exists
		auto foundShader = shaderPgms.find(shaderName);
		if (foundShader != shaderPgms.end())
		{
			std::cerr << "Shader:" << shaderName << " Already Exists";
			return foundShader->second;
		}

		std::ifstream vertShaderFile(vertFile, std::ios::binary);
		if (!vertShaderFile)
		{
			std::cerr << "Unable to open Vertex Shader File";
			return -1;
		}
		std::string vertShaderSource;
		vertShaderFile.seekg(0, std::ios::end);
		vertShaderSource.resize(vertShaderFile.tellg());
		vertShaderFile.seekg(0, std::ios::beg);
		vertShaderFile.read(&vertShaderSource[0], vertShaderSource.size());
		vertShaderFile.close();
		GLchar const* vert_shader_code[] = { vertShaderSource.c_str() };

		std::ifstream fragShaderFile(fragFile, std::ios::binary);
		if (!fragShaderFile)
		{
			std::cerr << "Unable to open Fragment Shader File";
			return -1;
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
			return -1;
		}
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, frag_shader_code, nullptr);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			std::cerr << "Unable to compile fragment shader:" << infoLog << std::endl;
			return -1;
		}
		GLuint shader = glCreateProgram();
		glAttachShader(shader, vertShader);
		glAttachShader(shader, fragShader);
		glLinkProgram(shader);

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Link / Compile Failed:" << infoLog << std::endl;
			return -1;
		}
		glValidateProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Validate Failed:" << infoLog << std::endl;
			return -1;
		}
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		shaderPgms.insert(std::make_pair(shaderName, shader));
		return shader;
	}

	/*
	Assumes file follows this order
	{
		GL_TRIANGLE NUM_VERTS NUM_IDX
		v0.x v0.y
		...
		idx1
		idx2
		...
	}
	*/
	void LoadObject(const std::string& objName, const std::string& modelFile)
	{
		auto foundObj = primitiveMaps.find(objName);
		if (foundObj != primitiveMaps.end())
		{
			std::cerr << "Object:" << objName << " Already Exists";
			return;
		}

		std::ifstream ifs(modelFile, std::ios::binary);
		if (!ifs)
		{
			std::cerr << "Unable to open Obj:" << modelFile;
			return;
		}
		Primitive p;// GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
		// No error checking, just fastest way to read data
		unsigned int numVert;
		ifs >> p.drawMode >> numVert >> p.drawCnt;
		if (p.drawMode == 0)
		{
			std::cerr << "Error reading obj file";
			return;
		}

		std::vector<glm::vec2> vtx;
		std::vector<glm::vec2> texCoord;
		std::vector<GLushort> idx;

		vtx.reserve(numVert);
		texCoord.reserve(numVert);
		idx.reserve(p.drawCnt);
		float v1, v2;
		for (unsigned int i{}; i < numVert; ++i)
		{
			ifs >> v1 >> v2;
			vtx.emplace_back(glm::vec2{ v1, v2 });
		}
		for (unsigned int i{}; i < numVert; ++i)
		{
			ifs >> v1 >> v2;
			texCoord.emplace_back(glm::vec2{ v1, v2 });
		}
		// Only save index when following Triangle method
		if (p.drawMode == GL_TRIANGLES)
		{
			GLshort i1;
			for (unsigned int i{}; i < p.drawCnt; ++i)
			{
				ifs >> i1;
				idx.emplace_back(i1);
			}
		}
		ifs.close();
		
		unsigned int sizeofVtxArray = numVert * sizeof(glm::vec2);

		glCreateBuffers(1, &p.vboid);
		glNamedBufferStorage(p.vboid, sizeofVtxArray * 2, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(p.vboid, 0, sizeofVtxArray, vtx.data());
		glNamedBufferSubData(p.vboid, sizeofVtxArray, sizeofVtxArray, texCoord.data());
		
		// Position
		glCreateVertexArrays(1, &p.vaoid);
		glEnableVertexArrayAttrib(p.vaoid, 0); // VAO's vertex attribute index is 0 (vert)
		glVertexArrayVertexBuffer(p.vaoid, 0, // vertex buffer binding point
			p.vboid, 0, sizeof(glm::vec2));
		glVertexArrayAttribFormat(p.vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(p.vaoid, 0, 0);

		// Texture
		glEnableVertexArrayAttrib(p.vaoid, 1);
		glVertexArrayVertexBuffer(p.vaoid, 1, p.vboid, sizeofVtxArray, sizeof(glm::vec2));
		glVertexArrayAttribFormat(p.vaoid, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(p.vaoid, 1, 1);

		// Index - only done for following triangle method(?)
		if (p.drawMode == GL_TRIANGLES)
		{
			GLuint eboid;
			glCreateBuffers(1, &eboid);
			glNamedBufferStorage(eboid, sizeof(GLushort) * p.drawCnt, idx.data(), GL_DYNAMIC_STORAGE_BIT);
			glVertexArrayElementBuffer(p.vaoid, eboid);
		}
		//glBindVertexArray(0);

		primitiveMaps.insert(std::make_pair(objName, p));
	}

	/*
	Assumes file follows this order
	{
		NUM_VERTS DRAW_COUNT
		v0.x v0.y
		...
	}
	*/
	void LoadDebugObject(const std::string& objName, const std::string& modelFile)
	{
		auto foundObj = primitiveMaps.find(objName);
		if (foundObj != primitiveMaps.end())
		{
			std::cerr << "Object:" << objName << " Already Exists";
			return;
		}

		std::ifstream ifs(modelFile, std::ios::binary);
		if (!ifs)
		{
			std::cerr << "Unable to open Obj:" << modelFile;
			return;
		}
		Primitive p;
		p.drawMode = GL_LINE_LOOP;
		unsigned int numVert;
		ifs >> numVert >> p.drawCnt;
		if (numVert == 0)
		{
			std::cerr << "Error reading debug obj file";
			return;
		}

		std::vector<glm::vec2> vtx;

		vtx.reserve(numVert);
		float v1, v2;
		for (unsigned int i{}; i < numVert; ++i)
		{
			ifs >> v1 >> v2;
			vtx.emplace_back(glm::vec2{ v1, v2 });
		}
		ifs.close();

		unsigned int sizeofVtxArray = numVert * sizeof(glm::vec2);

		glCreateBuffers(1, &p.vboid);
		glNamedBufferStorage(p.vboid, sizeofVtxArray, vtx.data(), GL_DYNAMIC_STORAGE_BIT);

		// Position
		glCreateVertexArrays(1, &p.vaoid);
		glEnableVertexArrayAttrib(p.vaoid, 0); // VAO's vertex attribute index is 0 (vert)
		glVertexArrayVertexBuffer(p.vaoid, 0, // vertex buffer binding point
			p.vboid, 0, sizeof(glm::vec2));
		glVertexArrayAttribFormat(p.vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(p.vaoid, 0, 0);

		primitiveMaps.insert(std::make_pair(objName, p));
	}


	void LoadTexture(const std::string& textureName, const std::string& textureFile, const GLuint& width, const GLuint& height, const GLuint& bpt)
	{
		auto foundTexture = textureMaps.find(textureName);
		if (foundTexture != textureMaps.end())
		{
			std::cerr << "Texture:" << textureName << " Already Exists";
			return;
		}

		GLuint texture;
		std::ifstream texIF{ textureFile, std::ios::binary };
		if (!texIF)
		{
			std::cerr << "Unable to open texture file\n";
			exit(EXIT_FAILURE);
		}
		char* ptr_texels = new char[width * height * bpt];
		texIF.read(ptr_texels, width * height * bpt);
		texIF.close();
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);
		delete[] ptr_texels;
		glTextureParameterf(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameterf(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, ); if width * bpt is not multiple of 4
		textureMaps.insert(std::make_pair(textureName, texture));
	}

	void ExportCircle(int numSlices, const std::string& modelFile)
	{
		const double M_PI = 3.14159265358979323846264;
		std::ofstream ofs(modelFile, std::ios::binary);
		if (ofs)
		{
			double angleInc{ 2.0 / static_cast<double>(numSlices) * M_PI };
			ofs << GL_TRIANGLE_FAN << ' ' << numSlices + 2 << ' ' << numSlices + 2 << '\n';
			ofs << "0 0\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<double>(i) * angleInc)) << ' ' << cosf((static_cast<double>(i) * angleInc)) << '\n';
			ofs << "0.5 0.5\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<double>(i) * angleInc)) * 0.5 + 0.5 << ' ' << cosf((static_cast<double>(i) * angleInc)) * 0.5 + 0.5 << '\n';
			ofs.close();
		}
	}
}

namespace Carmicah
{
	void GraphicsSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<GraphicsSystem>(mSignature);

		currShader = LoadShader("main", "../Assets/Shaders/basic.vert", "../Assets/Shaders/basic.frag");
		LoadObject("obj1", "../Assets/Meshes/Circle.o");
		LoadDebugObject("obj2", "../Assets/Meshes/Circle.do");
		LoadTexture("Duck", "../Assets/Images/duck-rgba-256.tex", 256, 256, 4);
	}

	void GraphicsSystem::Update()
	{


	}

	void GraphicsSystem::Render(GLFWwindow*& window)
	{
		glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(currShader);

		int i = 0;
		Primitive p{};

		for (auto entity : mEntitiesSet)
		{
			if (i++ == 0)
				p = primitiveMaps["obj1"];
			else
				p = primitiveMaps["obj2"];

			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			glm::mat3 mat = glm::mat3(1.f);
			mat = glm::translate(mat, glm::vec2{ transform.xPos, transform.yPos});
			mat = glm::rotate(mat, glm::radians(transform.rot));
			mat = glm::scale(mat, glm::vec2{ transform.xScale * 0.5f, transform.yScale * 0.5f});

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
			glBindTextureUnit(0, textureMaps["Duck"]);
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
		glfwSwapBuffers(window);
	}

	void GraphicsSystem::Exit()
	{
		for(const auto& i : textureMaps)
			glDeleteTextures(1, &i.second);
		for (const auto& i : primitiveMaps)
		{
			glDeleteVertexArrays(1, &i.second.vaoid);
			glDeleteBuffers(1, &i.second.vboid);
		}
		for (const auto& i : shaderPgms)
			glDeleteProgram(i.second);

		textureMaps.clear();
		primitiveMaps.clear();
		shaderPgms.clear();
	}
}
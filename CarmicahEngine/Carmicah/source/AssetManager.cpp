#include "pch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssetManager.h"
#include "Systems/SoundSystem.h"

namespace Carmicah
{
	void AssetManager::LoadAll(const char* assetPath)
	{
		std::filesystem::path directoryPath = assetPath;

		textureMaps.insert(std::make_pair("", 0)); // Sets No Texture

		InitSound();

		if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath))
		{
			for (const auto& subFile : std::filesystem::directory_iterator(directoryPath))
			{
				if (std::filesystem::exists(subFile) && std::filesystem::is_directory(subFile))
				{
					std::string folderName = subFile.path().filename().string();
					for (const auto& entry : std::filesystem::directory_iterator(subFile))
					{
						std::string fileName = entry.path().stem().string();
						if (folderName == "Audio")
						{
							LoadSound(fileName, entry.path().string(), false);
						}
						else if (folderName == "BGM")
						{
							LoadSound(fileName, entry.path().string(), true);
						}
						else if (folderName == "Images")
						{
							LoadTexture(fileName, entry.path().string(), 256, 256, 4);
						}
						else if (folderName == "Meshes")
						{
							std::string fileExt = entry.path().extension().string();
							if (fileExt == ".o")
							{
								LoadObject(fileName, entry.path().string());
							}
							else if (fileExt == ".do")
							{
								LoadDebugObject("Debug" + fileName, entry.path().string());
							}
						}
						else if (folderName == "Scene")
						{
						}
						else if (folderName == "Shaders")
						{

						}
					}
				}
			}
		}
		LoadShader("basic", "../Assets/Shaders/basic.vert", "../Assets/Shaders/basic.frag");
		LoadShader("debug", "../Assets/Shaders/debug.vert", "../Assets/Shaders/debug.frag");
	}

	void AssetManager::UnloadAll()
	{
		// Unload Graphics
		for (const auto& i : textureMaps)
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

		// Unload Sound
		for (auto& sound : soundMap)
			sound.second.sound->release();
		if (soundSystem != NULL)
			soundSystem->release();
	}
	//bool AssetManager::TryGetPrimitive(Primitive& p, const std::string& s)
	//{
	//	auto& temp = primitiveMaps.find(s);
	//	if (temp != primitiveMaps.end())
	//	{
	//		p = temp->second;
	//		return true;
	//	}
	//	return false;
	//}
	// Load Functions
	GLuint AssetManager::LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile)
	{
		// Shader Exists
		auto& foundShader = shaderPgms.find(shaderName);
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
	void AssetManager::LoadObject(const std::string& objName, const std::string& modelFile)
	{
		auto& foundObj = primitiveMaps.find(objName);
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
	void AssetManager::LoadDebugObject(const std::string& objName, const std::string& modelFile)
	{
		auto& foundObj = primitiveMaps.find(objName);
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

	void AssetManager::LoadTexture(const std::string& textureName, const std::string& textureFile, const GLuint& width, const GLuint& height, const GLuint& bpt)
	{
		auto& foundTexture = textureMaps.find(textureName);
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

	void AssetManager::ExportCircle(int numSlices, const std::string& modelFile)
	{
		const float M_PI = 3.14159265358979323846264;
		std::ofstream ofs(modelFile, std::ios::binary);
		if (ofs)
		{
			float angleInc{ 2.0f / static_cast<float>(numSlices) * M_PI };
			ofs << GL_TRIANGLE_FAN << ' ' << numSlices + 2 << ' ' << numSlices + 2 << '\n';
			ofs << "0 0\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<float>(i) * angleInc)) << ' ' << cosf((static_cast<float>(i) * angleInc)) << '\n';
			ofs << "0.5 0.5\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<float>(i) * angleInc)) * 0.5f + 0.5f << ' ' << cosf((static_cast<float>(i) * angleInc)) * 0.5f + 0.5f << '\n';
			ofs.close();
		}
	}

	void AssetManager::InitSound()
	{
		if (FMOD::System_Create(&soundSystem) != FMOD_OK)
			return;
		soundSystem->init(maxChannels, FMOD_INIT_NORMAL, NULL);
	}

	void AssetManager::LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop)
	{
		auto& sound = soundMap.find(soundName);
		if (sound != soundMap.end())
		{
			std::cerr << "Sound:" << soundName << " Already Exists";
			return;
		}

		FMOD_MODE eMode = FMOD_DEFAULT;
		Audio audio{};
		soundSystem->createSound(soundFile.c_str(), eMode, nullptr, &audio.sound);
		if (audio.sound)
		{
			audio.isLoop = b_isLoop;
			soundMap.insert(std::make_pair(soundName, audio));
			if (b_isLoop)
			{
				audio.sound->setMode(FMOD_LOOP_NORMAL);
				audio.sound->setLoopCount(-1);
			}
		}
	}

}
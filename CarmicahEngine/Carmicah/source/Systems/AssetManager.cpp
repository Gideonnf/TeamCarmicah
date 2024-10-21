/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetManager.cpp

 author:		Won Yu Xuan Rainne(77.5%)
 co-author(s):	Gideon Francis(15%)
				YANG YUJIE(7.5%)

 email:			won.m@digipen.edu

 brief:			Asset Manager checks through the folders and files inside Asset folder and loads them.
				It handles Assets such as scene data, graphics, audio, and prefabs

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "AssetManager.h"
#include "Systems/SoundSystem.h"
#include "log.h"
#include "Systems/SerializerSystem.h"
namespace Carmicah
{
	void AssetManager::LoadConfig(const std::string& configPath)
	{
		std::filesystem::path directoryPath = configPath;
		if (std::filesystem::exists(directoryPath))
		{
			enConfig = Serializer.LoadConfig(directoryPath.string());
		}
	}

	void AssetManager::LoadAll(const char* assetPath)
	{
		std::filesystem::path directoryPath = assetPath;

		mTextureMaps.insert(std::make_pair("", Texture{})); // Sets No Texture

		InitSound();
		InitFontType();

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
							LoadSound(fileName, entry.path().string(), false, 1.0f); 
						}
						else if (folderName == "BGM")
						{
							LoadSound(fileName, entry.path().string(), true, 1.0f);  
						}
						else if (folderName == "Images")
						{
							std::string fileExt = entry.path().extension().string();

							if (fileExt == ".png")
							{
								const auto testOtherFile = subFile.path() / (fileName + std::string(".ani"));
								LoadTexture(fileName, entry.path().string(), testOtherFile.string());
							}
						}
						else if (folderName == "Fonts")
						{
							LoadFont(fileName, entry.path().string(), enConfig.fontSize);
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
						//	std::cout << entry.path().string() << std::endl;
							//std::cout << fileName << std::endl;
							mSceneFiles.insert(std::make_pair(fileName, entry.path().string()));
						}
						else if (folderName == "Shaders")
						{
							std::string fileExt = entry.path().extension().string();
							if (fileExt == ".vert")
							{
								const auto testOtherFile = subFile.path() / (fileName + std::string(".frag"));
								if(std::filesystem::exists(testOtherFile))
								{
									LoadShader(fileName, entry.path().string(), testOtherFile.string());
								}
							}
						}
						else if (folderName == "Prefabs")
						{
							Prefab goPrefab = Serializer.DeserializePrefab(entry.path().string());
							mPrefabFiles.insert(std::make_pair(fileName, goPrefab));
						}
					}
				}
			}
		}
	
		//enConfig = Serializer.LoadConfig(directoryPath)
	}

	void AssetManager::UnloadAll()
	{
		// Unload Graphics
		for (const auto& i : mTextureMaps)
			glDeleteTextures(1, &i.second.t);
		for (const auto& i : mPrimitiveMaps)
		{
			glDeleteVertexArrays(1, &i.second.vaoid);
			glDeleteBuffers(1, &i.second.vboid);
		}
		for (const auto& i : mShaderPgms)
			glDeleteProgram(i.second);
		mTextureMaps.clear();
		mPrimitiveMaps.clear();
		mShaderPgms.clear();
		FT_Done_FreeType(mFTLib);

		// Unload Sound
		for (auto& sound : mSoundMap)
			sound.second.sound->release();
		if (mSoundSystem != NULL)
			mSoundSystem->release();
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
		auto foundShader = mShaderPgms.find(shaderName);
		if (foundShader != mShaderPgms.end())
		{
			std::cerr << "Shader:" << shaderName << " Already Exists";
			return foundShader->second;
		}

		std::ifstream vertShaderFile(vertFile, std::ios::binary);
		if (!vertShaderFile)
		{
			assert("Unable to open Vertex Shader File");
			//std::cerr << "Unable to open Vertex Shader File";
			return 0;
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
			assert("Unable to open Fragment Shader File");
			//std::cerr << "Unable to open Fragment Shader File";
			return 0;
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
			assert("Unable to compile vertex shader");
			//std::cerr << "Unable to compile vertex shader:" << infoLog << std::endl;
			return 0;
		}
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, frag_shader_code, nullptr);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			assert("Unable to compile fragment shader");
			//std::cerr << "Unable to compile fragment shader:" << infoLog << std::endl;
			return 0;
		}
		GLuint shader = glCreateProgram();
		glAttachShader(shader, vertShader);
		glAttachShader(shader, fragShader);
		glLinkProgram(shader);

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			assert("Link / Compile failed");
			//std::cerr << "Link / Compile Failed:" << infoLog << std::endl;
			return 0;
		}
		glValidateProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			assert("Validate failed");
			//std::cerr << "Validate Failed:" << infoLog << std::endl;
			return 0;
		}
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		mShaderPgms.insert(std::make_pair(shaderName, shader));
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
		auto foundObj = mPrimitiveMaps.find(objName);
		if (foundObj != mPrimitiveMaps.end())
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

		mPrimitiveMaps.insert(std::make_pair(objName, p));
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
		auto foundObj = mPrimitiveMaps.find(objName);
		if (foundObj != mPrimitiveMaps.end())
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
		ifs >> p.drawCnt;
		if (p.drawCnt == 0)
		{
			std::cerr << "Error reading debug obj file";
			return;
		}

		std::vector<glm::vec2> vtx;

		vtx.reserve(p.drawCnt);
		float v1, v2;
		for (unsigned int i{}; i < p.drawCnt; ++i)
		{
			ifs >> v1 >> v2;
			vtx.emplace_back(glm::vec2{ v1, v2 });
		}
		ifs.close();

		unsigned int sizeofVtxArray = p.drawCnt * sizeof(glm::vec2);

		glCreateBuffers(1, &p.vboid);
		glNamedBufferStorage(p.vboid, sizeofVtxArray, vtx.data(), GL_DYNAMIC_STORAGE_BIT);

		// Position
		glCreateVertexArrays(1, &p.vaoid);
		glEnableVertexArrayAttrib(p.vaoid, 0); // VAO's vertex attribute index is 0 (vert)
		glVertexArrayVertexBuffer(p.vaoid, 0, // vertex buffer binding point
			p.vboid, 0, sizeof(glm::vec2));
		glVertexArrayAttribFormat(p.vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(p.vaoid, 0, 0);

		mPrimitiveMaps.insert(std::make_pair(objName, p));
	}

	void AssetManager::LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile)
	{
		auto foundTexture = mTextureMaps.find(textureName);
		if (foundTexture != mTextureMaps.end())
		{
			std::cerr << "Texture:" << textureName << " Already Exists";
			return;
		}

		Texture texture;
		std::ifstream ssDets{ spriteSheetFile, std::ios::binary };
		if (ssDets)
		{
			ssDets >> texture.xSlices >> texture.ySlices;
			ssDets.close();
		}

		stbi_uc* data = stbi_load(textureFile.c_str(), &texture.width, &texture.height, &texture.bpt, 0);
		if (!data)
		{
			std::cerr << "Unable to open texture file\n";
			exit(EXIT_FAILURE);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.t);
		glTextureStorage2D(texture.t, 1, GL_RGBA8, texture.width, texture.height);

		glTextureParameterf(texture.t, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameterf(texture.t, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(texture.t, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, ); if width * bpt is not multiple of 4
		mTextureMaps.insert(std::make_pair(textureName, texture));
	}

	void AssetManager::InitFontType()
	{
		if (FT_Init_FreeType(&mFTLib))
		{
			std::cerr << "Error with Free Type Init" << std::endl;
			return;
		}
	}

	void AssetManager::LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight)
	{
		auto foundFontTex = mFontMaps.find(fontName);
		if (foundFontTex != mFontMaps.end())
		{
			std::cerr << "Font: " << fontName << " Already Exists";
			return;
		}

		std::array<Carmicah::FontChar, 128> newFont;

		FT_Face fontFace;
		if (FT_New_Face(mFTLib, fontLoc.c_str(), 0, &fontFace))
		{
			std::cerr << "Error with Free Type Face: " << fontName << std::endl;
			return;
		}
		FT_Set_Pixel_Sizes(fontFace, 0, fontHeight);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; ++c)
		{
			if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
			{
				std::cerr << "Failed to load FreeType Glyph: " <<fontName << "(" << c << ")" << std::endl;
				continue;
			}
			Carmicah::FontChar fc;
			fc.width = fontFace->glyph->bitmap.width;
			fc.height = fontFace->glyph->bitmap.rows;
			fc.xBearing = fontFace->glyph->bitmap_left;
			fc.yBearing = fontFace->glyph->bitmap_top;
			fc.advance = fontFace->glyph->advance.x;

			glCreateTextures(GL_TEXTURE_2D, 1, &fc.texID);
			glTextureStorage2D(fc.texID, 1, GL_R8, fc.width, fc.height);
			glTextureSubImage2D(fc.texID, 0, 0, 0, fc.width, fc.height, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);

			glTextureParameterf(fc.texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameterf(fc.texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(fc.texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameterf(fc.texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			newFont[c] = std::move(fc);
		}

		FT_Done_Face(fontFace);

		mFontMaps.insert(std::make_pair(fontName, std::move(newFont)));
	}


	void AssetManager::ExportCircle(int numSlices, const std::string& modelFile)
	{
		const float M_PI = 3.14159265358979f;
		std::ofstream ofs(modelFile, std::ios::binary);
		if (ofs)
		{
			float angleInc{ 2.0f / static_cast<float>(numSlices) * M_PI };
			ofs << GL_TRIANGLE_FAN << ' ' << numSlices + 2 << ' ' << numSlices + 2 << '\n';
			ofs << "0 0\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<float>(i) * angleInc)) * 0.5f << ' ' << cosf((static_cast<float>(i) * angleInc)) * 0.5f << '\n';
			ofs << "0.5 0.5\n";
			for (int i{}; i < numSlices + 1; ++i)
				ofs << sinf((static_cast<float>(i) * angleInc)) * 0.5f + 0.5f << ' ' << cosf((static_cast<float>(i) * angleInc)) * 0.5f + 0.5f << '\n';
			ofs.close();
		}
	}

	void AssetManager::InitSound()
	{
		if (FMOD::System_Create(&mSoundSystem) != FMOD_OK)
			return;
		mSoundSystem->init(maxChannels, FMOD_INIT_NORMAL, NULL);
	}


	void AssetManager::LoadSound(const std::string& soundName, const std::string& soundFile, bool isLoop, float defaultVolume)
	{
		auto sound = mSoundMap.find(soundName);
		if (sound != mSoundMap.end())
		{
			std::cerr << "Sound:" << soundName << " Already Exists";
			return;
		}

		FMOD_MODE eMode = FMOD_DEFAULT;
		Audio audio{};
		mSoundSystem->createSound(soundFile.c_str(), eMode, nullptr, &audio.sound);
		if (audio.sound)
		{
			audio.isLoop = isLoop;
			audio.defaultVolume = defaultVolume;
			mSoundMap.insert(std::make_pair(soundName, audio));
			if (isLoop)
			{
				audio.sound->setMode(FMOD_LOOP_NORMAL);
				audio.sound->setLoopCount(-1);
			}
		}
	}



	//void AssetManager::LoadSound(const std::string& soundName, const std::string& soundFile, bool isLoop, float defaultVolume)
	//{
	//	auto sound = mSoundMap.find(soundName);
	//	if (sound != mSoundMap.end())
	//	{
	//		std::cerr << "Sound:" << soundName << " Already Exists";
	//		return;
	//	}

	//	FMOD_MODE eMode = FMOD_DEFAULT;
	//	Audio audio{};
	//	mSoundSystem->createSound(soundFile.c_str(), eMode, nullptr, &audio.sound);
	//	if (audio.sound)
	//	{
	//		audio.isLoop = isLoop;
	//		audio.defaultVolume = defaultVolume;
	//		mSoundMap.insert(std::make_pair(soundName, audio));
	//		if (isLoop)
	//		{
	//			audio.sound->setMode(FMOD_LOOP_NORMAL);
	//			audio.sound->setLoopCount(-1);
	//		}
	//	}
	//}

	bool AssetManager::GetScene(std::string scene, std::string& filePath)
	{
		if (mSceneFiles.count(scene) != 0)
		{
			filePath = mSceneFiles[scene];
			return true;
		}
		else
		{
			CM_CORE_ERROR("Scene not found");
			return false;
		}
	}
}
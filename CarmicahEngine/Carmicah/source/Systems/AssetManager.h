/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetManager.h

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
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <FMOD/fmod.hpp>
#include <filesystem>
#include <unordered_map>
#include "Singleton.h"
#include <any>

namespace Carmicah
{
#pragma region SharedPtr & UniquePtr version of Asset storing
	/*
	class IAsset {
	public:
		virtual ~IAsset() = default;
	};

	template <typename T>
	class Asset : public IAsset
	{
	public:
		std::unordered_map<std::string, std::shared_ptr<IAsset>> mAssetMap;
	};

	class AssetManagerPtrVersion : Singleton<AssetManagerPtrVersion>
	{
	public:
		// since theres gonna be different assets, storing it as a generic void pointer can
		// ignore the type of asset and just store it as an Asset<T>
		// Then just static cast based on the type we're looking for
		// kinda the same as component manager except theres another level above component manager
		std::unordered_map<std::string, std::any> mAssetMap; // idk how to name this map

		FT_Library mFTLib;
		const unsigned int fontSize{ 36 };
		// Audio
		const int maxChannels{ 32 };
		FMOD::System* mSoundSystem{};
		std::unordered_map<std::string, FMOD::Channel*> mChannelMap;
		std::unordered_map<std::string, Audio> mSoundMap{};

		template <typename T>
		Asset<T>& GetAssetMap()
		{
			std::string assetType = typeid(T).name();

			// Doesnt exist in the map yet
			if (mAssetMap.count(assetType) == 0)
			{
				assert("Asset type does not exist");
				//mAssetMap[assetType] = std::make_unique<Asset<T>>();
			}

			return std::static_pointer_cast<Asset<T>>(mAssetMap[assetType]);
		}

		template <typename T>
		void RegisterAsset()
		{
			std::string assetType = typeid(T).name();

			if (mAssetMap.count(assetType) == 0)
			{
				mAssetMap[assetType] = std::any<Asset<T>>();
			}
			else
			{
				CM_CORE_ERROR("Asset being registered more than once");
			}
		}

		void LoadAll(const char* assetPath)
		{
			std::filesystem::path directoryPath = assetPath;

			RegisterAsset<Audio>();
			RegisterAsset<Texture>();
			RegisterAsset<FontChar>();
			RegisterAsset<Primitive>();
			RegisterAsset<Scene>();
			RegisterAsset<Shader>();
			RegisterAsset<Prefab>();

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
								LoadSound(fileName, entry.path().string(), false);
							}
							else if (folderName == "BGM")
							{
								LoadSound(fileName, entry.path().string(), true);
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
								LoadFont(fileName, entry.path().string(), fontSize);
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
								auto& mSceneAsset = GetAssetMap<Scene>();
								Scene sceneAsset;
								sceneAsset.sceneFile = entry.path().string();
								mSceneAsset.mAssetMap.insert(std::make_pair(fileName, std::make_shared<Asset<Scene>>(sceneAsset)));
							}
							else if (folderName == "Shaders")
							{
								std::string fileExt = entry.path().extension().string();
								if (fileExt == ".vert")
								{
									const auto testOtherFile = subFile.path() / (fileName + std::string(".frag"));
									if (std::filesystem::exists(testOtherFile))
									{
										LoadShader(fileName, entry.path().string(), testOtherFile.string());
									}
								}
							}
							else if (folderName == "Prefabs")
							{
								Prefab goPrefab = Serializer.DeserializePrefab(entry.path().string());
								auto& mPrefabAsset = GetAssetMap<Prefab>();
								mPrefabAsset.mAssetMap.insert(std::make_pair(fileName, std::make_shared<Asset<Prefab>>(goPrefab)));
							}
						}
					}
				}
			}
		}

		void UnloadAll()
		{
			for (auto& mAssetType : mAssetMap)
			{
				// if shader
				if (mAssetType.first == typeid(Shader).name())
				{
					for (auto& asset : std::any_cast<Asset<Shader>>(mAssetType).mAssetMap)
					{
						glDeleteTextures(1, &(std::any_cast<Shader>(asset).s));
					}
				}
				else if (mAssetType.first == typeid(Primitive).name())
				{
					for ( auto& asset : std::any_cast<Asset<Primitive>>(mAssetType).mAssetMap)
					{
						GLuint& primitiveVAOID = std::any_cast<Primitive>(asset).vaoid;
						glDeleteTextures(1, &);
					}
				}
			}
		}

		// Sound Assets
		void InitSound()
		{
			if (FMOD::System_Create(&mSoundSystem) != FMOD_OK)
				return;
			mSoundSystem->init(maxChannels, FMOD_INIT_NORMAL, NULL);
		}

		void InitFontType()
		{
			if (FT_Init_FreeType(&mFTLib))
			{
				std::cerr << "Error with Free Type Init" << std::endl;
				return;
			}
		}

		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile)
		{
			auto& mTextureAsset = GetAssetMap<Texture>();
			auto& foundTexture = mTextureAsset.mAssetMap.find(textureName);
			if (foundTexture != mTextureAsset.mAssetMap.end())
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
			mTextureAsset.mAssetMap.insert(std::make_pair(textureName, std::make_shared<Asset<Texture>>(texture)));
		}

		void LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop)
		{
			auto& sound = mSoundMap.find(soundName);
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
				audio.isLoop = b_isLoop;
				mSoundMap.insert(std::make_pair(soundName, audio));
				if (b_isLoop)
				{
					audio.sound->setMode(FMOD_LOOP_NORMAL);
					audio.sound->setLoopCount(-1);
				}
			}
		}

		void LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight)
		{
			auto& mFontAsset = GetAssetMap<FontChar>();
			auto& foundFontTex = mFontAsset.mAssetMap.find(fontName);
			if (foundFontTex != mFontAsset.mAssetMap.end())
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
					std::cerr << "Failed to load FreeType Glyph: " << fontName << "(" << c << ")" << std::endl;
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

			mFontAsset.mAssetMap.insert(std::make_pair(fontName, std::make_shared<Asset<FontChar>>(fontFace)));
		}

		void LoadObject(const std::string& objName, const std::string& modelFile)
		{
			auto& mObjectAsset = GetAssetMap<Primitive>();
			auto& foundObj = mObjectAsset.mAssetMap.find(objName);
			if (foundObj != mObjectAsset.mAssetMap.end())
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

			mObjectAsset.mAssetMap.insert(std::make_pair(objName, std::make_shared<Asset<Primitive>>(p)));
		}
	
		void LoadDebugObject(const std::string& objName, const std::string& modelFile)
		{
			auto& mObjectAsset = GetAssetMap<Primitive>();
			auto& foundObj = mObjectAsset.mAssetMap.find(objName);
			if (foundObj != mObjectAsset.mAssetMap.end())
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

			mObjectAsset.mAssetMap.insert(std::make_pair(objName, std::make_shared<Asset<Primitive>>(p)));
		}

		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile)
		{
			auto& mShaderAssets = GetAssetMap<Shader>();
			// Shader Exists
			auto& foundShader = mShaderAssets.mAssetMap.find(shaderName);
			if (foundShader != mShaderAssets.mAssetMap.end())
			{
				std::cerr << "Shader:" << shaderName << " Already Exists";
				return std::any_cast<Shader>(foundShader->second).s;
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

			mShaderAssets.mAssetMap.insert(std::make_pair(shaderName, std::make_shared<Asset<Shader>>(shader)));
			return shader;
		}
};*/
#pragma endregion SharedPtr & UniquePtr version of Asset storing

	struct Primitive
	{
		GLuint vaoid{};
		GLuint vboid{};
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct Shader
	{
		GLuint s;
	};
	struct Texture
	{
		GLuint t;
		int width;
		int height;
		int bpt;

		int xSlices;
		int ySlices;
	};
	struct FontChar 
	{
		unsigned int texID, width, height;
		int			 xBearing, yBearing;
		long		 advance;
	};
	struct Audio
	{
		bool isLoop;
		FMOD::Sound* sound;
	};
	struct Prefab
	{
		unsigned int mPrefabID;
		std::string mName;
		std::unordered_map<std::string, std::any> mComponents;
	};
	struct Scene
	{
		std::string sceneFile;
	};
	struct EngineConfig
	{
		int Width, Height;
		std::string defaultScene;
		std::string lastScene;
		std::string defaultShader;
		std::string assetLoc;
	};

	class AssetManager : public Singleton<AssetManager>
	{
	public:

		void LoadConfig(const std::string& configPath);
		// Data
		void LoadAll(const char*);
		void UnloadAll();

		//std::unordered_map<std::string, std::shared_ptr<BaseAsset>> mMapOfAssets;
		//
		EngineConfig enConfig;
		std::unordered_map<std::string, GLuint> mShaderPgms{};
		std::unordered_map<std::string, Texture> mTextureMaps{};
		std::unordered_map<std::string, Primitive> mPrimitiveMaps{};
		std::unordered_map<std::string, std::string> mSceneFiles{};
		std::unordered_map<std::string, Prefab> mPrefabFiles{};
		std::unordered_map<std::string, std::array<Carmicah::FontChar, 128>> mFontMaps{};

		FT_Library mFTLib;
		const unsigned int fontSize{ 36 };
		// Audio
		const int maxChannels{ 32 };
		FMOD::System* mSoundSystem{};
		std::unordered_map<std::string, FMOD::Channel*> mChannelMap;
		std::unordered_map<std::string, Audio> mSoundMap{};


		bool GetScene(std::string scene, std::string& filePath);

	private:

		// Graphics Assets
		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		void LoadObject(const std::string& objName, const std::string& modelFile);
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);
		void InitFontType();
		void LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight);

		void ExportCircle(int numSlices, const std::string& modelFile);

		// Sound Assets
		void InitSound();
		void LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop);

	};
}
#endif
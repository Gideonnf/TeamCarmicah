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
#include <stb/stb_image.h>
#include "AssetManager.h"
#include "log.h"
#include "Math/Vec2.h"
#include "Systems/SoundSystem.h"
#include "Systems/SerializerSystem.h"
#include "Systems/AssetTypes.h"
#include "Graphics/RenderHelper.h"

namespace Carmicah
{
	void AssetManager::Init(std::shared_ptr<PrefabSystem> prefabRef)
	{
		prefabPtr = prefabRef;
	}
	GLuint BasePrimitive::uidCount = 0;

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

		InitTexture();
		InitSound();
		InitFontType();
		fileWatcher.Init(assetPath);
		fileWatcher.Update();
		//if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath))
		//{
		//	for (const auto& subFile : std::filesystem::directory_iterator(directoryPath))
		//	{
		//		if (std::filesystem::exists(subFile) && std::filesystem::is_directory(subFile))
		//		{
		//			std::string folderName = subFile.path().filename().string();
		//			for (const auto& entry : std::filesystem::directory_iterator(subFile))
		//			{
		//				std::string fileName = entry.path().stem().string();
		//				if (folderName == "Animation")
		//				{
		//					LoadAnimation(fileName, entry.path().string());
		//				}
		//				else if (folderName == "Audio")
		//				{
		//					LoadSound(fileName, entry.path().string(), false, 1.0f); 
		//				}
		//				else if (folderName == "BGM")
		//				{
		//					LoadSound(fileName, entry.path().string(), true, 1.0f);  
		//				}
		//				else if (folderName == "Images")
		//				{
		//					std::string fileExt = entry.path().extension().string();

		//					if (fileExt == ".png")
		//					{
		//						const auto testOtherFile = subFile.path() / (fileName + std::string(".txt"));
		//						LoadTexture(fileName, entry.path().string(), testOtherFile.string());
		//					}
		//				}
		//				else if (folderName == "Fonts")
		//				{
		//					LoadFont(fileName, entry.path().string(), enConfig.fontSize);
		//				}
		//				else if (folderName == "Meshes")
		//				{

		//					std::string fileExt = entry.path().extension().string();
		//					if (fileExt == ".o")
		//					{
		//						LoadObject(fileName, entry.path().string());
		//					}
		//					else if (fileExt == ".do")
		//					{
		//						LoadDebugObject("Debug" + fileName, entry.path().string());
		//					}
		//				}
		//				else if (folderName == "Scene")
		//				{
		//					//	std::cout << entry.path().string() << std::endl;
		//						//std::cout << fileName << std::endl;
		//					Scene newScene{ entry.path().string() };
		//					AddAsset<Scene>(fileName, newScene);
		//					//mSceneFiles.insert(std::make_pair(fileName, entry.path().string()));
		//				}
		//				else if (folderName == "Shaders")
		//				{
		//					std::string fileExt = entry.path().extension().string();
		//					if (fileExt == ".vert")
		//					{
		//						const auto testOtherFile = subFile.path() / (fileName + std::string(".frag"));
		//						if (std::filesystem::exists(testOtherFile))
		//						{
		//							LoadShader(fileName, entry.path().string(), testOtherFile.string());
		//						}
		//					}
		//				}
		//				else if (folderName == "Prefabs")
		//				{
		//					Prefab goPrefab = Serializer.DeserializePrefab(entry.path().string());
		//					
		//					prefabPtr->AddPrefab(goPrefab);

		//					AddAsset<Prefab>(fileName, goPrefab);
		//					//mPrefabFiles.insert(std::make_pair(fileName, goPrefab));
		//				}
		//			}
		//		}
		//	}
		//}

		//enConfig = Serializer.LoadConfig(directoryPath)
		}

	bool AssetManager::LoadAsset(File const& file, bool reload)
	{
		std::string fileName = file.fileEntry.path().stem().string();
		std::string fileExt = file.fileEntry.path().extension().string();


		if (fileExt == ".ani")
		{

			if (!reload && AssetExist<AnimAtlas>(fileName))
			{
				CM_CORE_WARN("Animation:" + fileName + " Already Exists");
				return false;
			}

			LoadAnimation(fileName, file.fileEntry.path().string());
		}
		else if (fileExt == ".wav" || fileExt == ".ogg")
		{
			if (!reload && AssetExist<FMOD::Sound*>(fileName))
			{
				CM_CORE_WARN("Sound:" + fileName + " Already Exists");
				return false;
			}
			LoadSound(fileName, file.fileEntry.path().string());
		}
		else if (fileExt == ".ttf")
		{
			// Font cant be reloaded for now cause operator = is giving me issues
			if (AssetExist<Font>(fileName))
			{
				CM_CORE_WARN("Font:" + fileName + " Already Exists");
				return false;
			}
			LoadFont(fileName, file.fileEntry.path().string(), enConfig.fontSize);
		}
		else if (fileExt == ".png")
		{
			if (!reload && AssetExist<Texture>(fileName))
			{
				CM_CORE_WARN("Texture:" + fileName + " Already Exists");
				return false;
			}

			const auto spriteSheet = file.fileEntry.path().parent_path() / (file.fileEntry.path().stem().string() + std::string(".txt"));

			if (fileWatcher.fileMap.count(spriteSheet.string()) != 0)
			{
				fileWatcher.fileMap[spriteSheet.string()].fileStatus = FILE_OK;
			}
			LoadTexture(fileName, file.fileEntry.path().string(), spriteSheet.string());
		}
		else if (fileExt == ".o")
		{
			if (!reload && AssetExist<Primitive>(fileName))
			{
				CM_CORE_WARN("Object:" + fileName + " Already Exists");
				return false;
			}
			LoadObject(fileName, file.fileEntry.path().string());
		}
		else if (fileExt == ".do")
		{
			if (!reload && AssetExist<BasePrimitive>(fileName))
			{
				CM_CORE_WARN("Object:" + fileName + " Already Exists");
				return false;
			}
			LoadDebugObject("Debug" + fileName, file.fileEntry.path().string());
		}
		else if (fileExt == ".scene")
		{
			if (!reload && AssetExist<Scene>(fileName))
			{
				CM_CORE_WARN("Scene:" + fileName + " Already Exists");
				return false;
			}

			Scene newScene{ file.fileEntry.path().string() };
			AddAsset<Scene>(fileName, newScene);
		}
		else if (fileExt == ".prefab")
		{
			if (!reload && AssetExist<Prefab>(fileName))
			{
				CM_CORE_WARN("Scene:" + fileName + " Already Exists");
				return false;
			}

			Prefab goPrefab = Serializer.DeserializePrefab(file.fileEntry.path().string());

			// If its a new asset then update the prefab ID list
			if (!reload)
				prefabPtr->AddPrefab(goPrefab);

			AddAsset<Prefab>(fileName, goPrefab);
			//mPrefabFiles.insert(std::make_pair(fileName, goPrefab));
		}
		else if (fileExt == ".vert")
		{
			const auto fragShader = file.fileEntry.path().parent_path() / (file.fileEntry.path().stem().string() + std::string(".frag")); // std::string(".frag");
			if (std::filesystem::exists(fragShader))
			{
				fileWatcher.fileMap[fragShader.string()].fileStatus = FILE_OK;

				LoadShader(fileName, file.fileEntry.path().string(), fragShader.string());
			}
		}
		else if (fileExt == ".json")
		{
			// dont do anything with json for now
			// only engine config uses it but carmicahCore loads it with:
			// AssetManager::GetInstance()->LoadConfig("../Assets/config.json");
		}
		else
		{
			CM_CORE_ERROR("Extension doesn't exist");
			return false;
		}
		return true;
	}

	void AssetManager::UnloadAll()
	{
		// Unload Graphics
		glDeleteTextures(1, &mArrayTex);
		for (int i{}; i < mPreviewTexs.size(); ++i)
			glDeleteTextures(1, &mPreviewTexs[i]);
		GetAssetMap<Texture>()->mAssetMap.clear();
		for (const auto& i : RenderHelper::GetInstance()->mBufferMap)
		{
			for (const auto& ii : i.second.buffer)
			{
				glDeleteVertexArrays(1, &ii.vao);
				glDeleteBuffers(1, &ii.vbo);
			}
			glDeleteBuffers(1, &i.second.ebo);
			glDeleteBuffers(1, &i.second.ibo);
		}

		// Delete Shaders
		for (const auto& i : GetAssetMap<Shader>()->mAssetList)
		{
			glDeleteProgram(i.s);
		}
		GetAssetMap<Shader>()->mAssetMap.clear();

		FT_Done_FreeType(mFTLib);

		// Unload Sound
		for (auto& i : GetAssetMap<FMOD::Sound*>()->mAssetList)
			i->release();
		if (mSoundSystem != NULL)
			mSoundSystem->release();
	}

	// Load Functions
	GLuint AssetManager::LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile)
	{
		// Shader Exists
		//if (AssetExist<Shader>(shaderName))
		//{
		//	CM_CORE_WARN("Shader:" + shaderName + " Already Exists");
		//	auto foundShader = GetAsset<Shader>(shaderName).s;
		//	return foundShader;
		//}
		std::ifstream vertShaderFile(vertFile, std::ios::binary);
		if (!vertShaderFile)
		{
			assert("Unable to open Vertex Shader File");
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
			CM_CORE_WARN("Unable to compile vertex shader:" + std::string(infoLog));
			assert("Unable to compile vertex shader");
			return 0;
		}
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, frag_shader_code, nullptr);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			CM_CORE_WARN("Unable to compile fragment shader:" + std::string(infoLog));
			assert("Unable to compile fragment shader");
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
			CM_CORE_WARN("Link / Compile Failed:" + std::string(infoLog));
			assert("Link / Compile failed");
			return 0;
		}
		glValidateProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			CM_CORE_WARN("Validate Failed:" + std::string(infoLog));
			assert("Validate failed");
			return 0;
		}
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		Shader newShader;
		newShader.s = shader;

		AddAsset(shaderName, newShader);
		//mShaderPgms.insert(std::make_pair(shaderName, shader));
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

		//if (AssetExist<Primitive>(objName))
		//{
		//	CM_CORE_WARN("Object:" + objName + " Already Exists");
		//	return;
		//}

		std::ifstream ifs(modelFile, std::ios::binary);
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open Obj:" + modelFile);
			return;
		}
		Primitive p;// GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
		p.uid = p.uidCount++;
		// No error checking, just fastest way to read data
		unsigned int numVert;
		ifs >> p.drawMode >> numVert >> p.drawCnt;
		if (p.drawMode == 0)
		{
			CM_CORE_ERROR("Error reading obj file");
			ifs.close();
			return;
		}

		p.vtx.reserve(numVert);
		p.texCoord.reserve(numVert);
		p.idx.reserve(p.drawCnt);
		float v1, v2;
		for (unsigned int i{}; i < numVert; ++i)
		{
			ifs >> v1 >> v2;
			p.vtx.emplace_back(Vec2f{ v1, v2 });
		}
		for (unsigned int i{}; i < numVert; ++i)
		{
			ifs >> v1 >> v2;
			p.texCoord.emplace_back(Vec2f{ v1, v2 });
		}
		// Only save index when following Triangle method
		if (p.drawMode == GL_TRIANGLES)
		{
			GLushort i1;
			for (unsigned int i{}; i < p.drawCnt; ++i)
			{
				ifs >> i1;
				p.idx.emplace_back(i1);
			}
		}
		ifs.close();
		AddAsset(objName, p);
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
		//if (AssetExist<BasePrimitive>(objName))
		//{
		//	CM_CORE_WARN("Object:" + objName + " Already Exists");
		//	return;
		//}

		std::ifstream ifs(modelFile, std::ios::binary);
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open Obj:" + modelFile);
			return;
		}
		BasePrimitive p;
		p.uid = p.uidCount++;
		p.drawMode = GL_LINE_LOOP;
		ifs >> p.drawCnt;
		if (p.drawCnt == 0)
		{
			CM_CORE_ERROR("Error reading debug obj file");
			ifs.close();
			return;
		}

		p.vtx.reserve(p.drawCnt);
		float v1, v2;
		for (unsigned int i{}; i < p.drawCnt; ++i)
		{
			ifs >> v1 >> v2;
			p.vtx.emplace_back(Vec2f{ v1, v2 });
		}
		ifs.close();

		AddAsset(objName, p);
	}

	void AssetManager::InitTexture()
	{
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &mArrayTex);
		glBindTexture(GL_TEXTURE_2D_ARRAY, mArrayTex);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		    1,							//No mipmaps as textures are 1x1
		    GL_RGBA8,					//Internal format
			enConfig.maxTexSize, enConfig.maxTexSize,		//width,height
			enConfig.maxNumTextures		//Number of layers
		);

		glTextureParameterf(mArrayTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameterf(mArrayTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		mPreviewTexs.resize(enConfig.maxNumTextures);
		for (int i{}; i < enConfig.maxNumTextures; ++i)
		{
			glGenTextures(1, &mPreviewTexs[i]);
			glTextureView(mPreviewTexs[i], GL_TEXTURE_2D, mArrayTex, GL_RGBA8, 0, 1, i, 1);
		}

	}

	void AssetManager::LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile)
	{
		//int maxTextureSize;
		//glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		//CM_CORE_INFO("Max Texture Size is: " + std::to_string(maxTextureSize));

		//if (AssetExist<Texture>(textureName))
		//{
		//	CM_CORE_WARN("Texture:" + textureName + " Already Exists");
		//	return;
		//}

		Texture texture{};
		int texWidth{}, texHeight{}, bytePerTex{};

		stbi_uc* data = stbi_load(textureFile.c_str(), &texWidth, &texHeight, &bytePerTex, 0);
		if (!data || texWidth > enConfig.maxTexSize || texHeight > enConfig.maxTexSize || bytePerTex != 4)
		{
			CM_CORE_ERROR("Unable to open texture file");
			stbi_image_free(data);
			return;
		}
		// Read if the sprite needs to be divided
		struct spriteDetails
		{
			std::string name;
			int x, y, width, height, num;
		};
		std::vector<spriteDetails> spriteD;

		std::ifstream ssDets{ spriteSheetFile, std::ios::binary };
		if (ssDets)
		{
			std::string tempS;
			char tempC;

			spriteDetails t;

			while (!ssDets.eof())
			{
				ssDets >> t.name;
				ssDets >> tempS >> tempS >> t.x >> tempC >> t.y >> tempC;
				ssDets >> t.width >> tempC >> t.height;
				ssDets >> t.num;

				if (!ssDets.eof())
					spriteD.push_back(t);

				ssDets >> tempS;
			}
			ssDets.close();
		}

		texture.t = currTexPt++;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);// if width * bpt is not multiple of 4
		glTextureSubImage3D(mArrayTex,
			0,						// Mipmap
			0, 0, texture.t,		// x/y/z Offset
			texWidth, texHeight, 1,	// width, height, depth
			GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		
		
		texture.mtx.m[0] = texture.mtx.m[1] = 0.f;
		texture.mtx.m[2] = static_cast<float>(texWidth);
		texture.mtx.m[3] = static_cast<float>(texHeight);
		AddTextureImage(texture, textureName);
		if (!spriteD.empty())
		{
			for (int i{}; i < spriteD.size(); ++i)
			{
				texture.mtx.m[0] = static_cast<float>(spriteD[i].x);
				texture.mtx.m[1] = static_cast<float>(spriteD[i].y);
				texture.mtx.m[2] = static_cast<float>(spriteD[i].width);
				texture.mtx.m[3] = static_cast<float>(spriteD[i].height);
				AddTextureImage(texture, textureName, spriteD[i].num, std::string("_") + spriteD[i].name);
			}
		}
	}

	void AssetManager::AddTextureImage(Texture& t, const std::string& textureName, const int& num, const std::string& extName)
	{
		float	x = t.mtx.m[0] / static_cast<float>(enConfig.maxTexSize),
				y = t.mtx.m[1] / static_cast<float>(enConfig.maxTexSize),
				width = t.mtx.m[2] / static_cast<float>(enConfig.maxTexSize),
				height = t.mtx.m[3] / static_cast<float>(enConfig.maxTexSize);
		Mtx33Identity(t.mtx);

		t.mtx.translateThis(x, 1.f - y - height).scaleThis(width, height);
		for (int i{}; i < num; ++i)
		{
			std::string name{ textureName };
			if(num != 1)
				name += extName + ' ' + std::to_string(i);
			AddAsset(name, t);
			t.mtx.m[6] += width;
			if (t.mtx.m[6] + width > 1.f)
			{
				t.mtx.m[6] = 0.f;
				t.mtx.m[7] -= height;
			}
		}

	}

	void AssetManager::LoadAnimation(const std::string& animName, const std::string& animFile)
	{
		//if (AssetExist<AnimAtlas>(animName))
		//{
		//	CM_CORE_WARN("Animation:" + animName + " Already Exists");
		//	return;
		//}


		std::ifstream ifs(animFile, std::ios::binary);
		if (!ifs)
		{
			CM_CORE_ERROR("Unable to open Animation:" + animFile);
			return;
		}
		AnimAtlas a;
		std::string texName{}, texTime{};
		while (true)
		{
			std::getline(ifs, texName);
			if (texName.back() == '\r')
				texName.pop_back();
			std::getline(ifs, texTime);

			a.anim.emplace_back(std::make_pair(std::stof(texTime), texName));
			if (ifs.eof())
				break;
		}
		ifs.close();
		AddAsset<AnimAtlas>(animName, a);
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
		//if (AssetExist<Font>(fontName))
		//{
		//	CM_CORE_WARN("Font: " + fontName + " Already Exists");
		//	return;
		//}

		Font fontObj;
		std::array<std::pair<unsigned char, unsigned int>, 96> fontHeightMap; // to sort by height

		FT_Face fontFace;
		if (FT_New_Face(mFTLib, fontLoc.c_str(), 0, &fontFace))
		{
			CM_CORE_ERROR("Error with Free Type Face: " + fontName);
			return;
		}
		FT_Set_Pixel_Sizes(fontFace, 0, fontHeight);

		for (unsigned char c{ charOffset }; c < 128; ++c)
		{
			if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
			{
				std::cerr << "Failed to load FreeType Glyph: " << fontName << "(" << c << ")" << std::endl;
				continue;
			}
			int i{ c - charOffset };

			fontObj.mFontMaps[i].width = fontFace->glyph->bitmap.width;
			fontObj.mFontMaps[i].height = fontFace->glyph->bitmap.rows;
			fontObj.mFontMaps[i].xBearing = fontFace->glyph->bitmap_left;
			fontObj.mFontMaps[i].yBearing = fontFace->glyph->bitmap_top;
			fontObj.mFontMaps[i].advance = fontFace->glyph->advance.x;


			fontHeightMap[i] = std::make_pair(c, fontObj.mFontMaps[i].height);
		}

		std::sort(fontHeightMap.begin(), fontHeightMap.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.second < rhs.second;
			});

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//glTextureParameterf(currTexPt, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTextureParameterf(currTexPt, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTextureParameterf(currTexPt, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameterf(currTexPt, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned int heightSoFar{}, widthAccumulated{}, heightAccumulated{};

		for (auto& i : fontHeightMap)
		{
			if (FT_Load_Char(fontFace, i.first, FT_LOAD_RENDER))
			{
				std::cerr << "Failed to load FreeType Glyph: " << fontName << "(" << i.first << ")" << std::endl;
				continue;
			}

			int fontArrayNum{ i.first - charOffset };

			Font::FontChar& fc = fontObj.mFontMaps[fontArrayNum];
			if (widthAccumulated > static_cast<unsigned int>(enConfig.maxTexSize))
			{
				widthAccumulated = 0;
				heightAccumulated += heightSoFar;	
			}
			glTextureSubImage3D(mArrayTex,
				0,						// Mipmap
				widthAccumulated, heightAccumulated, currTexPt,		// x/y/z Offset
				fc.width, fc.height, 1,	// width, height, depth
				GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);
			FontTexture fontTex{};
			fontTex.t = currTexPt;
			float	x		= static_cast<float>(widthAccumulated) / static_cast<float>(enConfig.maxTexSize),
					y		= static_cast<float>(enConfig.maxTexSize - heightAccumulated) / static_cast<float>(enConfig.maxTexSize),
					width	= static_cast<float>(fc.width) / static_cast<float>(enConfig.maxTexSize),
					height	= static_cast<float>(fc.height) / static_cast<float>(enConfig.maxTexSize);

			fontTex.mtx.translateThis(x, 1.f - y - height).scaleThis(width, height);


			std::string name = fontName + ' ' + std::to_string(i.first);
			AddAsset<FontTexture>(name,fontTex);
			fc.texRef = name;

			if (fc.height > heightSoFar)
				heightSoFar = fc.height;
			widthAccumulated += fc.width;
		}

		++currTexPt;
		FT_Done_Face(fontFace);
		AddAsset<Font>(fontName, fontObj);
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
		FMOD_RESULT result = FMOD::System_Create(&mSoundSystem);
		if (result != FMOD_OK) {
			CM_CORE_ERROR("FMOD System creation failed");
			return;
		}

		result = mSoundSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK) {
			CM_CORE_ERROR("FMOD System initialization failed");
			return;
		}
	}


	void AssetManager::LoadSound(const std::string& soundName, const std::string& soundFile)
	{
		FMOD_MODE eMode = FMOD_DEFAULT;
		FMOD::Sound* sound{ nullptr };
		mSoundSystem->createSound(soundFile.c_str(), eMode, nullptr, &sound);
		if (sound)
		{
			AddAsset<FMOD::Sound*>(soundName, sound);
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
		if (AssetExist<Scene>(scene))
		{
			filePath = GetAsset<Scene>(scene).sceneFile;
			return true;
		}
		//if (mSceneFiles.count(scene) != 0)
		//{
		//	filePath = mSceneFiles[scene];
		//	return true;
		//}
		else
		{
			CM_CORE_ERROR("Scene not found");
			return false;
		}
	}
}
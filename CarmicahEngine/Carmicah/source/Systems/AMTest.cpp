#include "pch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <FMOD/fmod.hpp>
#include <filesystem>
#include "AMTest.h"
#include "Systems/SerializerSystem.h"
namespace CarmicahTesting
{
	void AssetManagerPtrVersion::LoadAll(const char* assetPath)
	{
		std::filesystem::path directoryPath = assetPath;
		RegisterAsset<Texture>();

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
						if (folderName == "Prefabs")
						{
							Prefab goPrefab = Carmicah::Serializer.DeserializePrefab(entry.path().string());
							mPrefabFiles.insert(std::make_pair(fileName, goPrefab));
						}
					}
				}
			}
		}
	}

	//void AssetManagerPtrVersion::LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile)
	//{
	//	if (AssetExist<Texture>(textureName))
	//	{
	//		return;
	//	}

	//	Texture texture;
	//	std::ifstream ssDets{ spriteSheetFile, std::ios::binary };
	//	if (ssDets)
	//	{
	//		ssDets >> texture.xSlices >> texture.ySlices;
	//		ssDets.close();
	//	}

	//	stbi_uc* data = stbi_load(textureFile.c_str(), &texture.width, &texture.height, &texture.bpt, 0);
	//	if (!data)
	//	{
	//		std::cerr << "Unable to open texture file\n";
	//		exit(EXIT_FAILURE);
	//	}

	//	glCreateTextures(GL_TEXTURE_2D, 1, &texture.t);
	//	glTextureStorage2D(texture.t, 1, GL_RGBA8, texture.width, texture.height);

	//	glTextureParameterf(texture.t, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTextureParameterf(texture.t, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//	glTextureSubImage2D(texture.t, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	stbi_image_free(data);
	//	//glPixelStorei(GL_UNPACK_ALIGNMENT, ); if width * bpt is not multiple of 4
	//	//mTextureMaps.insert(std::make_pair(textureName, texture));
	//	AddAsset<Texture>(textureName, texture);
	//}

}
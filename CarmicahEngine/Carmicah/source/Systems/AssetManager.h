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

#include <assert.h>
#include <GLFW/glfw3.h>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <FMOD/fmod.hpp>
#include <filesystem>
#include <unordered_map>
#include "Singleton.h"
#include "Systems/AssetTypes.h"

namespace Carmicah
{
	// Only used for the map so that theres a base class that can be dynamic casted
	class IAsset
	{
	public:
		virtual ~IAsset() = default;
	};

	template <typename T>
	class AssetType : public IAsset
	{
	public:
		std::vector<T> mAssetList;
		std::unordered_map<std::string, unsigned int> mAssetMap;
	};

	class AssetManager : public Singleton<AssetManager>
	{
	public:

		void LoadConfig(const std::string& configPath);
		// Data
		void LoadAll(const char*);
		void UnloadAll();

		EngineConfig enConfig{};
		std::unordered_map<std::string, std::shared_ptr<IAsset>> mAssetTypeMap{};

		FT_Library mFTLib;
		GLuint mArrayTex{}, currTexPt{};
		const int maxTexSize{ 4096 };
		// Audio
		const int maxChannels{ 32 };
		FMOD::System* mSoundSystem{};
		std::unordered_map<std::string, FMOD::Channel*> mChannelMap;
		std::unordered_map<std::string, Audio> mSoundMap{};


		bool GetScene(std::string scene, std::string& filePath);

		template <typename T>
		std::shared_ptr<AssetType<T>> GetAssetMap()
		{
			std::string assetType = typeid(T).name();

			// Doesnt exist in the map yet
			if (mAssetTypeMap.count(assetType) == 0)
			{
				//assert("Asset type does not exist");
				//mAssetMap[assetType] = std::make_unique<Asset<T>>();
			}
			// cast it to the correct asset type 
			return std::static_pointer_cast<AssetType<T>>(mAssetTypeMap[assetType]);
		}

		template <typename T>
		T& GetAsset(std::string name)
		{
			if (GetAssetMap<T>()->mAssetList.size() == 0)
				assert("Asset does not exist");
			if (GetAssetMap<T>()->mAssetMap.count(name) == 0)
				assert("Asset does not exist");

			return GetAssetMap<T>()->mAssetList[GetAssetMap<T>()->mAssetMap[name]];
		}

		template <typename T>
		void RegisterAsset()
		{
			std::string assetType = typeid(T).name();

			if (mAssetTypeMap.count(assetType) == 0)
			{
				mAssetTypeMap[assetType] = std::make_shared<AssetType<T>>();
			}
			else
			{
				//CM_CORE_ERROR("Asset being registered more than once");
				//assert("Asset has been registered before");
			}
		}

		template <typename T>
		void AddAsset(std::string name, T& asset)
		{
			// Check if asset exist
			std::string assetType = typeid(T).name();
			if (mAssetTypeMap.count(assetType) == 0)
			{
				// If the asset doesn't exist yet, then create it
				RegisterAsset<T>();
			}

			GetAssetMap<T>()->mAssetList.push_back(asset);
			// The map is name -> index
			// where index is where the asset belongs in mAssetList
			GetAssetMap<T>()->mAssetMap[name] = GetAssetMap<T>()->mAssetList.size() - 1;
		}

		template <typename T>
		bool AssetExist(std::string name)
		{
			// Check if asset exist
			std::string assetType = typeid(T).name();
			if (mAssetTypeMap.count(assetType) == 0)
			{
				// If the asset doesn't exist yet, then create it
				RegisterAsset<T>();
			}

			if (GetAssetMap<T>()->mAssetList.size() == 0)
				return false;
			if (GetAssetMap<T>()->mAssetMap.count(name) == 0)
				return false;

			return true;
		}
		// TODO: Handle removal of assets
		// cant rlly be done/tested until editor has ways to delete and add assets
	private:


		// Graphics Assets
		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		void LoadObject(const std::string& objName, const std::string& modelFile);
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		void InitTexture();
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);
		void AddTextureImage(Texture& t, const std::string& textureName, const int& num = 0, const int& ver = 0, bool wholeSprite = false);
		void LoadAnimation(const std::string& animName, const std::string& animFile);
		void InitFontType();
		void LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight);

		void ExportCircle(int numSlices, const std::string& modelFile);

		// Sound Assets
		void InitSound();
		//void LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop);
		void LoadSound(const std::string& soundName, const std::string& soundFile, bool isLoop, float defaultVolume = 1.0f);


	};
}
#endif
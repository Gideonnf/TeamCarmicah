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
#include "PrefabSystem.h"
#include "FileWatcher.h"

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
		void Init(std::shared_ptr<PrefabSystem> prefabRef);

		/*!*************************************************************************
		brief
			Loads the config data from
		param[assetPath]
			Location of the config file to read data from
		***************************************************************************/
		void LoadConfig(const std::string& configPath);
		//----------------------------  Data  ----------------------------
		/*!*************************************************************************
		brief
			Tries to load all data from the Asset filePath
		param[assetPath]
			Asset Path to load data from
		***************************************************************************/
		void LoadAll(const char* assetPath);

		bool LoadAsset(File const& file, bool reload = false);

		/*!*************************************************************************
		brief
			Unloads and frees data held by the AssetManager
		***************************************************************************/
		void UnloadAll();

		FileWatcher fileWatcher;
		EngineConfig enConfig{};
		std::unordered_map<std::string, std::shared_ptr<IAsset>> mAssetTypeMap{};

		FT_Library mFTLib;
		GLuint mArrayTex{}, currTexPt{};// currTexPt used for both fonts and textures
		std::vector<GLuint> mPreviewTexs;

		// Audio
		const int MAX_CHANNELS = 256;
		FMOD::System* mSoundSystem{};

		std::shared_ptr<PrefabSystem> prefabPtr;

		/*!*************************************************************************
		brief
			Gets the Scene data from the fileName
		param[name]
			Name of the scene to save as
		param[name]
			filepath to get the scene data from
		return
			If the scene exists
		***************************************************************************/
		bool GetScene(std::string scene, std::string& filePath);

		/*!*************************************************************************
		brief
			Gets the AssetMap
		return
			Shared Pointer to the AssetType
		***************************************************************************/
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

		/*!*************************************************************************
		brief
			Gets the Asset from the AssetMap
		param[name]
			Name of the asset to get
		return
			The Asset Reference gotten
		***************************************************************************/
		template <typename T>
		T& GetAsset(std::string name)
		{
			if (GetAssetMap<T>()->mAssetList.size() == 0)
				assert("Asset does not exist");
			if (GetAssetMap<T>()->mAssetMap.count(name) == 0)
				assert("Asset does not exist");

			return GetAssetMap<T>()->mAssetList[GetAssetMap<T>()->mAssetMap[name]];
		}

		/*!*************************************************************************
		brief
			Registers the Asset (Creates a key for the asset into the map)
		***************************************************************************/
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

		/*!*************************************************************************
		brief
			Adds the Asset into the Asset Map
		param[name]
			Name of the asset to add
		param[asset]
			Asset reference to add into the Asset Map
		***************************************************************************/
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

			// The asset exist already
			if (GetAssetMap<T>()->mAssetMap.count(name) != 0)
			{
				// if it dose then we have to replace it in the vector
				GetAssetMap<T>()->mAssetList[GetAssetMap<T>()->mAssetMap[name]] = asset;
			}
			else
			{
				GetAssetMap<T>()->mAssetList.push_back(asset);
				// The map is name -> index
				// where index is where the asset belongs in mAssetList
				GetAssetMap<T>()->mAssetMap[name] = (unsigned int)GetAssetMap<T>()->mAssetList.size() - 1;

			}
		}

		/*!*************************************************************************
		brief
			Checks if the asset exists based on the name
		param[name]
			Name of the asset to check
		return
			If the asset exists
		***************************************************************************/
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


		enum class ASSETCOPIED
		{
			SUCCESS,
			FAILURE,
			MP3_ERROR
		};

		/*!*************************************************************************
		brief
			Checks if the asset exists based on the name
		param[source]
			Source file path of the asset to copy
		param[assetPath]
			The destination of the asset copied
		return
			Whether the copy was successful/unsupported.
		***************************************************************************/
		ASSETCOPIED CopyAssetToAssetsFolder(const std::string& source, const char* assetPath);
	private:


		//----------------------------  Graphics  ----------------------------
		/*!*************************************************************************
		brief
			Loads the shader and saves it as an asset
		param[shaderName]
			Name of the shader to create
		param[vertFile]
			File to generate vertex shader from
		param[fragFile]
			File to generate fragment shader from
		return
			Generated Shader ID
		***************************************************************************/
		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		/*!*************************************************************************
		brief
			Loads the Object from object file and saves it as an asset
		param[objName]
			Name of the Object to create
		param[modelFile]
			File to read the Object data from
		***************************************************************************/
		void LoadObject(const std::string& objName, const std::string& modelFile);
		/*!*************************************************************************
		brief
			Loads the Debug Object from debug object file and saves it as an asset
		param[objName]
			Name of the Debug Object to create
		param[modelFile]
			File to read the Debug Object data from
		***************************************************************************/
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		/*!*************************************************************************
		brief
			Initialises the 2D_Array of textures using enConfig.maxTexSize (width/height), and enConfig.maxNumTextures
		***************************************************************************/
		void InitTexture();
		/*!*************************************************************************
		brief
			Loads the Image from the provided file, and an optional SpriteSheetFile and saves it as an Texture asset
		param[textureName]
			Name of the Texture to create
		param[textureFile]
			File to read the Image from
		param[spriteSheetFile]
			File to read SpriteSheet data from (if any)
		***************************************************************************/
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);
		/*!*************************************************************************
		brief
			Splits the texture into multiple smaller textures (using the same ID, but using different Transfomation matrix to access the specific part of the Texture)
		param[t]
			Texture ID, and addition information passed in through the matrix in the Texture obj
		param[textureName]
			Base name of the texture to create
		param[num]
			Number of textures to split the texture into
		param[extName]
			Extension for the name when spliting the texture (added to the back of the textureName for readability) [ONLY IF NUM > 1]
		***************************************************************************/
		void AddTextureImage(Texture& t, const std::string& textureName, const int& num = 1, const std::string& extName = "");
		/*!*************************************************************************
		brief
			Load the Animation Atlas with reference to Texture Names, and time for the the animation should spend in that texture
		param[animName]
			Name of the Animation Object to create
		param[animFile]
			File to read the Animation Atlas from
		***************************************************************************/
		void LoadAnimation(const std::string& animName, const std::string& animFile);
		/*!*************************************************************************
		brief
			Initializes the FreeType Font Library
		***************************************************************************/
		void InitFontType();
		/*!*************************************************************************
		brief
			Loads the font from the given Font File and Font Height(quality) into a Font object (Single Texture)
		param[fontName]
			Font name to store as
		param[fontLoc]
			File to read the .ttf font from
		param[fontHeight]
			The height of the font to generate
		***************************************************************************/
		void LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight);

		/*!*************************************************************************
		brief
			Calculates and saves a generated circle vertex data into a text file (using my mesh reading specifications)
		param[numSlices]
			The number of slices of the circle to calculate
		param[modelFile]
			File name to save the circle's data in
		***************************************************************************/
		void ExportCircle(int numSlices, const std::string& modelFile);


		//----------------------------  Sound Assets  ----------------------------
		/*!*************************************************************************
		brief
			Initializes the FMod Sound System
		***************************************************************************/
		void InitSound();
		/*!*************************************************************************
		brief
			Loads a sound from the Sound File
		param[soundName]
			Name of the sound to save as
		param[soundFile]
			File name to load the sound data from
		***************************************************************************/
		void LoadSound(const std::string& soundName, const std::string& soundFile);

		
	};
}
#endif
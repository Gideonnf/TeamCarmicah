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
#include <stb/stb_image.h>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <FMOD/fmod.hpp>
#include <filesystem>
#include <unordered_map>
#include "Singleton.h"
#include "Systems/AssetTypes.h"
#include "PrefabSystem.h"
#include <mutex>
#include "FileWatcher.h"

namespace Carmicah
{

	//struct threadGraphicData
	//{
	//	std::string textureName;
	//	stbi_uc* dataPtr;
	//	std::string spriteSheetName;
	//};

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
		void LoadAssetFile(std::string assetFile);
		void InitialLoadThread(FileWatcher& fw);
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
		std::atomic_bool doneLoading = false;

		std::mutex inMutex;
		std::queue <std::tuple<std::string, stbi_uc*, std::string>> dataStuff;

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

#pragma region REMOVE_ASSETS
	
		template <typename T>
		void RemoveAsset(std::string name)
		{
			std::string assetType = typeid(T).name();

			// asset type doesn't exist
			if (mAssetTypeMap.count(assetType) == 0)
			{
				return;
			}

			auto& assetMap = GetAssetMap<T>()->mAssetMap;
			auto& assetList = GetAssetMap<T>()->mAssetList;


			// erase from the vector first
			if (assetMap.count(name) == 0)
			{
				CM_CORE_ERROR("Asset being removed does not exist");
			}

			unsigned int index = assetMap[name];

			if constexpr (std::is_same_v <T, FMOD::Sound>)
			{
				assetList[index].release();
			}

			assetMap.erase(name);

			// check if the index is correct before accessing vector
			if (index < assetList.size() - 1)
			{
				// swap with the back element
				std::swap(assetList[index], assetList.back());
				for (auto& pair : assetMap)
				{
					// find the old index of the asset before the swap
					if (pair.second == assetList.size() - 1)
					{
						// and set it to its new index
						pair.second = index;
						break;
					}
				}
			}

			assetList.pop_back();
		}

		template <>
		void RemoveAsset<FMOD::Sound>(std::string name)
		{
			std::string assetType = typeid(FMOD::Sound).name();

			// asset type doesn't exist
			if (mAssetTypeMap.count(assetType) == 0)
			{
				return;
			}

			auto& assetMap = GetAssetMap<FMOD::Sound>()->mAssetMap;
			auto& assetList = GetAssetMap<FMOD::Sound>()->mAssetList;

			// erase from the vector first
			if (assetMap.count(name) == 0)
			{
				CM_CORE_ERROR("Asset being removed does not exist");
			}

			unsigned int index = assetMap[name];
			assetList[index].release();
			assetMap.erase(name);

			// check if the index is correct before accessing vector
			if (index < assetList.size() - 1)
			{
				// swap with the back element
				//std::swap(assetList[index], assetList.back());
				//FMOD::Sound temp = assetList[index];
				assetList[index] = assetList.back();

				for (auto& pair : assetMap)
				{
					// find the old index of the asset before the swap
					if (pair.second == assetList.size() - 1)
					{
						// and set it to its new index
						pair.second = index;
						break;
					}
				}
			}

			assetList.pop_back();
		}

		template <>
		void RemoveAsset<Texture>(std::string name)
		{
			std::string assetType = typeid(Texture).name();

			// asset type doesn't exist
			if (mAssetTypeMap.count(assetType) == 0)
			{
				return;
			}

			auto& assetMap = GetAssetMap<Texture>()->mAssetMap;
			auto& assetList = GetAssetMap<Texture>()->mAssetList;

			std::string lowerFileName = name;
			std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), [](unsigned char c)
				{
					return std::tolower(c);
				});

			std::vector<std::string> assetsToRemove;

			for (const auto& entry : assetMap)
			{
				if (entry.first.find(name) == 0)
				{
					assetsToRemove.push_back(entry.first);
				}
			}

			//assetsToRemove.push_back(name);

			for (const auto& assetName : assetsToRemove)
			{
				// erase from the vector first
				if (assetMap.count(assetName) == 0)
				{
					CM_CORE_ERROR("Asset being removed does not exist");
				}

				unsigned int index = assetMap[assetName];

				assetMap.erase(assetName);

				// check if the index is correct before accessing vector
				if (index < assetList.size() - 1)
				{
					// swap with the back element
					std::swap(assetList[index], assetList.back());
					for (auto& pair : assetMap)
					{
						// find the old index of the asset before the swap
						if (pair.second == assetList.size() - 1)
						{
							// and set it to its new index
							pair.second = index;
							break;
						}
					}
				}

				assetList.pop_back();
			}
		}
#pragma endregion

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

		template <>
		bool AssetExist<Prefab>(std::string name)
		{
			std::string assetType = typeid(Prefab).name();
			if (mAssetTypeMap.count(assetType) == 0)
			{
				// If the asset doesn't exist yet, then create it
				RegisterAsset<Prefab>();
			}

			if (GetAssetMap<Prefab>()->mAssetList.size() == 0)
				return false;
			if (GetAssetMap<Prefab>()->mAssetMap.count(name) == 0)
				return false;

			return true;
		}

		// TODO: Handle removal of assets
		// cant rlly be done/tested until editor has ways to delete and add assets

		void RemoveFromAssetManager(std::string filePath);



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

		/*!*************************************************************************
		brief
			Renames the Scene
		param[oldName]
		The old name of the Scene
		param[newName]
			The new name of the Scene
		param[assetPath]
			The asset folder path
		***************************************************************************/
		void RenameScene(std::string oldName, std::string newName, const char* assetPath);

		void LoadTextureThreaded(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);

		/*!*************************************************************************
		brief
			Creates a Scene Name thats not a duplicate
		param[sceneName]
		The name of the Scene to be checked
		param[newName]
			The new name of the Scene
		return
			String for the Name of the scene
		***************************************************************************/
		std::string CreateSceneName(std::string sceneName);

		/*!*************************************************************************
		brief
			Clones a Scene
		param[sceneName]
		The name of the Scene to be cloned
		***************************************************************************/
		void CloneScene(std::string sceneName);

		/*!*************************************************************************
		brief
			Deletes a Scene
		param[sceneName]
		The name of the Scene to be deleted
		***************************************************************************/
		void DeleteScene(std::string sceneName);

	private:

		bool LoadAssetThreaded(File const& file, bool reload = false);
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
		void AddTextureImage(Texture& t, const std::string& textureName, const int& num = 1, const std::string& extName = "", const bool forcedSpriteSheet = false);
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